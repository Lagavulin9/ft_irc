/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:45:51 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/16 21:45:39 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

Server::Server(){};
Server::Server(const Server&){};
Server::Server(int port, std::string pass)
{
	std::cout << "Opening Socket..." << std::endl;
	_serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (_serv_sock == -1)
	{
		perror("Socket");
		exit(1);
	}
	bzero(&_serv_adr, sizeof(_serv_adr));
	_serv_adr.sin_family = AF_INET;
	_serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	_serv_adr.sin_port = htons(port);
	_port = port;
	_pass = pass;
	this->bind();
	this->listen();
	this->initPoll();
}

Server::~Server()
{
	for (int i=SERVER_POLLFD_IDX + 1; i < MAX_CLIENT; i++)
		close(_poll_fds[i].fd);
	close(_serv_sock);
	std::map<int,Client*>::iterator	client_it = _clients.begin();
	while (client_it != _clients.end())
	{
		delete client_it->second;
		client_it++;
	}
	std::map<std::string,Channel*>::iterator	channel_it = _channels.begin();
	while (channel_it != _channels.end())
	{
		delete channel_it->second;
		channel_it++;
	}
}

void	Server::listen(void)
{
	if (::listen(_serv_sock, MAX_CLIENT) == -1)
	{
		perror("Listen");
		exit(1);
	}
	std::cout << "Listening on " << _port << std::endl;
}

void	Server::bind(void)
{
	std::cout << "Binding..." << std::endl;
	if (::bind(_serv_sock, (struct sockaddr*)&_serv_adr, sizeof(_serv_adr)) == -1)
	{
		perror("Bind");
		exit(1);
	}
}

void	Server::accept(void)
{
	int					client_socket, i;
	socklen_t			client_addr_size;
	struct sockaddr_in	client_addr;

	client_socket = ::accept(_serv_sock, (struct sockaddr*)&client_addr, &client_addr_size);
	if (client_socket == -1)
	{
		perror("Accept");
		exit(1);
	}
	
	for (i = SERVER_POLLFD_IDX + 1; i < MAX_CLIENT; i++)
	{
		if (_poll_fds[i].fd < 0)
		{
			_poll_fds[i].fd = client_socket;
			_poll_fds[i].events = POLLIN;
			break;
		}
	}
	if (i == MAX_CLIENT)
	{
		this->clientWrite(client_socket, "The server is full");
		close(client_socket);
	}
	else
	{
		std::cout << "Client #"<< client_socket << " trying to connect to the server" << std::endl;
		this->registerClient(client_socket);
	}
}

void	Server::initPoll(void)
{
	_poll_fds[SERVER_POLLFD_IDX].fd = _serv_sock;
	_poll_fds[SERVER_POLLFD_IDX].events = POLLIN;

	for(int i = SERVER_POLLFD_IDX + 1; i < MAX_CLIENT; i++)
		_poll_fds[i].fd = -1;
}

void	Server::clientRead(void)
{
	int		r;
	char	buffer[BUFFER_SIZE];

	memset(buffer, 0, BUFFER_SIZE);
	for (int i = SERVER_POLLFD_IDX + 1; i < MAX_CLIENT; i++)
	{
		if (_poll_fds[i].fd < 0)
			continue;
		if (_poll_fds[i].revents & (POLLIN | POLLERR))
		{
			std::map<int, Client*>::iterator it = _clients.find(_poll_fds[i].fd);
			if (it == _clients.end())
				continue;
			Client	*client = it->second;
			r = recv(_poll_fds[i].fd, buffer, BUFFER_SIZE, MSG_DONTWAIT);
			if (r <= 0)
			{
				if (r == 0)
					std::cout << "Client #" << _poll_fds[i].fd <<" has left the server" << std::endl;
				else
					std::cout << "Client #" << _poll_fds[i].fd << "read error" << std::endl;
				this->removeClient(client);
				_poll_fds[i].fd = -1;
			}
			else
			{
				this->handleRequest(client, buffer);
			}
		}
	}
}

void	Server::clientWrite(int client_socket, std::string reply)
{
	write(client_socket, reply.c_str(), reply.length());
}

void	Server::registerClient(int client_socket)
{
	Client	*client = new Client(client_socket);
	_clients.insert(std::pair<int, Client*>(client_socket, client));
}

void	Server::removeClient(Client	*client)
{
	if (!client)
		return;
	int client_socket = client->getFD();
	close(client_socket);
	std::map<int, Client*>::iterator it = _clients.find(client_socket);
	if (it == _clients.end())
		return ;
	Client *to_remove = it->second;
	std::vector<Channel*> channel_list = to_remove->getChannels();
	std::vector<Channel*>::iterator to_leave = channel_list.begin();
	while (to_leave != channel_list.end())
	{
		(*to_leave)->removeClient(*to_remove);
		if ((*to_leave)->isEmpty())
		{
			_channels.erase((*to_leave)->getName());
			std::cout << "Channel " << (*to_leave)->getName() << " removed from the Server" << std::endl;
			delete *to_leave;
		}
		else
			(*to_leave)->announceQuit(*to_remove);
		to_leave++;
	}
	delete to_remove;
	_clients.erase(it);
	for (int i=SERVER_POLLFD_IDX + 1; i<MAX_CLIENT; i++)
	{
		if (_poll_fds[i].fd == client_socket)
		{
			_poll_fds[i].fd = -1;
			break ;
		}
	}
	std::cout << "Client #" << client_socket << " disconnected"<< std::endl;
}

void	Server::handleRequest(Client *client, std::string req)
{
	size_t		pos, i;
	std::string	line, cmd;
	std::string	_validCmds[] = { "JOIN","KICK","NICK","PART","PING","PRIVMSG","MODE","USER","PASS","QUIT" };

	client->addBuffer(req);
	while (1)
	{
		pos = client->getBuffer().find('\n');
		if (pos == std::string::npos)
			break ;
		line = client->getBuffer().substr(0, pos);
		std::cout << "[Client #" << client->getFD() << "] to [Server] << " << line << std::endl;
		client->setBuffer(client->getBuffer().substr(pos+1));

		std::vector<std::string>	cmd_info;
		setCommandInfo(line, cmd_info);
		if (cmd_info.empty())
			continue ;
		cmd = *(cmd_info.begin());
		for (i=0; i< NUMBER_OF_COMMANDS; i++)
		{
			if (cmd == _validCmds[i])
				break ;
		}
		if (!client->isAuthenticated() && i != PASS)
			continue;
		switch (i)
		{
			case JOIN: this->join(*client, cmd_info); break;
			case KICK: this->kick(*client, cmd_info); break;
			case NICK: this->nick(*client, cmd_info); break;
			case PART: printf("PART\n"); break;
			case PING: this->pong(*client); break;
			case PRIVMSG: this->privmsg(*client, cmd_info); break;
			case MODE: this->mode(*client, cmd_info); break;
			case USER: this->user(*client, cmd_info); break;
			case PASS: this->pass(*client, cmd_info); break;
			case QUIT: this->quit(*client); break;
			default: ERR_UNKNOWNCOMMAND(*client, cmd_info[0]); break;
		}
	}
}

void	Server::join(Client& client, std::vector<std::string> cmd_info)
{
	std::string	channel_name;
	Channel *channel;

	if (cmd_info.size() < 2)
		return ;
	channel_name = cmd_info[1];
	std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);
	if (it == _channels.end())
	{
		channel = new Channel(channel_name);
		channel->addOperator(client);
		_channels.insert(std::pair<std::string, Channel*>(channel_name, channel));
		client.addChannel(*channel);
	}
	else
	{
		channel = it->second;
		channel->addClient(client);
		client.addChannel(*channel);
	}
	channel->announceJoin(client);
}

void	Server::kick(Client& client, std::vector<std::string> cmd_info)
{
	std::string	channel_name = cmd_info[1];
	std::string	nick = cmd_info[2];
	std::map<std::string,Channel*>::iterator it = _channels.find(channel_name);
	if (it == _channels.end())
	{
		ERR_NOSUCHCHANNEL(client);
		return ;
	}
	Channel	*channel = it->second;
	if (channel->getOperator() != &client)
	{
		ERR_CHANOPRIVSNEEDED(client, *channel);
		return ;
	}
	std::vector<Client*> client_list = channel->getClients();
	std::vector<Client*>::iterator	iter = client_list.begin();
	while (iter != client_list.end())
	{
		Client *to_kick = *iter;
		if (to_kick->getNickName() == nick)
		{
			channel->announceKick(client, *to_kick);
			channel->removeClient(*to_kick);
			to_kick->removeChannel(*channel);
			return ;
		}
		iter++;
	}
	ERR_USERNOTINCHANNEL(client, *channel);
}

// void	Server::part(Client& client, std::vector<std::string> cmd_info)
// {
	
// }

void	Server::privmsg(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() < 3)
		return ;
	std::string	target = cmd_info[1];
	std::string msg = cmd_info[2];
	std::map<std::string,Channel*>::iterator it = _channels.find(target);
	if (it != _channels.end())
	{
		Channel	*channel = it->second;
		channel->broadcast(client, msg);
	}
}

void	Server::mode(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() < 2)
		return ;
	std::string	target = cmd_info[1];
	if (target[0] == '#')
	{
		std::map<std::string,Channel*>::iterator it = _channels.find(target);
		if (it == _channels.end())
			ERR_NOSUCHCHANNEL(client);
		else
		{
			Channel	*channel = it->second;
			RPL_CHANNELMODEIS(client, *channel);
		}
	}
	else
	{
		if (cmd_info.size() != 3)
			return ;
		if (cmd_info[2] != "+i" && cmd_info[2] != "+o")
			return ;
		RPL_USERMODE(client, cmd_info[2]);
	}
}

void	Server::pong(Client& client)
{
	RPL_PONG(client);
}

void	Server::user(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() != 5)
		return;
	client.setUserName(cmd_info[1]);
	client.setHostName(cmd_info[3]);
	client.setRealName(cmd_info[4]);
	this->welcomeClient(client);
}

void	Server::nick(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() != 2)
		return ;
	std::string nick = cmd_info[1];
	std::map<int,Client*>::iterator	it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second->getNickName() == nick)
		{
			if (client.isUserSet())
				ERR_NICKNAMEINUSE(client);
			else
			{
				client.setNickName(nick);
				client.setNickInUse(true);
			}
			return ;
		}
		it++;
	}
	client.setNickName(nick);
	client.setNickInUse(false);
	RPL_NICK(client);
	this->welcomeClient(client);
}

void	Server::pass(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() != 2)
		return ;
	if (cmd_info[1] != _pass)
	{
		ERR_PASSWDMISMATCH(client);
		return ;
	}
	client.setAuth(true);
	std::cout << "Client #" << client.getFD() << " connected to server" << std::endl;
}

void	Server::quit(Client& client)
{
	RPL_QUIT(client, client);
	this->removeClient(&client);
}

void	Server::setCommandInfo(std::string line, std::vector<std::string>& cmd_info)
{
	std::istringstream	iss(line);
	std::string	args;
	while (iss >> args)
	{
		if (args == "PRIVMSG")
		{
			cmd_info.push_back(args);
			if (iss >> args)
				cmd_info.push_back(args);
			size_t	pos = iss.str().find(":");
			// if (pos == std::string::npos)
			// 	send_some_error
			cmd_info.push_back(iss.str().substr(pos + 1));
			return ;
		}
		cmd_info.push_back(args);
	}
}

void	Server::welcomeClient(Client& client)
{
	if (!client.isWelcomed())
	{
		if (client.isUserSet())
		{
			if (client.isNickUsed())
				ERR_NICKNAMEINUSE(client);
			else if (client.isNickSet())
			{
				RPL_WELCOME(client);
				RPL_YOURHOST(client);
				RPL_CREATED(client);
				RPL_MYINFO(client);
				client.setWelcome(true);
			}
		}
	}
}

void	Server::launchServer(void)
{
	int	n;

	while (1)
	{
		n = poll(_poll_fds, MAX_CLIENT, 5000);
		if (n == 0)
			continue;
		if (_poll_fds[SERVER_POLLFD_IDX].revents & POLLIN)
			this->accept();
		this->clientRead();
	}
}

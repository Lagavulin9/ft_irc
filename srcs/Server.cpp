/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:45:51 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/14 22:55:33 by ijinhong         ###   ########.fr       */
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
		printf("Client #%d trying to connect to the server\n", client_socket);
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

void	Server::broadcast(int from, const std::string& msg)
{
	for (int i = SERVER_POLLFD_IDX + 1; i < MAX_CLIENT; i++)
	{
		if (_poll_fds[i].fd != from && _poll_fds[i].fd != -1)
			write(_poll_fds[i].fd, msg.c_str(), msg.length());
	}
	printf("Client #%d: %s", from, msg.c_str());
}

void	Server::clientRead(void)
{
	int							r;
	char						buffer[BUFFER_SIZE];

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
			r = read(_poll_fds[i].fd, buffer, BUFFER_SIZE);
			if (r <= 0)
			{
				if (r == 0)
					printf("Client #%d has left the server\n", _poll_fds[i].fd);
				else
					printf("Client #%d read error\n", _poll_fds[i].fd);
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
	std::map<int, Client*>::iterator it = _clients.find(client->getFD());
	if (it == _clients.end())
		return ;
	close(client->getFD());
	delete it->second;
	_clients.erase(it);
	std::cout << "Client removed" << std::endl;
}

void	Server::handleRequest(Client *client, std::string req)
{
	std::istringstream			iss(req);
	std::string					line, cmd;
	std::string					_validCmds[] = {
		"JOIN",
		"KICK", 
		"NICK",
		"PART",
		"PING",
		"PRIVMSG",
		"QUIT",
		"USER",
		"PASS",
		"NAME"
	};

	while (std::getline(iss, line, '\n'))
	{
		int	i;
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
		switch (i)
		{
			case JOIN: this->join(*client, cmd_info); break;
			case KICK: this->kick(*client, cmd_info); break;
			case NICK: this->nick(*client, cmd_info); break;
			case PART: printf("PART\n"); break;
			case PING: this->pong(*client); break;
			case PRIVMSG: this->privmsg(*client, cmd_info); break;
			case QUIT: printf("QUIT\n"); break;
			case USER: this->user(*client, cmd_info); break;
			case PASS: this->pass(*client, cmd_info); break;
			default: printf("Unknown Command\n"); break;
		}
	}
}

void	Server::join(Client& client, std::vector<std::string> cmd_info)
{
	std::string	channel_name;

	if (cmd_info.size() < 2)
		return ;
	channel_name = cmd_info[1];
	std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);
	if (it == _channels.end())
	{
		Channel *newChannel = new Channel(channel_name);
		newChannel->addOperator(client);
		_channels.insert(std::pair<std::string, Channel*>(channel_name, newChannel));
		client.addChannel(*newChannel);
	}
	else
	{
		Channel *channel = it->second;
		channel->addClient(client);
		client.addChannel(*channel);
	}
	std::cout << "Channels: ";
	for (it=_channels.begin(); it!=_channels.end(); it++)
	{
		std::cout << it->second->getName() << ", ";
	}
	std::cout << std::endl;
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
		ERR_CHANOPRIVSNEEDED(client);
		return ;
	}
	std::vector<Client*> client_list = channel->getClients();
	std::vector<Client*>::iterator	iter = client_list.begin();
	while (iter != client_list.end())
	{
		Client *to_kick = *iter;
		if (to_kick->getNickName() == nick)
		{
			if (to_kick == &client)
				return ;
			channel->removeClient(*to_kick);
			to_kick->removeChannel(*channel);
			std::cout << to_kick->getNickName() << " has been removed from channel: " << channel->getName() << std::endl;
			return ;
		}
		iter++;
	}
	ERR_USERNOTINCHANNEL(client);
}

// void	Server::part(Client& client, std::vector<std::string> cmd_info)
// {
	
// }

void	Server::privmsg(Client& client, std::vector<std::string> cmd_info)
{
	std::string	channel_name = cmd_info[1];
	std::map<std::string,Channel*>::iterator it = _channels.find(channel_name);
	if (it == _channels.end())
	{
		ERR_NOSUCHCHANNEL(client);
		return ;
	}
	Channel	*channel = it->second;
	std::string	msg;
	for (size_t i=2; i<cmd_info.size(); i++)
	{
		msg.append(cmd_info[i]);
		if (i != cmd_info.size() - 1)
			msg.append(" ");
	}
	channel->broadcast(client, msg);
}

// void	Server::quit(Client& client)
// {
	
// }

void	Server::pong(Client& client)
{
	RPL_PONG(client);
	std::cout << "Server Pong" << std::endl;
}

void	Server::user(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() != 5)
		return;
	client.setUserName(cmd_info[1]);
	client.setHostName(cmd_info[3]);
	client.setRealName(cmd_info[4]);
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
			ERR_NICKNAMEINUSE(client);
			nick.append("_JJap");
			break;
		}
		it++;
	}
	client.setNickName(nick);
	sendToClient(client, ":"+cmd_info[1]+"!"+client.getUserName()+"@localhost NICK "+nick+"\n");
}

void	Server::pass(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() != 2 || cmd_info[1] != _pass)
		return ;
	client.setAuth(true);
	std::cout << "Client #" << client.getFD() << " connected to server" << std::endl;
	RPL_WELCOME(client);
	RPL_YOURHOST(client);
	RPL_CREATED(client);
	RPL_MYINFO(client);
}

void	Server::setCommandInfo(std::string line, std::vector<std::string>& cmd_info)
{
	std::istringstream	iss(line);
	std::string	args;
	std::cout << "line: ";
	while (iss >> args)
	{
		cmd_info.push_back(args);
		std::cout << args << " ";
	}
	std::cout << std::endl;
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

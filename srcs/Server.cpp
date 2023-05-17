/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinholee <jinholee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:45:51 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/17 14:51:05 by jinholee         ###   ########.fr       */
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
	for (int i=SERVER_POLLFD_IDX + 1; i <=MAX_CLIENT; i++)
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
		close(client_socket);
		return ;
	}
	int flag = fcntl(client_socket, F_GETFL, 0);
	fcntl(client_socket, F_SETFL, flag | O_NONBLOCK);
	for (i = SERVER_POLLFD_IDX + 1; i <= MAX_CLIENT; i++)
	{
		if (_poll_fds[i].fd < 0)
		{
			_poll_fds[i].fd = client_socket;
			_poll_fds[i].events = POLLIN | POLLOUT;
			break;
		}
	}
	if (i > MAX_CLIENT)
	{
		send(client_socket, "The server is full", 18, MSG_DONTWAIT);
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

	for(int i = SERVER_POLLFD_IDX + 1; i <= MAX_CLIENT; i++)
		_poll_fds[i].fd = -1;
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
	for (int i=SERVER_POLLFD_IDX + 1; i<=MAX_CLIENT; i++)
	{
		if (_poll_fds[i].fd == client_socket)
		{
			_poll_fds[i].fd = -1;
			break ;
		}
	}
	std::cout << "Client #" << client_socket << " disconnected"<< std::endl;
}

void	Server::clientWrite()
{
	size_t	sent;
	std::map<int,Client*>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		Client *client = it->second;
		std::string to_send = client->getWriteBuffer();
		if (!to_send.empty())
		{
			sent = send(client->getFD(), to_send.c_str(), to_send.length(), MSG_DONTWAIT);
			std::cout << "[Server] to [Client #" << client->getFD() << "] >> " << to_send.substr(0,sent);
			client->setWriteBuffer(to_send.substr(sent));
		}
		it++;
	}
}

void	Server::clientRead(void)
{
	int		r;
	char	buffer[BUFFER_SIZE+1];

	memset(buffer, 0, BUFFER_SIZE+1);
	for (int i = SERVER_POLLFD_IDX + 1; i <= MAX_CLIENT; i++)
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
				this->handleRequest(client, buffer);
		}
	}
}

void	Server::handleRequest(Client *client, std::string req)
{
	size_t		pos, i;
	std::string	line, cmd;
	std::string	_validCmds[] = { "JOIN","KICK","MODE","NICK","PASS","PING","PRIVMSG","QUIT","USER" };

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

void	Server::welcomeClient(Client& client)
{
	if (!client.isWelcomed())
	{
		if (client.isUserSet())
		{
			if (client.isNickUsed())
				ERR_NICKNAMEINUSE(client, "");
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

void	Server::launchServer(bool& ServerShutdown)
{
	int	n;

	while (!ServerShutdown)
	{
		n = poll(_poll_fds, MAX_CLIENT+1, 0);
		if (n == 0)
			continue;
		if (_poll_fds[SERVER_POLLFD_IDX].revents & POLLIN)
			this->accept();
		this->clientRead();
		this->clientWrite();
	}
	this->shutdown();
}

void	Server::shutdown(void)
{
	std::cout << " Server Shutting down..." << std::endl;
	for (int i=SERVER_POLLFD_IDX+1; i<=MAX_CLIENT; i++)
		close(_poll_fds[i].fd);
	//system("leaks ircserv");
	exit(130);
}

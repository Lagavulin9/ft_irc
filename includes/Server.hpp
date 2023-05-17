/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:25:46 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/17 13:24:57 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __SERVER_CPP__
# define __SERVER_CPP__

# include "defines.hpp"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/poll.h>
# include <arpa/inet.h>
# include <iostream>
# include <string>
# include <sstream>
# include <vector>
# include <map>

class Client;
class Channel;

class Server {
private:
	struct sockaddr_in				_serv_adr;
	char							_buffer[BUFFER_SIZE];
	int								_serv_sock, _port;
	struct pollfd					_poll_fds[MAX_CLIENT+1];
	std::string						_pass;
	std::map<int, Client*>			_clients;
	std::map<std::string, Channel*>	_channels;

	Server();
	Server(const Server&);
	Server&	operator=(const Server&);
	void	listen(void);
	void	bind(void);
	void	accept(void);
	void	initPoll(void);
	void	clientRead(void);
	void	clientWrite(void);
	void	registerClient(int client_socket);
	void	removeClient(Client *client);
	void	setCommandInfo(std::string line, std::vector<std::string>& cmd_info);
	void	join(Client& client, std::vector<std::string> cmd_info);
	void	kick(Client& client, std::vector<std::string> cmd_info);
	void	part(Client& client, std::vector<std::string> cmd_info);
	void	privmsg(Client& client, std::vector<std::string> cmd_info);
	void	pass(Client& client, std::vector<std::string> cmd_info);
	void	user(Client& client, std::vector<std::string> cmd_info);
	void	nick(Client& client, std::vector<std::string> cmd_info);
	void	pong(Client& client);
	void	mode(Client& client, std::vector<std::string> cmd_info);
	void	quit(Client& client);
	void	handleRequest(Client *client, std::string req);
	void	welcomeClient(Client& client);
	void	shutdown(void);
public:
	Server(int port, std::string pass);
	~Server();

	void		launchServer(bool&);
};

#endif

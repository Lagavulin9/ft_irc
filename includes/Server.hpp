/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:25:46 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/04 21:48:26 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __SERVER_CPP__
# define __SERVER_CPP__

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <iostream>

# define BUFFER_SIZE 1024
# define MAX_CLIENT 10

class Server {
private:
	struct sockaddr_in	serv_adr, clnt_adr;
	char				buffer[BUFFER_SIZE];
	int					clnt_sock, clnt_adr_size;
	int					serv_sock, port;

	Server();
	Server(const Server&);
public:
	Server(int port);
	~Server();

	Server&	operator=(const Server&);

	void	listen(int backlog);
	void	accept(void);
	// void	client_read(int clnt_scok);
	void	broadcast(void);
};

#endif

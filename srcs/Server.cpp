/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:45:51 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/04 21:52:35 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(){};
Server::Server(const Server&){};
Server::Server(int port)
{
	std::cout << "Opening Socket..." << std::endl;
	this->serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (this->serv_sock == -1)
	{
		perror("Sock");
		exit(1);
	}
	bzero(&serv_adr, sizeof(serv_adr));
	this->serv_adr.sin_family = AF_INET;
	this->serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->serv_adr.sin_port = htons(port);
	this->port = port;

	std::cout << "Binding..." << std::endl;
	if (bind(this->serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
	{
		perror("Bind");
		exit(1);
	}
}

Server::~Server()
{
	close(this->serv_sock);
}

void	Server::listen(int backlog)
{
	if (::listen(this->serv_sock, backlog) == -1)
	{
		perror("Listen");
		exit(1);
	}
	std::cout << "Listening on " << this->port << std::endl;
}

void	Server::accept(void)
{
	this->clnt_sock = ::accept(serv_sock, (struct sockaddr*)&this->clnt_adr, (unsigned int*)&this->clnt_adr_size);
	if (this->clnt_sock == -1)
	{
		perror("Accept");
		exit(1);
	}
	std::cout << "Client: " << this->clnt_sock << " connected" << std::endl;
}

void	Server::broadcast(void)
{
	int	str_len;

	do
	{
		str_len = read(clnt_sock, this->buffer, BUFFER_SIZE);
		if (str_len > 0)
		{
			write(clnt_sock, "FROM Server: ", 13);
			write(clnt_sock, buffer, str_len);
		}
	} while (str_len > 0);
	close(clnt_sock);
}

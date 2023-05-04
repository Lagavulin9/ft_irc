/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:19:54 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/04 18:56:11 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int	main(int argc, char **argv)
{
	int	port;

	if (argc != 2)
	{
		std::cout << "Usage " << argv[0] << " <port>" << std::endl;
		exit(1);
	}
	std::stringstream iss(argv[1]);
	iss >> port;
	Server serv(port);
	serv.listen(port);
	serv.accept();
	serv.broadcast();
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:19:54 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/17 02:00:17 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"
#include "../includes/Server.hpp"

bool ServerShutdown = false;

void	signalHandler(int signo)
{
	(void)signo;
	ServerShutdown = true;
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage " << argv[0] << " <port> <password>" << std::endl;
		exit(1);
	}
	signal(SIGINT, signalHandler);
	Server	serv(myAtoi(argv[1]), argv[2]);
	serv.launchServer(ServerShutdown);
	return (0);
}

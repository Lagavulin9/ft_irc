/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:19:54 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/16 18:50:38 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"
#include "../includes/Server.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage " << argv[0] << " <port> <password>" << std::endl;
		exit(1);
	}
	Server	serv(myAtoi(argv[1]), argv[2]);
	serv.launchServer();
	return (0);
}

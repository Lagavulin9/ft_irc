/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:34:15 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/10 20:35:54 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

void	RPL_WELCOME(Client& client)
{
	std::string	reply;

	reply = "001\n Welcome to the Internet Relay Network " + client.getNickName() + "!" + client.getUserName() + "@localhost\n";
	send(client.getFD(), reply.c_str(), reply.length(), 0);
}

void	RPL_YOURHOST(Client& client)
{
	std::string	reply;

	reply = "002\n Your host is jinholee_IRC, running version 1.0\n";
	send(client.getFD(), reply.c_str(), reply.length(), 0);
}

void	RPL_CREATED(Client& client)
{
	std::string	reply;

	reply = "003\n This server was created 42 years ago\n";
	send(client.getFD(), reply.c_str(), reply.length(), 0);
}

void	RPL_MYINFO(Client& client)
{
	std::string	reply;

	reply = "004\n jinholee_IRC v1.0 +i +o\n";
	send(client.getFD(), reply.c_str(), reply.length(), 0);
}

void	RPL_PONG(Client& client)
{
	std::string	reply;

	reply = "PONG " + client.getUserName() + "\n";
	send(client.getFD(), reply.c_str(), reply.length(), 0);
}
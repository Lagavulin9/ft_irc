/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:34:15 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/14 21:05:46 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

void	sendToClient(Client& client, std::string reply)
{
	send(client.getFD(), reply.c_str(), reply.length(), 0);
}

void	RPL_WELCOME(Client& client)
{
	sendToClient(client, "001\nWelcome to the Internet Relay Network " + client.getNickName() + "!" + client.getUserName() + "@localhost\n");
}

void	RPL_YOURHOST(Client& client)
{
	sendToClient(client, "002\nYour host is jinholee_IRC, running version 1.0\n");
}

void	RPL_CREATED(Client& client)
{
	sendToClient(client, "003\nThis server was created 42 years ago\n");
}

void	RPL_MYINFO(Client& client)
{
	sendToClient(client,  "004\njinholee_IRC v1.0 +i +o\n");
}

void	RPL_PONG(Client& client)
{
	sendToClient(client, "PONG " + client.getUserName() + "\n");
}

void	ERR_NOSUCHNICK(Client& client)
{
	sendToClient(client, "401\nNo Such Nick\n");
}

void	ERR_NOSUCHCHANNEL(Client& client)
{
	sendToClient(client, "403\nNo Such Channel\n");
}

void	ERR_NICKNAMEINUSE(Client& client)
{
	sendToClient(client ,"433\nNick already in use\n");
}

void	ERR_USERNOTINCHANNEL(Client& client)
{
	sendToClient(client, "441\nUser not in channel\n");
}

void	ERR_CHANOPRIVSNEEDED(Client& client)
{
	sendToClient(client, "482\nYou are not channel operator\n");
}
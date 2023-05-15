/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:34:15 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/15 19:45:48 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

void	sendToClient(Client& client, std::string reply)
{
	send(client.getFD(), reply.c_str(), reply.length(), 0);
	std::cout << "[Server] to [Client #" << client.getFD() << "] >> " << reply;
}

void	RPL_WELCOME(Client& client)
{
	sendToClient(client, ":localhost 001 "+client.getNickName()+" :Welcome to the Internet Relay Network\n");
}

void	RPL_YOURHOST(Client& client)
{
	sendToClient(client, ":localhost 002 "+client.getNickName()+" :Your host is jinholee_IRC, running version 1.0\n");
}

void	RPL_CREATED(Client& client)
{
	sendToClient(client, ":localhost 003 "+client.getNickName()+" :This server was created 42 years ago\n");
}

void	RPL_MYINFO(Client& client)
{
	sendToClient(client,  ":localhost 004 "+client.getNickName()+" :jinholee_IRC v1.0 +i +o\n");
}

void	RPL_CHANNELMODEIS(Client& client, Channel& channel)
{
	sendToClient(client, ":localhost 324 "+client.getNickName()+" "+channel.getName()+"\n");
}

void	RPL_NAMREPLY(Client& client, Channel& channel)
{
	sendToClient(client, ":localhost 353 "+client.getUserName()+" = "+channel.getName()+" :"+channel.getClientNameList()+"\n");
}

void	RPL_ENDOFNAMES(Client& client, std::string channel_name)
{
	sendToClient(client, ":localhost 366 "+client.getUserName()+" "+channel_name+" :End of /NAMES list\n");
}

void	RPL_NICK(Client& client)
{
	sendToClient(client, ":"+client.getNickName()+"!"+client.getUserName()+"@localhost NICK "+client.getNickName()+"\n");
}

void	RPL_JOIN(Client& client, std::string from, std::string channel_name)
{
	sendToClient(client, ":"+from+"!"+client.getUserName()+"@localhost"+" JOIN :"+channel_name+"\n");
}

void	RPL_PONG(Client& client)
{
	sendToClient(client, "PONG " + client.getNickName() + "\n");
}

void	RPL_USERMODE(Client& client, std::string mod)
{
	sendToClient(client, ":"+client.getNickName()+" MODE "+client.getNickName()+" :"+mod+"\n");
}

void	ERR_NOSUCHNICK(Client& client)
{
	sendToClient(client, ":localhost 4?? :No Such Nick\n");
}

void	ERR_NOSUCHCHANNEL(Client& client)
{
	sendToClient(client, ":localhost 403 :No Such Channel\n");
}

void	ERR_UNKNOWNCOMMAND(Client& client, std::string cmd)
{
	sendToClient(client, ":localhost 421 "+client.getNickName()+" "+cmd+" : Unknown Command\n");
}

void	ERR_NICKNAMEINUSE(Client& client)
{
	sendToClient(client ,":localhost 433 "+client.getNickName()+" "+client.getUserName()+" :Nick already in use\n");
}

void	ERR_USERNOTINCHANNEL(Client& client)
{
	sendToClient(client, ":localhost 441 :User not in channel\n");
}

void	ERR_CHANOPRIVSNEEDED(Client& client)
{
	sendToClient(client, ":localhost 482 :You are not channel operator\n");
}

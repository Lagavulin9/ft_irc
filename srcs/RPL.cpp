/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:34:15 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/16 20:59:32 by ijinhong         ###   ########.fr       */
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
	sendToClient(client, ":"+client.getOldNick()+"!"+client.getUserName()+"@localhost NICK "+client.getNickName()+"\n");
}

void	RPL_JOIN(Client& client, std::string from, std::string channel_name)
{
	sendToClient(client, ":"+from+"!"+client.getUserName()+"@localhost"+" JOIN :"+channel_name+"\n");
}

void	RPL_KICK(Client& client, Client& from, Client& to_kick, Channel& channel)
{
	sendToClient(client, ":"+from.getNickName()+"!"+client.getUserName()+"@localhost KICK "+channel.getName()+" "+to_kick.getNickName()+" :Kicked by the channel's operator\n");
}

void	RPL_PONG(Client& client)
{
	sendToClient(client, "PONG " + client.getNickName() + "\n");
}

void	RPL_USERMODE(Client& client, std::string mod)
{
	sendToClient(client, ":"+client.getNickName()+" MODE "+client.getNickName()+" :"+mod+"\n");
}

void	RPL_QUIT(Client& client, Client& from)
{
	sendToClient(client, ":"+from.getNickName()+"!"+from.getUserName()+"@localhost QUIT :QUIT :leaving\n");
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

void	ERR_USERNOTINCHANNEL(Client& client, Channel& channel)
{
	sendToClient(client, ":localhost 441 "+client.getNickName()+" "+channel.getName()+" :User not in channel\n");
}

void	ERR_PASSWDMISMATCH(Client& client)
{
	sendToClient(client, ":localhost 464  :Password Incorrect.\n");
}

void	ERR_CHANOPRIVSNEEDED(Client& client, Channel& channel)
{
	sendToClient(client, ":localhost 482 "+client.getNickName()+" "+channel.getName()+" :You are not channel operator\n");
}

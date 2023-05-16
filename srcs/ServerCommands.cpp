/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 00:58:21 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/17 02:27:31 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

void	Server::setCommandInfo(std::string line, std::vector<std::string>& cmd_info)
{
	std::istringstream	iss(line);
	std::string	args;
	while (iss >> args)
	{
		if (args == "PRIVMSG")
		{
			cmd_info.push_back(args);
			if (iss >> args)
				cmd_info.push_back(args);
			size_t	pos = iss.str().find(":");
			if (pos == std::string::npos)
				return ;
			cmd_info.push_back(iss.str().substr(pos + 1));
			return ;
		}
		cmd_info.push_back(args);
	}
}

void	Server::join(Client& client, std::vector<std::string> cmd_info)
{
	std::string	channel_name;
	Channel *channel;

	if (cmd_info.size() < 2)
		return ;
	channel_name = cmd_info[1];
	std::map<std::string, Channel*>::iterator it = _channels.find(channel_name);
	if (it == _channels.end())
	{
		channel = new Channel(channel_name);
		channel->addOperator(client);
		_channels.insert(std::pair<std::string, Channel*>(channel_name, channel));
		client.addChannel(*channel);
	}
	else
	{
		channel = it->second;
		channel->addClient(client);
		client.addChannel(*channel);
	}
	channel->announceJoin(client);
}

void	Server::kick(Client& client, std::vector<std::string> cmd_info)
{
	std::string	channel_name = cmd_info[1];
	std::string	nick = cmd_info[2];
	std::map<std::string,Channel*>::iterator it = _channels.find(channel_name);
	if (it == _channels.end())
	{
		ERR_NOSUCHCHANNEL(client);
		return ;
	}
	Channel	*channel = it->second;
	if (channel->getOperator() != &client)
	{
		ERR_CHANOPRIVSNEEDED(client, *channel);
		return ;
	}
	std::vector<Client*> client_list = channel->getClients();
	std::vector<Client*>::iterator	iter = client_list.begin();
	while (iter != client_list.end())
	{
		Client *to_kick = *iter;
		if (to_kick->getNickName() == nick)
		{
			channel->announceKick(client, *to_kick);
			channel->removeClient(*to_kick);
			to_kick->removeChannel(*channel);
			return ;
		}
		iter++;
	}
	ERR_USERNOTINCHANNEL(client, *channel);
}

// void	Server::part(Client& client, std::vector<std::string> cmd_info)
// {
	
// }

void	Server::privmsg(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() < 3)
		return ;
	std::string	target = cmd_info[1];
	std::string msg = cmd_info[2];
	std::map<std::string,Channel*>::iterator it = _channels.find(target);
	if (it != _channels.end())
	{
		Channel	*channel = it->second;
		channel->broadcast(client, msg);
	}
}

void	Server::mode(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() < 2)
		return ;
	std::string	target = cmd_info[1];
	if (target[0] == '#')
	{
		std::map<std::string,Channel*>::iterator it = _channels.find(target);
		if (it == _channels.end())
			ERR_NOSUCHCHANNEL(client);
		else
		{
			Channel	*channel = it->second;
			RPL_CHANNELMODEIS(client, *channel);
		}
	}
	else
	{
		if (cmd_info.size() != 3)
			return ;
		if (cmd_info[2] != "+i" && cmd_info[2] != "+o")
			return ;
		RPL_USERMODE(client, cmd_info[2]);
	}
}

void	Server::pong(Client& client)
{
	RPL_PONG(client);
}

void	Server::user(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() != 5)
		return;
	client.setUserName(cmd_info[1]);
	client.setHostName(cmd_info[3]);
	client.setRealName(cmd_info[4]);
	this->welcomeClient(client);
}

void	Server::nick(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() != 2)
		return ;
	std::string nick = cmd_info[1];
	std::map<int,Client*>::iterator	it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second->getNickName() == nick)
		{
			if (client.isUserSet())
				ERR_NICKNAMEINUSE(client);
			else
			{
				client.setNickName(nick);
				client.setNickInUse(true);
			}
			return ;
		}
		it++;
	}
	client.setNickName(nick);
	client.setNickInUse(false);
	RPL_NICK(client);
	this->welcomeClient(client);
}

void	Server::pass(Client& client, std::vector<std::string> cmd_info)
{
	if (cmd_info.size() != 2)
		return ;
	if (cmd_info[1] != _pass)
	{
		ERR_PASSWDMISMATCH(client);
		return ;
	}
	client.setAuth(true);
	std::cout << "Client #" << client.getFD() << " connected to server" << std::endl;
}

void	Server::quit(Client& client)
{
	RPL_QUIT(client, client);
	this->removeClient(&client);
}

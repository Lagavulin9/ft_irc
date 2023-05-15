/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 16:56:16 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/15 20:11:30 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

Channel::Channel(const std::string& name):
	_name(name),
	_operator(0)
{}

Channel::~Channel()
{}

const std::string&	Channel::getName(void)
{
	return (_name);
}

const Client*	Channel::getOperator(void)
{
	return (_operator);
}

const std::vector<Client*>&	Channel::getClients(void)
{
	return (_clients);
}

std::string	Channel::getClientNameList(void)
{
	std::string	list;
	std::vector<Client*>::iterator	it = _clients.begin();
	while (it != _clients.end())
	{
		if ((*it) == _operator)
			list += "@";
		list += (*it)->getNickName() + " ";
		it++;
	}
	return (list);
}

bool	Channel::isEmpty(void)
{
	return (_clients.empty());
}

void	Channel::addOperator(Client& client)
{
	_operator = &client;
	this->addClient(client);
}

void	Channel::addClient(Client& client)
{
	std::vector<Client*>::iterator	it;
	it = std::find(_clients.begin(), _clients.end(), &client);
	if (it == _clients.end())
		_clients.push_back(&client);
}

void	Channel::removeClient(Client& client)
{
	std::vector<Client*>::iterator	it;
	it = std::find(_clients.begin(), _clients.end(), &client);
	if (it != _clients.end())
	{
		if (*it == _operator)
			_operator = 0;
		_clients.erase(it);
	}
}

void	Channel::broadcast(Client& from, std::string msg)
{
	std::vector<Client*>::iterator	it = _clients.begin();
	while (it != _clients.end())
	{
		Client	*to_send = *it;
		if (to_send != &from)
			sendToClient(*to_send, ":"+from.getNickName()+"!"+from.getUserName()+"@localhost PRIVMSG "+_name+" :"+msg+"\n");
		it++;
	}
}

void	Channel::broadcastAll(std::string msg)
{
	std::vector<Client*>::iterator	it = _clients.begin();
	while (it != _clients.end())
	{
		Client	*to_send = *it;
		sendToClient(*to_send, msg);
		it++;
	}
}

void	Channel::announce(Client& from)
{
	std::vector<Client*>::iterator	it = _clients.begin();
	while (it != _clients.end())
	{
		RPL_JOIN(*(*it), from.getNickName(), _name);
		RPL_NAMREPLY(*(*it), *this);
		RPL_ENDOFNAMES(*(*it), _name);
		it++;
	}
}
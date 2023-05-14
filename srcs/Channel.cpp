/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 16:56:16 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/14 22:48:24 by ijinhong         ###   ########.fr       */
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
			sendToClient(*to_send, msg);
		it++;
	}
}

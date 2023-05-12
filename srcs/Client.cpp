/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:46:18 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/10 20:26:55 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"
#include "../includes/Client.hpp"

Client::Client(int socketfd):
	_authenticated(false),
	_socketfd(socketfd),
	_roll("normal")
{
	printf("Client created\n");
}

Client::~Client()
{
}

void	Client::setAuth(bool auth)
{
	_authenticated = auth;
}

void	Client::setUserName(std::string new_name)
{
	_username = new_name;
}

void	Client::setRealName(std::string new_name)
{
	_realname = new_name;
}

void	Client::setHostName(std::string new_name)
{
	_hostname = new_name;
}

void	Client::setNickName(std::string new_name)
{
	_nick = new_name;
}

void	Client::setRoll(std::string roll)
{
	_roll = roll;
}

void	Client::addChannel(Channel& channel)
{
	_channels.push_back(&channel);
}

bool	Client::isAuthenticated()
{
	return (this->_authenticated);
}

int	Client::getFD()
{
	return (this->_socketfd);
}

const std::string&	Client::getUserName()
{
	return (this->_username);
}

const std::string&	Client::getRealName()
{
	return (this->_realname);
}

const std::string&	Client::getHostName()
{
	return (this->_hostname);
}

const std::string&	Client::getNickName()
{
	return (this->_nick);
}

const std::string&	Client::getRoll()
{
	return (this->_roll);
}

const std::vector<Channel*>	Client::getChannels()
{
	return (this->_channels);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:46:18 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/17 13:30:51 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"
#include "../includes/Client.hpp"

Client::Client(int socketfd):
	_authenticated(false),
	_welcomed(false),
	_nickInUse(false),
	_socketfd(socketfd),
	_roll("normal")
{
	std::cout << "Client created" << std::endl;
}

Client::~Client()
{
}

void	Client::setAuth(bool auth)
{
	_authenticated = auth;
}

void	Client::setWelcome(bool welcome)
{
	if (!_welcomed)
		_welcomed = welcome;
}

void	Client::setNickInUse(bool flag)
{
	_nickInUse = flag;
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
	_oldnick = _nick;
	_nick = new_name;
}

void	Client::setRoll(std::string roll)
{
	_roll = roll;
}

void	Client::setBuffer(std::string str)
{
	_readBuffer = str;
}

void	Client::setWriteBuffer(std::string buffer)
{
	_writeBuffer = buffer;
}

void	Client::addChannel(Channel& channel)
{
	std::vector<Channel*>::iterator	it;
	it = std::find(_channels.begin(), _channels.end(), &channel);
	if (it == _channels.end())
		_channels.push_back(&channel);
}

void	Client::addBuffer(std::string buffer)
{
	_readBuffer += buffer;
}

void	Client::addWriteBuffer(std::string buffer)
{
	_writeBuffer += buffer;
}

bool	Client::isAuthenticated()
{
	return (this->_authenticated);
}

bool	Client::isWelcomed()
{
	return (this->_welcomed);
}

bool	Client::isNickSet()
{
	return (!_nick.empty());
}

bool	Client::isNickUsed()
{
	return (_nickInUse);
}

bool	Client::isUserSet()
{
	return (!(_username.empty() || _realname.empty() || _hostname.empty()));
}

int	Client::getFD()
{
	return (this->_socketfd);
}

std::string	Client::getBuffer()
{
	return (this->_readBuffer);
}

std::string	Client::getWriteBuffer()
{
	return (this->_writeBuffer);
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

const std::string&	Client::getOldNick()
{
	return (this->_oldnick);
}

const std::string&	Client::getRoll()
{
	return (this->_roll);
}

std::vector<Channel*>	Client::getChannels()
{
	return (this->_channels);
}

void	Client::removeChannel(Channel& channel)
{
	std::vector<Channel*>::iterator	it = _channels.begin();
	while (it != _channels.end())
	{
		Channel	*to_remove = *it;
		if (to_remove == &channel)
			_channels.erase(it);
	}
}

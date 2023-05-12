/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 23:23:19 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/10 20:14:48 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __USER_HPP__
# define __USER_HPP__

# include <unistd.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/poll.h>
# include <arpa/inet.h>
# include <iostream>
# include <string>
# include <sstream>

class Channel;
class Server;

class Client {
private:
	bool					_authenticated;
	int						_socketfd;
	std::string				_username;
	std::string				_realname;
	std::string				_hostname;
	std::string				_nick;
	std::string				_roll;
	std::vector<Channel*>	_channels;
	Client();
	Client(const Client&);
	Client&	operator=(const Client&);
public:
	Client(int _socketfd);
	//Client(int _socketfd, std::string nick, std::string username);
	~Client();

	void				setAuth(bool);
	void				setUserName(std::string);
	void				setRealName(std::string);
	void				setHostName(std::string);
	void				setNickName(std::string);
	void				setRoll(std::string);
	void				addChannel(Channel& channel);
	bool				isAuthenticated();
	int					getFD(void);
	const std::string&	getUserName(void);
	const std::string&	getRealName(void);
	const std::string&	getHostName(void);
	const std::string&	getNickName(void);
	const std::string&	getRoll(void);
	const std::vector<Channel*> getChannels(void);
};

#endif

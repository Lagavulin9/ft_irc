/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinholee <jinholee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 23:23:19 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/17 13:58:45 by jinholee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __USER_HPP__
# define __USER_HPP__

# include <iostream>
# include <string>
# include <sstream>
# include <vector>

class Channel;
class Server;

class Client {
private:
	bool					_authenticated;
	bool					_welcomed;
	bool					_nickInUse;
	int						_socketfd;
	std::string				_username;
	std::string				_realname;
	std::string				_hostname;
	std::string				_nick;
	std::string				_oldnick;
	std::string				_roll;
	std::vector<Channel*>	_channels;
	std::string				_readBuffer;
	std::string				_writeBuffer;
	Client();
	Client(const Client&);
	Client&	operator=(const Client&);
public:
	Client(int _socketfd);
	//Client(int _socketfd, std::string nick, std::string username);
	~Client();

	void				setAuth(bool);
	void				setWelcome(bool);
	void				setNickInUse(bool);
	void				setUserName(std::string);
	void				setRealName(std::string);
	void				setHostName(std::string);
	void				setNickName(std::string);
	void				setRoll(std::string);
	void				setBuffer(std::string);
	void				setWriteBuffer(std::string);
	void				addChannel(Channel& channel);
	void				addBuffer(std::string);
	void				addWriteBuffer(std::string);
	bool				isAuthenticated(void);
	bool				isWelcomed(void);
	bool				isNickUsed(void);
	bool				isNickSet(void);
	bool				isUserSet(void);
	int					getFD(void);
	std::string			getBuffer(void);
	std::string			getWriteBuffer(void);
	const std::string&	getUserName(void);
	const std::string&	getRealName(void);
	const std::string&	getHostName(void);
	const std::string&	getNickName(void);
	const std::string&	getOldNick(void);
	const std::string&	getRoll(void);
	std::vector<Channel*> getChannels(void);
	void				removeChannel(Channel&);
};

#endif

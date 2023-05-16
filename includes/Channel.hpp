/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 23:32:33 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/17 00:56:34 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __CHANNEL_HPP__
# define __CHANNEL_HPP__

# include <iostream>
# include <string>
# include <vector>

class Client;
class Server;

class Channel {
private:
	std::string				_name;
	std::vector<Client*>	_clients;
	Client*					_operator;

	Channel();
	Channel(const Channel&);
	Channel&	operator=(const Channel&);
public:
	Channel(const std::string& name);
	~Channel();

	const std::string&			getName(void);
	const Client*				getOperator(void);
	const std::vector<Client*>&	getClients(void);
	std::string					getClientNameList(void);
	bool						isEmpty(void);
	void						addOperator(Client& client);
	void						addClient(Client& client);
	void						removeClient(Client& client);
	void						broadcast(Client&, std::string);
	void						broadcastAll(std::string);
	void						announceJoin(Client& from);
	void						announceKick(Client& from, Client& to_kick);
	void						announceQuit(Client& from);
};

#endif

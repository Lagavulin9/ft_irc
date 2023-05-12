/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 22:07:39 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/10 18:52:06 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __IRC_HPP__
# define __IRC_HPP__

# include "defines.hpp"
# include "Command.hpp"
# include "Client.hpp"
# include "Channel.hpp"
# include "Server.hpp"
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
# include <vector>
# include <map>

int		myAtoi(const std::string& str);
void	RPL_WELCOME(Client& client);
void	RPL_YOURHOST(Client& client);
void	RPL_CREATED(Client& client);
void	RPL_MYINFO(Client& client);
void	RPL_PONG(Client& client);

#endif

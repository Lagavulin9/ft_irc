/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinholee <jinholee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 00:18:03 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/17 14:19:04 by jinholee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __DEFINES_HPP__
# define __DEFINES_HPP__

# define VERSION 1.0
# define BUFFER_SIZE 4096
# define MAX_CLIENT 5
# define SERVER_POLLFD_IDX 0
# define SERVERNAME minji_IRC
# define NUMBER_OF_COMMANDS 9

enum e_commands
{
	JOIN,KICK,MODE,NICK,PASS,PING,PRIVMSG,QUIT,USER
};

#endif

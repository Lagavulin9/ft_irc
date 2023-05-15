/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 00:18:03 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/15 18:46:26 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __DEFINES_HPP__
# define __DEFINES_HPP__

# define VERSION 1.0
# define BUFFER_SIZE 1024
# define MAX_CLIENT 10
# define SERVER_POLLFD_IDX 0
# define SERVERNAME jinholee_IRC
# define NUMBER_OF_COMMANDS 9

enum e_commands {
	JOIN, KICK, NICK, PART, PING, PRIVMSG, MODE, USER, PASS
};

#endif

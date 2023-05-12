/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijinhong <ijinhong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 22:08:51 by ijinhong          #+#    #+#             */
/*   Updated: 2023/05/09 19:09:39 by ijinhong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

int	myAtoi(const std::string& str)
{
	int	ret;

	std::stringstream	ss(str);
	ss >> ret;
	if (ss.fail() || !ss.eof() || ret < 0)
	{
		std::cerr << "Invalid port" << std::endl;
		exit(1);
	}
	return (ret);
}

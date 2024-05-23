/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseListen.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:23:51 by dacortes          #+#    #+#             */
/*   Updated: 2024/05/11 14:21:31 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parsing.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
# define IP4 1
# define IP6 2
# define IS_IP 3
# define NO_IP 4
# define ERROR -1

short parsingListen(std::string &listen)
{
	size_t words = ::wordCounter(listen);
	if (!words or words > 1)
		return (false);
	size_t find = listen.find("[::]:");
	std::string::iterator it = listen.begin();
	if (find != std::string::npos)
		return ((*it != '[' ? -2 : 0) + IP6);//posible error
	find = listen.find(":");
	if (*it != ':' and find != std::string::npos)
	{
		
		return (IS_IP);
	}
	it += (*it == ':' ? 1 : 0);
	for (; it != listen.end(); ++it)
		if (!isdigit(*it))
			return (-1); // si es -1 se debe validar el error
	return (NO_IP);
}
//web/port
//ip/port
//port

std::string getIp(std::string &listen)
{
	short check = parsingListen(listen);
	if (check == IP6 or check == NO_IP)
		return ("");
	if (check == IS_IP)
		return (std::string(listen.begin(), listen.begin() + listen.find(':')));
	return ("");
}

std::string getPort(std::string &listen)
{
	short check = parsingListen(listen);
	if (check == IP6)
		return (std::string(listen.begin() + 5, listen.end()));
	if (check == IS_IP)
		return (std::string(listen.begin()
			+ (listen.find(':') + 1), listen.end()));
	if (check == NO_IP)
		return (std::string(listen.begin()
			+ (listen[0] == ':' ? 1 : 0), listen.end()));
	return ("");
}

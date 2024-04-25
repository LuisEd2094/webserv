/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 09:22:27 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/25 14:48:17 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parsing.hpp"

Parsing::Parsing( void )
{
	_methods.push_back(std::string("GET"));
	_methods.push_back(std::string("POST"));
	_methods.push_back(std::string("DELETE"));
	_findNewline = 0;
}

Parsing::~Parsing( void )
{}

/*
 * Membert Funtions
*/
std::string Parsing::readSocket(int fd)
{
	char buffRead[BUFFER_READ + 1];
	if (fd == -1)
		std::cout << "Error: open" << std::endl;
	int	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, buffRead, BUFFER_READ);
		if (bytes < 0)
			break ;
		buffRead[bytes] = '\0';
		_read += buffRead;
	}
	return (_read);
}

bool Parsing::isEmptyLine(const std::string& line) const
{
	return (!line[0] or (line[0] == '\r' and line[1] == '\n')
		or line[0] == '\n');
}

bool Parsing::isMethods(const std::string& keyword) const
{
	return ((std::find(_methods.begin(), _methods.end(), keyword)
	== _methods.end()) ? EXIT_FAILURE : EXIT_SUCCESS);
}

bool Parsing::isVersion(const std::string& version) const
{
	return (version != "HTTP/1.1");
}

bool Parsing::checkMethod(const std::string& strRead)
{
	size_t	word = 0;
	size_t	space = 0;
	size_t	tmp = 0;

	//\r \n --- \r\n --- \n
	_findNewline = strRead.find('\n');
	for (size_t i = 0; i < _findNewline; i++)
	{
		tmp = word;
		if (!::isblank(strRead[i]) and word != 3)
		{
			(word == 0 ? _method.method += strRead[i] : _method.method);
			(word == 1 ? _method.requested += strRead[i] : _method.requested);
			(word == 2 ? _method.version += strRead[i] : _method.version);
		}
		else
		{
			word += (::isblank(strRead[i]) and (strRead[i + 1]
				and !::isblank(strRead[i + 1])) ? 1 : 0);
			space += (::isblank(strRead[i]) ? 1 : 0);
			if (space > 2)
			{
				std::cout << "Error: format space" << std::endl;
				return (EXIT_FAILURE);
			}
			space = (tmp != word ? 0 : space);
		}
	}
	if (word > 2 or isMethods(_method.method) or isVersion(_method.version))
	{
		std::cout << "Error: invalid " <<
		(word > 2 ? "Header" : (isMethods(_method.method) == 1 \
		? "Method" : "Version")) << std::endl;
		return (EXIT_FAILURE);
	}
	_method.requestedIsInRoot = IS_IN_ROOT(_method.requested[0]);
	return (EXIT_SUCCESS);
}

bool	Parsing::parsingHeader(const std::string& strRead)
{
	if (checkMethod(strRead))
		return (EXIT_FAILURE);
	size_t start = _findNewline, end = 0;
	while (true)
	{
		start += (_read[start] == '\n' ? 1 : 0);
		std::string tmpEnd = &_read[start]; 
		end = start + tmpEnd.find('\n');
		if ((_read[start]) == '\0' or tmpEnd.find('\n') == std::string::npos)
			break;
		std::string tmp(_read.begin() + start , _read.begin() + end);
		if (tmp[0] != '\0')
		{
			std::string key = ::getKey(tmp, ':'), value = ::getValue(tmp, ':');
			if (key == "ERROR" or value == "ERROR" or ::checkSpace(value, 2))
			{
				std::cout << "Error: format key or value" << std::endl;
				return (EXIT_FAILURE);
			}
			_method.content.insert(std::pair<std::string,std::string>(key, value));
		}
		start = end;
	}
	::printMap(_method.content);
	return (EXIT_SUCCESS);
}

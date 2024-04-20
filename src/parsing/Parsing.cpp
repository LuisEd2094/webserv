/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 09:22:27 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/20 17:17:20 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parsing.hpp"

/* hacer  primero la clase no instanciable despues inicializar los constructores
en privado y rellenar la clase */

Parsing::Parsing( void )
{
	_methods.push_back(std::string("GET"));
	_methods.push_back(std::string("POST"));
	_methods.push_back(std::string("DELETE"));
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
	// std::cout << _read << std::endl;
	return (_read);
}

bool Parsing::isSpace(const char c) const
{
	return (c == 32 or (c >= 9 and c <= 13));
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
	size_t	finNewline = 0;
	size_t	word = 0;
	size_t	space = 0;
	size_t	tmp = 0;

	finNewline = strRead.find('\n');
	for (size_t i = 0; i < finNewline; i++)
	{
		tmp = word;
		if (!isSpace(strRead[i]) and word != 3)
		{
			(word == 0 ? _method.method += strRead[i] : _method.method);
			(word == 1 ? _method.requested += strRead[i] : _method.method);
			(word == 2 ? _method.version += strRead[i] : _method.method);
		}
		else
		{
			word += (isSpace(strRead[i]) and (strRead[i + 1]
				and !isSpace(strRead[i + 1])) ? 1 : 0);
			space += (isSpace(strRead[i]) ? 1 : 0);
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

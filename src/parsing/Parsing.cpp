/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 09:22:27 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/19 10:24:51 by dacortes         ###   ########.fr       */
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

bool Parsing::isSpace( const char c) const
{
	return (c == 32 or (c >= 9 and c <= 13));
}

bool Parsing::isEmptyLine( const std::string& line ) const
{
	return (!line[0] or (line[0] == '\r' and line[1] == '\n')
		or line[0] == '\n');
}

bool Parsing::isMethods( const std::string& keyword) const
{
	return ((std::find(_methods.begin(), _methods.end(), keyword)
	== _methods.end()) ? EXIT_FAILURE : EXIT_SUCCESS);
}

size_t Parsing::countSpace(const std::string& line)
{
	size_t count = 0;
	size_t i = 0;

	std::cout << line << std::endl;
	while (isSpace(line[i]))
	{
		std::cout << "ssss" << std::endl;
		i++;
	}
	return (count);
}
#include <stdlib.h>
bool Parsing::checkMethod(const std::string& strRead)
{
	//suponemos que ya sabemos si tiene saltos de linea o retorno de carra y
	//salto de  lineas tambien verificamos los espacios si son mas de dos
	// isMethods(strRead);
	size_t	finNewline;
	size_t	word = 0;

	//comprobar si la poscion es un nulo al principio
	finNewline = strRead.find('\n');
	std::cout << finNewline << std::endl;
	for (size_t i = 0; i < finNewline; i++)
	{
		if (!isSpace(strRead[i]) && word != 3)
		{
			switch (word)
			{
				case 0:
					_method.method += strRead[i];
					break;
				case 1:
					_method.requested += strRead[i];
					break;
				case 2:
					_method.version += strRead[i];
					break;
				default:
					break;
			}
		}
		else
			word++;
	}
	if (word > 3)
	{
		std::cout << "Error: method" << std::endl;
		return (EXIT_FAILURE);
	}
	std::cout << "word: " << word << std::endl;
	std::cout << "method: " << _method.method << std::endl;
	std::cout << "requested: " << _method.requested << std::endl;
	std::cout << "version: " << _method.version << std::endl;
	// std::cout << "pos: " << &strRead[pos]  << "*" << std::endl;
	return (EXIT_SUCCESS);
}

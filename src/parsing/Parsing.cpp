/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 09:22:27 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/07 18:24:47 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parsing.hpp"

Parsing::Parsing( void )
{
	_methods[0] = "GET";
	_methods[1] = "POST";
	_methods[2] = "PUT";
	_methods[3] = "DELETE";
	_methods[4] = "PATCH";
	_methods[5] = "HEAD";
}

Parsing::Parsing( int fd )
{
	if (fd == -1)
		std::cout << "Error: open" << std::endl;
	int	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, _read, BUFFER_READ);
		if (bytes < 0)
			break ;
	}
	std::cout << _read << std::endl;
}

Parsing::~Parsing( void )
{}

/*
 * Membert Funtions
*/
bool Parsing::isEspace( const char c) const
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
	for (size_t i = 0; i < (LEN_METHODS - 1); i++)
	{
		if (keyword == (const std::string)_methods[i])
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

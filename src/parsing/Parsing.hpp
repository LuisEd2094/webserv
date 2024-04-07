/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 09:22:27 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/07 10:51:27 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parsing.hpp"

/*
 * Orthodox Canonical Form
*/
//private:
Parsing::Parsing( void ){}

Parsing::Parsing( const Parsing& obj)
{
	//igualar variables
}

Parsing &Parsing::operator=( const Parsing& obj)
{
	//igualar variables
	return (*this);
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
	for (size_t i = 0; i < LEN_METHODS; i++)
	{
		if (METHODS[i] == keyword)
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
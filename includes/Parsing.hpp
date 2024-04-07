/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:01:39 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/07 18:23:40 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

/******************************************************************************/
/*                            INCLUDES                                        */
/******************************************************************************/

# include <cstdlib>
# include <string>
# include <iostream>
# include <cerrno>
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>

/******************************************************************************/
/*                            MACROS                                          */
/******************************************************************************/

# define LEN_METHODS 7
# define BUFFER_READ 3000

/******************************************************************************/
/*                            CLASS                                           */
/******************************************************************************/

class Parsing
{
	private:
		std::string	_methods[LEN_METHODS];
		char		_read[BUFFER_READ];
	public:
		Parsing( void );
		Parsing( int fd );
		~Parsing( void );
	/*
	 * Get Methods
	*/
	/*
	 * Membert Funtions
	*/
	bool isEspace( const char c) const;
	bool isEmptyLine( const std::string& line ) const;
	bool isMethods( const std::string& keyword ) const;
	bool parsingHeader( char *strRead ) const;
	/*
	 * Exception Classes
	*/
};
#endif

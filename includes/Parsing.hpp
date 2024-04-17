/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:01:39 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/17 16:58:29 by dacortes         ###   ########.fr       */
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
# include <list>
# include <algorithm>

/******************************************************************************/
/*                            MACROS                                          */
/******************************************************************************/

# define LEN_METHODS 4
# define BUFFER_READ 20

/******************************************************************************/
/*                            CLASS                                           */
/******************************************************************************/

class Parsing
{
	private:
		//std::string	_methods[LEN_METHODS];
		std::list<std::string> _methods;
		std::string	_read;
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

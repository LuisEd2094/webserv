/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:01:39 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/07 10:48:14 by dacortes         ###   ########.fr       */
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

/******************************************************************************/
/*                            MACROS                                          */
/******************************************************************************/

# define METHODS {"GET", "POST", "PUT", "DELETE", "PATCH", "HEAD"}
# define LEN_METHODS 6

/******************************************************************************/
/*                            CLASS                                           */
/******************************************************************************/

class Parsing
{
	private:
		Parsing(void);
	public:
	/*
	 * Orthodox Canonical Form
	*/
		Parsing( const Parsing& obj );
		Parsing &operator=( const Parsing& obj );
		~Parsing( void );
	/*
	 * Get Methods
	*/
	/*
	 * Membert Funtions
	*/
	bool isEspace( const char c) const;
	bool isEmptyLine( const std::string& line ) const;
	bool isMethods( const std::string& keyword) const; 
	/*
	 * Exception Classes
	*/
};
#endif
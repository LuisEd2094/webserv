/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:01:39 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/23 15:58:12 by dacortes         ###   ########.fr       */
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
# include <dirent.h> //readdir
# include <map> //map

/******************************************************************************/
/*                            MACROS                                          */
/******************************************************************************/

# define BUFFER_READ 3000
# define IS_IN_ROOT(c) ((c) == '/')

/******************************************************************************/
/*                            CLASS                                           */
/******************************************************************************/

template<typename M>
void	printmap(const M& map)
{
	typename M::const_iterator i;
	for (i = map.begin(); i != map.end(); ++i)
		std::cout << "Key: " << i->first << "Value: " << i->second << std::endl;
}

//agregar atributo que indique si es de tipo RN o solo ON(only line)
typedef struct s_request
{
	std::string	method;
	std::string requested;
	std::string version;
	short		typeMethod;
	short		typeNewLine;
	bool		requestedIsInRoot;
	std::map<std::string, std::string> content;
} t_request;

class Parsing
{
	private:
		std::list<std::string> _methods;
		std::string	_read;
		t_request	_method;
		size_t		_findNewline;
	public:
		Parsing(void);
		~Parsing(void);
	/*
	 * Get Methods
	*/
	/*
	 * Membert Funtions
	*/
	//utils
	std::string readSocket(int fd);
	bool isSpace(const char c) const;
	bool isEmptyLine(const std::string& line) const;
	bool isMethods(const std::string& keyword) const;
	bool isVersion(const std::string& version) const;
	bool checkMethod(const std::string& strRead);
	bool parsingHeader(const std::string& strRead);
	/*
	 * Exception Classes
	*/
};
#endif

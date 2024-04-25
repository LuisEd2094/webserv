/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:01:39 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/25 12:49:08 by dacortes         ###   ########.fr       */
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
void	printMap(const M& map)
{
	typename M::const_iterator i;
	for (i = map.begin(); i != map.end(); ++i)
		std::cout << "Key: " << i->first << "\tValue: "
			<< i->second << std::endl;
}

template<typename K>
std::string	getKey(const K& str)
{
	if (!str[0] or str[0] == ':')
		return ("ERROR");
	size_t i;
	for (i = 0; str[i] and str[i] != ':'; i++)
	{
		if (str[i] == ' ')
			return ("ERROR");
	}
	return (std::string(str.begin(), (str.begin() + i)));
}

template<typename V>
std::string getValue(const V& str, char separator)
{
    size_t i = str.find(separator) + 1, j = str.size();
	size_t space_i = 0, space_j = 0;
    while (i < j and str[i] and ::isblank(str[i]))
	{
		space_i++;
		if (space_i > 2)
			return ("ERROR");
		i++;
	}
    while (j > i and str[j - 1] and ::isblank(str[j - 1]))
	{
		space_j++;
		if (space_j > 2)
			return ("ERROR");
		j--;
	}
	std::string res(str.begin() + i, str.begin() + j);
	if (!res[0])
		return ("ERROR");
    return (res);
}

template<typename S, typename N>
bool checkSpace(const S& str, const N num)
{
	int space = 0;
	for(size_t i = 0; i < str.size(); i++)
	{
		space += (::isblank(str[i]) ? 1 : 0);
		if (space > num)
			return (EXIT_FAILURE);
		space = (!::isblank(str[i])) ? 0 : space;
	}
	return (EXIT_SUCCESS);
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

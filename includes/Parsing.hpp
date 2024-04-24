/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:01:39 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/24 10:54:47 by dacortes         ###   ########.fr       */
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
	bool flag = false;
	size_t i;
	for (i = 0; str[i] and str[i] != ':'; i++)
	{
		if (str[i] == ' ')
			flag = true;
	}
	if (flag)
		return ("ERROR");
	return (std::string(str.begin(), (str.begin() + i)));
}

template<typename V>
std::string getValue(const V& str)
{
    size_t i = str.find(':') + 1, j = str.size();
	size_t space_i = 0, space_j = 0;
    while (i < j && str[i] == ' ')
	{
		space_i++;
		i++;
	}
    while (j > i && str[j - 1] == ' ')
	{
		space_j++;
		j--;
	}
    if (space_i > 2 or space_j > 2)
        return ("ERROR");
	std::string res(str.begin() + i, str.begin() + j);
	if (!res[0])
		return ("ERROR");
    return (res);
}

template<typename S, typename N, typename C>
bool checkSapce(const S& str, const N num, const C valid)
{
	int space = 0;
	for(size_t i = 0; i < str.size(); i++)
	{
		space += (str[i] == valid ? 1 : 0);
		if (space > num)
			return (EXIT_FAILURE);
		(str[i] != valid ? space = 0 : space);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:01:39 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/23 18:29:16 by dacortes         ###   ########.fr       */
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
	bool flag = false;
	size_t i;
	for (i = 0; str[i] and str[i] != ':'; i++)
	{
		if (str[i] == ' ')
			flag = true;
	}
	if (flag)
		return ("ERROR");
	std::string res(str.begin(), (str.begin() + i));
	return (res);
}

template<typename V>
std::string getValue(const V& str)
{
	size_t space_i = 0;
	size_t i = str.find(':') + 1;
	std::string line(str.begin() + i, str.end());
	std::cout << "line: " << line << std::endl;
	while ((line[i] and line[i] == ' '))
	{
		space_i++; 
		i++;
	}
	size_t j = i;
	size_t space_j = 0;
	while (line[j])
		j++;
	while(line[j] and line[j] == ' ')
	{
		space_j++; 
		j--;
	}
	if ((space_i > 2) or (space_j > 2))
		return ("ERROR");
	std::string res(line.begin() + i, line.begin() + j);
	std::cout << "res: " << res << std::endl;
	return (res);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 12:01:39 by dacortes          #+#    #+#             */
/*   Updated: 2024/06/15 10:27:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
#define PARSING_HPP

/******************************************************************************/
/*                            COLORS                                          */
/******************************************************************************/

# define END "\033[m"                 //end
# define RED "\033[1m\033[1;31m"      //red
# define GREEN "\033[1m\033[1;32m"      //green
# define YELLOW "\033[1m\033[1;33m"      //yellow
# define BLUE "\033[1;34m"             //blue
# define TUR "\033[1m\033[1;35m"      //Turquesa
# define CYAN "\033[1;36m"             //Cyan
# define ORANGE "\033[1m\033[38;5;208m"  //orange
# define PURPLE "\033[1m\033[38;5;128m"  //purple

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
# include <sstream>
# include <vector>
# include <Response.hpp>
# include <Uri.hpp>
# include <Path.hpp>
# include <ConfigElement.hpp>

/******************************************************************************/
/*                            MACROS                                          */
/******************************************************************************/

# define BUFFER_READ	1
# define ERROR			-1
# define WARNING 		1
# define ERROR_FORMAT	2 //si no es htttp numero punto numero HTTP 1.0 es valido 2.0
# define ERROR_HEADER	3
# define ERROR_METHOD	4
# define ERROR_VERSION	5
# define ERROR_STATUS "\033[1m\033[1;31m[ Error ]\033[m"
# define WARNING_StATUS "\033[1m\033[1;33m[ Warning ]\033[m"
# define VERSION_HTTP "HTTP/1.1"
# define IS_IN_ROOT(c) ((c) == '/')

/******************************************************************************/
/*                            CLASS                                           */
/******************************************************************************/

template<typename Container>
Container ft_split(const std::string& str, char delimiter)
{
    Container list;
    std::istringstream input(str);
    std::string word;
    
    while (std::getline(input, word, delimiter))
        list.push_back(word);
    return list;
}

template<typename L>
void	printList(const L& list)
{
	typename L::const_iterator i;
	(void)list;
 	for (i = list.begin(); i != list.end(); i++)
		std::cerr << ORANGE << "*" << END << *i 
		<< ORANGE << "*" << END << std::endl;
}

template<typename S>
std::string	cutSpace(const S& str)
{
	if (!str[0])
		return ("");
	typename S::const_iterator start = str.begin();
	typename S::const_iterator end = str.end();
	while (::isblank(*start) && start != end)
		start++;
	if (*end != '\0')
		end--;
	while (::isblank(*end) && start != start)
		end--;
	return (std::string(start, end));
}

template<typename M>
void	printMap(const M& map)
{
	printMap(map, 0);
}

template<typename M>
void	printMap(const M& map, int padding)
{
	typename M::const_iterator i;
	(void)map;
 	for (i = map.begin(); i != map.end(); ++i)
	{
		std::cerr << ConfigElement::genSpace(padding) ;
		std::cerr << ORANGE << "Key: " << END << i->first << ORANGE
			<< "\tValue: " << END << i->second << std::endl;
	}
}

template<typename LV>
std::string	containerToString(const LV& obj)
{
	typename LV::const_iterator i;
	std::string resul;
	bool	noBegin = false;
	for (i = obj.begin(); i != obj.end(); ++i)
	{
		if (noBegin)
			resul += "; ";
		else
			noBegin = true;
		resul += *i;
	}
	return (resul);
}

template<typename K>
std::string	getKey(const K& str, char separator)
{
	if (!str[0] or str[0] == separator)
		return ("ERROR");
	size_t i;
	for (i = 0; str[i] and str[i] != separator; i++)
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
template<typename S>
size_t wordCounter(const S& str)
{
	typename S::const_iterator i = str.begin();
	size_t		word = 0;
	word += (*i and !::isblank(*i) ? 1 : 0);
	for (; i != str.end(); ++i)
	{
		word += (::isblank(*i) and ((*(i + 1))
			and !::isblank((*(i + 1)))) ? 1 : 0);
	}
	return (word);
}

typedef struct s_request
{
	std::string	method;
	std::string requested;
	std::string version;
	bool		requestedIsInRoot;
	std::map<std::string, std::string> content;
	s_request() : method(""), requested(""), version("") {}
} t_request;

class Parsing
{
	private:
		std::list<std::string>		_methods;
		std::vector<std::string>	_errors;
		std::string	_read;
		std::string	_typeLine;
		t_request	_method;
		size_t		_findNewline;
		size_t		_begin;
		bool		_endRead;
		int			_statusError;

		std::string _not_found;
	public:
		Parsing(void);
		~Parsing(void);
	/*
	 * Membert Funtions
	*/
		//utils
		bool	isEmptyLine(const std::string& line) const;
		int		isMethods(const std::string& keyword) const;
		int		isVersion(const std::string& version) const;
		int		printStatus(const std::string& messages, short flag, int exitCode);
		int		checkMethod(const std::string& strRead);
		int 	parsingHeader(const std::string& strRead);
		int		normalizeError(int	error);
		void	resetParsing(void);
	/*
	 * Get Methods
	*/
		size_t								getPos(void);
		size_t								getEndSize(void);
		const std::string&					getVersion(void) const;
		const std::string& 					getMethod(void) const;
		const std::string& 					getRequested(void) const;
		const std::string&					getMapValue(const std::string& key) const;
		const std::string					getTypeLine(const std::string& strFind) const;
		std::map<std::string, std::string>	getMap(void);
		bool 								getEndRead() const;
};

#endif

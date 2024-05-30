/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 09:22:27 by dacortes          #+#    #+#             */
/*   Updated: 2024/05/30 10:56:20 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Parsing.hpp>
// # include "../../includes/Parsing.hpp"

Parsing::Parsing( void )
{
	_methods.push_back(std::string("GET"));
	_methods.push_back(std::string("POST"));
	_methods.push_back(std::string("DELETE"));
	_errors.push_back(" Format space.");
	_errors.push_back(" Format Key.");
	_errors.push_back(" Format Value.");
	_errors.push_back(" Invalid Header");
	_errors.push_back(" Invalid Method");
	_errors.push_back(" Invalid Version");
	_findNewline = 0;
	_endRead = false;
	_statusError = 0;
}

Parsing::~Parsing( void )
{}

/*
 * Membert Funtions
*/
std::string Parsing::readSocket(int fd)
{
	char buffRead[BUFFER_READ + 1];
	if (fd == -1)
		std::cout << "Error: open" << std::endl;
	int	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, buffRead, BUFFER_READ);
		if (bytes < 0)
			break ;
		buffRead[bytes] = '\0';
		_read += buffRead;
	}
	return (_read);
}

bool Parsing::isEmptyLine(const std::string& line) const
{
	return (!line[0] or (line[0] == '\r' and line[1] == '\n')
		or line[0] == '\n');
}

bool Parsing::isMethods(const std::string& keyword) const
{
	return ((std::find(_methods.begin(), _methods.end(), keyword)
	== _methods.end()) ? EXIT_FAILURE : EXIT_SUCCESS);
}

bool Parsing::isVersion(const std::string& version) const
{
	return (version != "HTTP/1.1");
}

int Parsing::checkMethod(const std::string& strIn)
{
	size_t	word = 0;
	size_t	space = 0;
	size_t	tmp = 0;

	// std::cout << TUR << "status error: " << END << _statusError << std::endl;
	if (_statusError < 0 or _statusError > 1)
		return (_statusError);
	_typeLine = getTypeLine(&strIn[_findNewline]);
	if (!_typeLine[0])
		return (_statusError = WARNING, this->printStatus(" End of line not found. ", WARNING, WARNING));
	_begin = _findNewline;
	size_t end = strIn.substr(_findNewline).find(_typeLine) + _findNewline;
	std::string strRead(strIn.begin() + _findNewline, strIn.begin() + end);

	for (size_t i = 0; end != std::string::npos && i < strRead.length(); i++)
	{
		tmp = word;
		if (!::isblank(strRead[i]) and word != 3)
		{
			(word == 0 ? _method.method += strRead[i] : _method.method);
			(word == 1 ? _method.requested += strRead[i] : _method.requested);
			(word == 2 ? _method.version += strRead[i] : _method.version);
		}
		else
		{
			word += (::isblank(strRead[i]) and (strRead[i + 1]
				and !::isblank(strRead[i + 1])) ? 1 : 0);
			space += (::isblank(strRead[i]) ? 1 : 0);
			if (space > 2)
				return (_statusError = this->printStatus(_errors[0], ERROR, ERROR_FORMAT));
			space = (tmp != word ? 0 : space);
		}
	}
	if (word > 2 or isMethods(_method.method) or isVersion(_method.version))
	{
		short error = (word > 2 ? ERROR_HEADER : (isMethods(_method.method) == 1) \
			? ERROR_METHOD : ERROR_VERSION);
		return (_statusError = this->printStatus(_errors[error], ERROR, error));
	}
	_method.requestedIsInRoot = IS_IN_ROOT(_method.requested[0]);
	_findNewline = end;

	return (_statusError = EXIT_SUCCESS);
}


const std::string& Parsing::getMethod(void)
{
	return (_method.method);
}

const std::string& Parsing::getRequested(void)
{
	return (_method.requested);
}

int	Parsing::printStatus(const std::string& messages, short flag, int exitCode)
{
	std::cout << (flag == -1 ? ERROR_STATUS : WARNING_StATUS)
	<< messages << std::endl;
	return (exitCode);
}

int	Parsing::parsingHeader(const std::string& strRead)
{
	size_t start = _findNewline, end = 0;

	if (_statusError < 0 or _statusError > 1)
		return (_statusError);
	while (true)
	{
		std::string typeLine = getTypeLine(&strRead[start]);
		if (!typeLine[0] or typeLine != _typeLine)
			return (_statusError = this->printStatus("End of line not found ", WARNING, WARNING));
		start += (typeLine[0] == '\r' ? 2 : 1);
		std::string tmpEnd = &strRead[start]; 
		std::size_t endPos = tmpEnd.find(typeLine);
		
		short emptyLine = 0;
		if (this->isEmptyLine(tmpEnd))
			emptyLine++;
		if ( endPos  == std::string::npos)
			return (_statusError = EXIT_FAILURE);
		end = start + endPos;
		std::string tmp(strRead.begin() + start , strRead.begin() + end);
		if (tmp[0] != '\0')
		{
			std::string key = ::getKey(tmp, ':'), value = ::getValue(tmp, ':');
			if (key == "ERROR" or value == "ERROR" or ::checkSpace(value, 2))
			{
				short error = (key == "ERROR") + ((value == "ERROR") * 2);
				return (_statusError = this->printStatus(_errors[error], ERROR, ERROR_FORMAT));
			}
			_method.content.insert(std::pair<std::string,std::string>(key, value));
		}
		start = end;
		_findNewline = start;
		if (emptyLine == 1)
		{
			_endRead = (emptyLine == 1);
			return (_statusError = EXIT_SUCCESS);
		}
	}
	return (_statusError = EXIT_SUCCESS);
}
std::string	Parsing::getTypeLine(const std::string& strFind)
{
	short	flag = 0;
	flag += (strFind.find("\r\n") != std::string::npos ? 2 : 0);
	flag += (strFind.find("\n") != std::string::npos ? 1 : 0);
	flag += (strFind.find("\n\r") != std::string::npos ? -1 : 0);
	flag += (strFind.find("\r \n") != std::string::npos ? -1 : 0);
	if (!flag)
		return ("");
	return (flag == 1 ? "\n" : "\r\n");
}

size_t	Parsing::getPos(void)
{
	return  (this->_findNewline);
}

std::map<std::string, std::string> Parsing::getMap(void)
{
	return (this->_method.content);
}

std::string Parsing::getMapValue(const std::string& key)
{
	std::map<std::string, std::string>::iterator it = this->_method.content.find(key);
	if (it == this->_method.content.end())
		return ("not found");
	return (it->second);
}

bool Parsing::getEndRead() const
{
	return (_endRead);
}

void Parsing::resetParsing( void)
{
	_method.content.clear();
	_method.method.clear();
	_method.requested.clear();
	_method.version.clear();
	_endRead = false;
	_findNewline += _typeLine.length();
}

std::list<std::string> ft_split(const std::string& str, char delimiter)
{
    std::list<std::string> list;
    std::istringstream input(str);
    std::string word;
    
    while (std::getline(input, word, delimiter))
        list.push_back(word);
    return (list);
}

// const t_request	*Parsing::getRequest(void) const 
// {
// 	return (&(this->_method));
// }

#include <Parsing.hpp>

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
	_not_found = PARSING_NOT_FOUND;
}

Parsing::~Parsing( void )
{}

/*
 * Membert Funtions
*/

bool Parsing::isEmptyLine(const std::string& line) const
{
	return (!line[0] or (line[0] == '\r' and line[1] == '\n')
		or line[0] == '\n');
}

int Parsing::isMethods(const std::string& keyword) const
{
	return ((std::find(_methods.begin(), _methods.end(), keyword)
	== _methods.end()) ? NOT_IMPLEMENTED : EXIT_SUCCESS);
}

int Parsing::isVersion(const std::string& version) const
{
	if (version.size() != 8)
		return (BAD_REQUEST);
	size_t i = 0;
	for (; VERSION_HTTP[i] == version[i] and version[i] != '/'; i++)
		;
	if ("HTTP" != std::string(version.begin(), version.begin() + i))
		return (BAD_REQUEST);
	size_t find = version.find("/");
	if (find == std::string::npos)
		return (BAD_REQUEST);
	find += 1;
	if (version[find] != '1')
		return (!std::isdigit(version[find]) ? BAD_REQUEST : VERSION_NOT_SUPPORTED);
	find += 1;
	if (version[find] != '.')
		return (BAD_REQUEST);
	find += 1;
	if (version[find] < '0' or version[find] > '1')
		return (!std::isdigit(version[find]) ? BAD_REQUEST : VERSION_NOT_SUPPORTED);
	return (EXIT_SUCCESS);
}

int Parsing::checkMethod(const std::string& strIn)
{
	size_t	word = 0;
	size_t	space = 0;
	size_t	tmp = 0;

	if (_statusError < 0 or _statusError > 1)
		return (_statusError);
	_typeLine = getTypeLine(&strIn[_findNewline]);
	if (!_typeLine[0])
		return (_statusError = this->printStatus(" End of line not found. ", WARNING, WARNING));
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
				return (_statusError = this->printStatus(_errors[0], ERROR, this->normalizeError(ERROR_FORMAT)));
			space = (tmp != word ? 0 : space);
		}
	}
	_method.requestedIsInRoot = IS_IN_ROOT(_method.requested[0]);
	_findNewline = end;
	if (word > 2 or isMethods(_method.method) or isVersion(_method.version))
	{
		short error = (word > 2 ? ERROR_HEADER : (isMethods(_method.method)) \
			? ERROR_METHOD : ERROR_VERSION);
		return (_statusError = this->printStatus(_errors[error], ERROR, this->normalizeError(error)));
	}


	return (_statusError = EXIT_SUCCESS);
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
			return (_statusError = WARNING);
		end = start + endPos;
		std::string tmp(strRead.begin() + start , strRead.begin() + end);
		if (tmp[0] != '\0')
		{
			std::string key = ::getKey(tmp, ':'), value = ::getValue(tmp, ':');
			if (key == "ERROR" or value == "ERROR" or ::checkSpace(value, 2))
			{
				short error = (key == "ERROR") + ((value == "ERROR") * 2);
				return (_statusError = this->printStatus(_errors[error], ERROR, this->normalizeError(ERROR_FORMAT)));
			}
			_method.content.insert(std::pair<std::string,std::string>(key, value));
		}
		start = end;
		_findNewline = start;
		if (emptyLine == 1)
		{
			_endRead = (emptyLine == 1);
			if (getMapValue("Host") == "not found")
			{
				return (_statusError = this->printStatus(_errors[3], ERROR, this->normalizeError(ERROR_FORMAT)));
			}
			std::string tmp = _method.content["Host"];
			_method.content["Host"] = ::getKey(tmp, ':');
			_method.content["Port"] = ::getValue(tmp, ':');
			_method.content["__Query__"] = Uri::Parse(_method.requested).QueryString.erase(0, 1);
			_method.content["__Path__"] = Uri::Parse(_method.requested).Path;
			if (this->getMethod() == "GET")
			{
				if (this->getMapValue("Content-Length") != _not_found
					or this->getMapValue("Transfer-Encoding") != _not_found)
					return (_statusError = this->printStatus(_errors[3], ERROR, this->normalizeError(ERROR_FORMAT)));
			}
			return (_statusError = EXIT_SUCCESS);
		}
	}
	return (_statusError = EXIT_SUCCESS);
}
const std::string	Parsing::getTypeLine(const std::string& strFind) const
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

const std::string& Parsing::getMapValue(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator it = this->_method.content.find(key);
	if (it == this->_method.content.end())
	{
		return (_not_found);
	}
	return (it->second);
}

size_t Parsing::getEndSize(void)
{
	return (this->_typeLine.length());
}

const std::string& Parsing::getVersion(void) const
{
	return (_method.version);
}

const std::string& Parsing::getMethod(void) const
{
	return (_method.method);
}

const std::string& Parsing::getRequested(void) const
{
	return (_method.requested);
}

bool Parsing::getEndRead() const
{
	return (_endRead);
}

int	Parsing::normalizeError(int error)
{
	if (error == ERROR_FORMAT or error == ERROR_HEADER)
		error = BAD_REQUEST;
	else if (error == ERROR_METHOD)
		error = NOT_IMPLEMENTED;
	else if (error == ERROR_VERSION)
		error = VERSION_NOT_SUPPORTED;
	return (error);
}

void Parsing::resetParsing( void)
{
	_method.content.clear();
	_method.method.clear();
	_method.requested.clear();
	_method.version.clear();
	_endRead = false;
	_findNewline = 0;
}

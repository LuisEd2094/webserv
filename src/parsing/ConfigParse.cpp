#include "../../includes/ConfigParse.hpp"
# include <algorithm>
# include <cctype>

# include <iostream>

std::list<char> ConfigParse::endStatementChars = std::list<char>();

std::string dumbGenString(std::string::iterator begin)
{
	std::string result("");

	while(*begin != '\0')
	{
		result += *begin;
		begin++;
	}
	return result;
}

std::string dumbGenString(std::string::iterator begin, std::string::iterator end)
{
	std::string result("");

	while(begin != end)
	{
		result += *begin;
		begin++;
	}
	return result;
}

bool ConfigParse::isEndOfStatement(char target)
{
	std::list<char>::iterator firstEndStatement = ConfigParse::endStatementChars.begin();
	std::list<char>::iterator lastEndStatement = ConfigParse::endStatementChars.end();
	return std::find(firstEndStatement,lastEndStatement, target) != lastEndStatement;

}

std::string::iterator ConfigParse::findEndOfStatement()
{
	std::string::iterator end;
	end = this->statementBegin;
	end++;
	while (end != this->eof && !ConfigParse::isEndOfStatement(*end))
	{
		end++;
	}
	return (end); 
}

void ConfigParse::newNestedElement(void)
{
	std::string::iterator beginParam = statementBegin;
	std::string::iterator endParam;
	std::string param1;
	std::string param2;

	std::cout << " ::: NEW nested element :::" << std::endl;
	
	while (beginParam != this->eof && std::isspace(*beginParam) && beginParam != this->statementEnd)
	{
		beginParam++;
	}
	endParam = beginParam;
	while (endParam != this->eof && !std::isspace(*endParam) && endParam != this->statementEnd)
	{
		endParam++;
	}
	param1 = std::string(beginParam, endParam);

	beginParam = endParam;
	while (beginParam != this->eof && std::isspace(*beginParam) && beginParam != this->statementEnd)
		beginParam++;
	endParam = beginParam;
	while (endParam != this->eof && !std::isspace(*endParam) && endParam != this->statementEnd)
	{
	std::cout << "*endParam" << *endParam << "****" << std::endl;
		endParam++;
	}
	param2 = std::string(beginParam, endParam);

	std::cout << "para1: >>"<< param1 << "<<" << std::endl;
	std::cout << "para2: >>"<< param2 << "<<" << std::endl;

	std::cout << (this->statementEnd == endParam) << std::endl;
	std::cout << "*this->statementEnd" << *this->statementEnd << std::endl;
	std::cout << "*endParam" << *endParam << "****" << std::endl;
	if (param1 == "server")
	{
		std::cout << "new server" << std::endl;
	}
	else if (param1 == "location")
	{
		std::cout << "new location" << std::endl;
	}
	else if  (param1 == "cgi")
	{
		std::cout << "new cgi" << std::endl;
	}
	else
	{
		std::cout << "user is stupid: " << param1 << std::endl;
	}
}

void ConfigParse::parse()
{
	std::string				statementStr;
	this->statementBegin = this->fileContent.begin();
	while(this->statementBegin != this->eof)
	{
		this->statementEnd = this->findEndOfStatement();
		if (statementEnd == this->eof)
			return ;

		std::cout << "STATEMENT: >>" << dumbGenString(this->statementBegin, this->statementEnd) << "<< " << std::endl;
		
		if (*statementBegin == '{')
		{
			std::cout << "user is stupid" << std::endl;
			return ;
		}
		if (*statementBegin == '}')
		{
			this->statementBegin++;
			std::cout << "- - - - - - - - - - - - - - - - " << std::endl;
			continue; //TODO this will be a return ;
		}
		else if (*statementEnd == '}')
		{
			this->statementBegin = statementEnd;
			this->statementBegin++;
			std::cout << "- - - - - - - - - - - - - - - - " << std::endl;
			continue; //TODO this will be a return ;
		}
		 else if (this->statementEnd == this->eof || *this->statementEnd == ';')
		{
			statementStr = std::string(this->statementBegin, this->statementEnd);
		}
		else if (*this->statementEnd == '{')
		{
			this->newNestedElement();
		}
		this->statementBegin = this->statementEnd;
		if (statementBegin != this->eof)
			this->statementBegin++;
		std::cout << "- - - - - - - - - - - - - - - - " << std::endl;
	}
}

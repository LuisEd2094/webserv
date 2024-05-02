#include "../../includes/ConfigParse.hpp"
# include <algorithm>
# include <cctype>

# include <iostream>

std::list<char> ConfigParse::endStatementChars = std::list<char>(';', '{');

std::string::iterator ConfigParse::endOfStatement()
{
	std::string::iterator end;
	end = this->statementBegin;
	std::list<char>::iterator firstEndStatement = ConfigParse::endStatementChars.begin();
	std::list<char>::iterator lastEndStatement = ConfigParse::endStatementChars.end();
	while (end != this->eof && std::find(firstEndStatement,lastEndStatement, *end) == lastEndStatement)
	{
		std::cout << *end << " " << std::endl;
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
	std::cout << "sdaf" << std::endl;
	
	std::cout << "value eof: " << (*this->eof == 0) << "*" << std::endl;
	while (*endParam != *this->eof && std::isspace(*beginParam))
		beginParam++;
	endParam = beginParam;
	std::cout << "fasad " << *endParam << std::endl;
	while (*endParam != '\0' && !std::isspace(*endParam))
	{
		std::cout << "toto " << *endParam << std::endl;
		//exit(1);
		endParam++;
	}
	param1 = std::string(beginParam, endParam);
	
	std::cout << "kokou" << std::endl;
	beginParam = endParam;
	while (*endParam != *this->eof && std::isspace(*beginParam))
		beginParam++;
	endParam = beginParam;
	std::cout << "yok" << std::endl;
	while (*endParam != *this->eof && !std::isspace(*endParam))
		endParam++;
	param2 = std::string(beginParam, endParam);

	std::cout << "para1: >>"<< param1 << "<<" << std::endl;
	std::cout << "para2: >>"<< param2 << "<<" << std::endl;

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
		std::cout << "user is stupid" << std::endl;
	}
}

void ConfigParse::parse()
{
	std::string				statementStr;
	this->statementBegin = this->fileContent.begin();
	while(this->statementBegin != this->eof)
	{
		this->statementEnd = this->endOfStatement();
		std::cout << *this->statementEnd  << std::endl;
		if (this->statementEnd == this->eof || *this->statementEnd == ';')
		{
			statementStr = std::string(this->statementBegin, this->statementEnd);
		}
		else if (*this->statementEnd == '{')
		{
			std::cout << *this->statementEnd  << std::endl;
			this->newNestedElement();
		}
		this->statementBegin = this->statementEnd;
	}
	
}

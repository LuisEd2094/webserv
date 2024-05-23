#include "../../includes/ParsingElement.hpp"
#include "../../includes/Parsing.hpp"
# include <algorithm>
# include <cctype>

# include <iostream>

std::list<char> ParsingElement::endStatementChars = std::list<char>();

//temporal:
short parsingListen(std::string &listen);
std::string getIp(std::string &listen);
std::string getPort(std::string &listen);

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

bool ParsingElement::isEndOfStatement(char target)
{
	std::list<char>::iterator firstEndStatement = ParsingElement::endStatementChars.begin();
	std::list<char>::iterator lastEndStatement = ParsingElement::endStatementChars.end();
	return std::find(firstEndStatement,lastEndStatement, target) != lastEndStatement;

}

/*
std::string::iterator ParsingElement::findEndOfStatement()
{
	std::string::iterator end;
	end = this->statementBegin;
	end++;
	while (end != this->eof && !ParsingElement::isEndOfStatement(*end))
	{
		end++;
	}
	std::cout << "exiting" << std::endl;
	return (end); 
}
*/

std::string::iterator ParsingElement::findEndOfStatement()
{
	while (this->statementEnd != this->eof && !ParsingElement::isEndOfStatement(*this->statementEnd))
	{
		this->statementEnd++;
	}
	return (this->statementEnd); //TODO no no need for retrun; 
}

void ParsingElement::newNestedElement(void)
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
		endParam++;
	}
	param2 = std::string(beginParam, endParam);

	if (statementEnd != this->eof)
		this->statementEnd++;
	while ( this->statementBegin != this->statementEnd)
		this->statementBegin++;
	
	std::cout << "- - - - - - - - - - - - - - - - " << std::endl;
	this->createNestedElement(param1, param2);
	/*
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
		throw std::exception();
	}
	*/
}

void ParsingElement::parse()
{
	std::string				statementStr;
	//this->statementBegin = this->fileContent.begin();
	std::cout << "FULL TEXT: >>" << dumbGenString(this->statementBegin) << "<< " << std::endl;
	while(this->statementBegin != this->eof)
	{
	//	std::cout << "statementBegin ; statementEnd " << *this->statementBegin << " " <<  *this->statementEnd << std::endl;

		//this->statementEnd = this->findEndOfStatement();
		this->statementEnd = this->findEndOfStatement();
		// std::cout << "statementBegin ; statementEnd " << *this->statementBegin << " ; " <<  *this->statementEnd << std::endl;
		if (statementEnd == this->eof)
			return ;

		// std::cout << "STATEMENT: >>" << dumbGenString(this->statementBegin, this->statementEnd) << "<< " << std::endl;
		// std::cout << "*this->statementBegin: >>" << *this->statementBegin << std::endl;
		// std::cout << "statementBegin = statementEnd" << (this->statementBegin == this->statementEnd) << std::endl;
		
		if (*statementBegin == '{')
		{
			std::cout << "user is stupidd" << std::endl;
			return ;
		}
		if (*statementBegin == '}')
		{
			this->statementBegin++;
			std::cout << "- - - - - - - - - - - - - - - - " << std::endl;
			std::cout << "Exiting parse" << std::endl;
			printMap(*this);
			return;
		}
		else if (*statementEnd == '}')
		{
			this->statementBegin = statementEnd;
			this->statementBegin++;
			std::cout << "- - - - - - - - - - - - - - - - " << std::endl;
			std::cout << "Exiting parse" << std::endl;
			printMap(*this);
			return;
		}
		else if (this->statementEnd == this->eof || *this->statementEnd == ';')
		{
			while(std::isspace(*this->statementBegin))
				this->statementBegin++;
			statementStr = std::string(this->statementBegin, this->statementEnd);
			std::string key = getKey(statementStr, ':'), value = getValue(statementStr, ':');
			// std::map<std::string, std::string>::insert(std::pair<std::string,std::string>(key, value));
			std::cout << BLUE << "type ip: " << parsingListen(value) << END << std::endl;
			std::cout << YELLOW << getIp(value) << END << std::endl;
			std::cout << TUR << getPort(value) << END << std::endl;
			std::pair<iterator,bool> insertReturn;
			insertReturn = std::map<std::string, std::string>::insert(std::pair<std::string,std::string>(key, value));
			// std::cout << "first: " << (insertReturn.first == end()) << ", second: " << insertReturn.second <<std::endl;
			if (!insertReturn.second)
			{
				std::cout << "user is stupid repited key parameter" << std::endl;
				throw std::exception();
			}
			std::cout << "----------------------*********" << std::endl;
			std::cout << this << std::endl;
		}
		else if (*this->statementEnd == '{')
		{
			this->newNestedElement();
		}
		if (statementEnd != this->eof)
			this->statementEnd++;
		while ( this->statementBegin != this->statementEnd)
			this->statementBegin++;
		std::cout << "- - - - - - - - - - - - - - - - " << std::endl;
	}
	std::cout << "Exiting parse" << std::endl;
	printMap(*this);
}

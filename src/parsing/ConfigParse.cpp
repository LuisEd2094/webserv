#include "../../includes/ConfigParse.hpp"
# include <algorithm>
# include <cctype>

# include <iostream>

std::list<char> ConfigParse::endStatementChars = std::list<char>({';', '{', '}'});

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
	//std::cout  << "fkkkkk " << (*this->eof == *end) << std::endl;
	//std::cout << "find end remainder: " <<  dumbGenString(end) << std::endl;
	//std::cout << (*end  == '\0') << " " << (end  == this->eof) << "end is end" << std::endl;
	end++;
	//std::cout << !ConfigParse::isEndOfStatement(*end) << std::endl;
	while (end != this->eof && !ConfigParse::isEndOfStatement(*end))
	{
		//std::cout << *end << " endd "  << *this->eof << std::endl;
		//std::cout << (*end  == '\0') << " " << (end  == this->eof) << "end is end" << std::endl;
		//std::cout << "find end remainder: " <<  dumbGenString(end) << std::endl;
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
	
	//while (beginParam != this->eof && std::isspace(*beginParam) && !ConfigParse::isEndOfStatement(*beginParam))
	while (beginParam != this->eof && std::isspace(*beginParam) && beginParam != this->endParam)
	{
		//std::cout << "beginParam " << *beginParam << std::endl;
		beginParam++;
	}
	//std::cout << "beginParam " << *beginParam << std::endl;
	endParam = beginParam;
	//while (endParam != this->eof && !std::isspace(*endParam) && !ConfigParse::isEndOfStatement(*endParam))
	while (endParam != this->eof && !std::isspace(*endParam) && beginParam != this->endParam)
	{
		//std::cout << "endParam " << *endParam << std::endl;
		endParam++;
	}
	//std::cout << "endParam " << *endParam << std::endl;
	param1 = std::string(beginParam, endParam);
	
	beginParam = endParam;
	//while (beginParam != this->eof && std::isspace(*beginParam) && !ConfigParse::isEndOfStatement(*beginParam))
	while (beginParam != this->eof && std::isspace(*beginParam) && beginParam != this->endParabeginParam != this->endParam
		beginParam++;
	endParam = beginParam;
	//while (endParam != this->eof && !std::isspace(*endParam) && !ConfigParse::isEndOfStatement(*endParam))
	while (endParam != this->eof && !std::isspace(*endParam) && beginParam != this->statementEnd)
		endParam++;
	param2 = std::string(beginParam, endParam);

	//std::cout << "para1: >>"<< param1 << "<<" << std::endl;
	//std::cout << "para2: >>"<< param2 << "<<" << std::endl;

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
	//std::cout << "content: " << this->fileContent << std::endl;
	while(this->statementBegin != this->eof)
	{
		//if (statementBegin == this->eof)
		//	return ;
		//std::cout << "   REMAINDER: " << dumbGenString(this->statementBegin) << std::endl;
		this->statementEnd = this->findEndOfStatement();
		if (statementEnd == this->eof)
			return ;
		//std::cout << "   this->statementBegin: " << *this->statementBegin << std::endl;
		//std::cout << "   this->statementEnd: " << *this->statementEnd << std::endl;
		//std::cout << "   remainder: " << dumbGenString(this->statementBegin) << std::endl;
		//std::cout << std::endl;
		if (*statementBegin == '}')
		{
			this->statementBegin++;
			//std::cout << "--- --- --- --- --- " << std::endl;
			continue; //TODO this will be a return ;
		}
		if (*statementEnd == '}')
		{
			this->statementBegin = statementEnd;
			this->statementBegin++;
			//std::cout << "--- --- --- --- --- " << std::endl;
			continue; //TODO this will be a return ;
		}
		//std::cout << "statement end" << *this->statementEnd  << std::endl;
		if (this->statementEnd == this->eof || *this->statementEnd == ';')
		{
			//std::cout << "not nested: " << *this->statementEnd  << std::endl;
			statementStr = std::string(this->statementBegin, this->statementEnd);
		}
		else if (*this->statementEnd == '{')
		{
			//std::cout << "nested: " << *this->statementEnd  << std::endl;
			this->newNestedElement();
		}
		this->statementBegin = this->statementEnd;
		//std::cout << "- - - - - - - - - - " << std::endl;
		if (statementBegin != this->eof)
			this->statementBegin++;
	}
}

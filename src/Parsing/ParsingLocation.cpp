#include "../../includes/ParsingLocation.hpp"
#include "../../includes/ParsingCgi.hpp"
#include "../../includes/Parsing.hpp"

ParsingLocation::ParsingLocation(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ParsingElement(begin, eof, statementEnd)
{
	std::cout << "::::: ParsingLocation constructor called :::::" << std::endl;
}

void	ParsingLocation::createNestedElement(std::string param0, std::string param1)
{
	std::cout << "location nested: " << param0 << "  " << param1 << std::endl;
	if (param0 == "location")
	{
		//put second parameter
		ParsingLocation element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		element.insert(std::make_pair("__elemType__", param0));
		element.insert(std::make_pair("__elemArgument__", param1));
		this->locations.push_back(element);
	}
	else if (param0 == "cgi")
	{
		ParsingCgi element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		element.insert(std::make_pair("__elemType__", param0));
		element.insert(std::make_pair("__elemArgument__", param1));
		this->cgis.push_back(element);
	}
	else
	{
		std::cout << "user is stupid" << std::endl;
		throw std::exception();
	}
}

void	ParsingLocation::recursivePrint(void)
{
	std::cout << YELLOW << "-> Locations" << END << std::endl;
	::printMap(*this);
	std::list<ParsingLocation>::iterator it;
	for (it = this->locations.begin(); it != this->locations.end(); ++it)
	{
		it->recursivePrint();
	}
	std::list<ParsingCgi>::iterator i;
	for (i = this->cgis.begin(); i != this->cgis.end(); ++i)
	{
		i->recursivePrint();
	}
	std::cout << "-----------------------------------------" << std::endl;
}

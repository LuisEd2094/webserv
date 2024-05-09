#include "../../includes/ConfigLocation.hpp"
#include "../../includes/ConfigCgi.hpp"
#include "../../includes/Parsing.hpp"

ConfigLocation::ConfigLocation(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ConfigParse(begin, eof, statementEnd)
{
	std::cout << "::::: ConfigLocation constructor called :::::" << std::endl;
}

void	ConfigLocation::createNestedElement(std::string param0, std::string param1)
{
	std::cout << "location nested: " << param0 << "  " << param1 << std::endl;
	if (param0 == "location")
	{
		ConfigLocation element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		this->locations.push_back(element);
	}
	else if (param0 == "cgi")
	{
		ConfigCgi element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		this->cgis.push_back(element);
	}
	else
	{
		std::cout << "user is stupid" << std::endl;
		throw std::exception();
	}
}

void	ConfigLocation::recursivePrint(void)
{
	std::cout << YELLOW << "-> Locations" << END << std::endl;
	::printMap(*this);
	std::list<ConfigLocation>::iterator it;
	for (it = this->locations.begin(); it != this->locations.end(); ++it)
	{
		it->recursivePrint();
	}
	std::list<ConfigCgi>::iterator i;
	for (i = this->cgis.begin(); i != this->cgis.end(); ++i)
	{
		i->recursivePrint();
	}
}

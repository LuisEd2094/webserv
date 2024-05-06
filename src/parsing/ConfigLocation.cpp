#include "../../includes/ConfigLocation.hpp"
#include "../../includes/ConfigCgi.hpp"

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
	}
	else if (param0 == "cgi")
	{
		ConfigCgi element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
	}
	else
	{
		std::cout << "user is stupid" << std::endl;
		throw std::exception();
	}
}

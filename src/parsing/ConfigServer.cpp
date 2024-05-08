#include "../../includes/ConfigServer.hpp"
#include "../../includes/ConfigLocation.hpp"

ConfigServer::ConfigServer(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ConfigParse(begin, eof, statementEnd)
{
	std::cout << "::::: ConfigServer constructor called :::::" << std::endl;
}

void	ConfigServer::createNestedElement(std::string param0, std::string param1)
{
	std::cout << "server nested: " << param0 << "  " << param1 << std::endl;
	if (param0 == "location")
	{
		ConfigLocation element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		std::cout << "user is not  stupid" << std::endl;
	}
	else
	{
		std::cout << "user is stupid" << std::endl;
		throw std::exception();
	}
}

#include "../../includes/ConfigCgi.hpp"

ConfigCgi::ConfigCgi(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ConfigParse(begin, eof, statementEnd)
{
	std::cout << "::::: ConfigCgi constructor called :::::" << std::endl;
}

void	ConfigCgi::createNestedElement(std::string param0, std::string param1)
{
		std::cout << "cgi nested: " << param0 << "  " << param1 << std::endl;
		//throw std::exception();
	
}

#include "../../includes/ConfigGlobal.hpp"
#include "../../includes/Parsing.hpp"

ConfigGlobal::ConfigGlobal(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ConfigParse(begin, eof, statementEnd)
{
	std::cout << "::::: ConfigGlobal constructor called :::::" << ">" << std::endl;
}

void	ConfigGlobal::createNestedElement(std::string param0, std::string param1)
{
	std::cout << "global nested: " << param0 << "  " << param1 << std::endl;
	if (param0 == "server")
	{
		ConfigServer element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		this->servers.push_back(element);
	}
	else
	{
		std::cout << "user is stupid" << std::endl;
		throw std::exception();
	}
}

void	ConfigGlobal::recursivePrint(void)
{
	std::cout << TUR << "Call ConfigGlobal" << END << std::endl;
	std::list<ConfigServer>::iterator it;
	for (it = this->servers.begin(); it != this->servers.end(); ++it)
		it->recursivePrint();
}

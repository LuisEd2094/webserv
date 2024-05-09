#include "../../includes/ConfigServer.hpp"
#include "../../includes/ConfigLocation.hpp"
#include "../../includes/Parsing.hpp"

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
		try
		{
			ConfigLocation element(this->statementBegin, this->eof, this->statementEnd);
			element.parse();
			this->locations.push_back(element);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}	
		std::cout << "user is not  stupid" << std::endl;
	}
	else
	{
		std::cout << "user is stupid" << std::endl;
		throw std::exception();
	}
}

void	ConfigServer::recursivePrint(void)
{
	std::cout << BLUE << "---------- SERVER ----------" << std::endl;
	::printMap(*this);
	std::list<ConfigLocation>::iterator it;
	for (it = this->locations.begin(); it != this->locations.end(); ++it)
	{
		it->recursivePrint();
	}
}

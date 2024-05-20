#include "../../includes/ParsingServer.hpp"
#include "../../includes/ParsingLocation.hpp"
#include "../../includes/Parsing.hpp"

ParsingServer::ParsingServer(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ParsingElement(begin, eof, statementEnd)
{
	std::cout << "::::: ParsingServer constructor called :::::" << std::endl;
}

void	ParsingServer::createNestedElement(std::string param0, std::string param1)
{
	std::cout << "server nested: " << param0 << "  " << param1 << std::endl;
	if (param0 == "location")
	{
		try
		{
			ParsingLocation element(this->statementBegin, this->eof, this->statementEnd);
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

void	ParsingServer::recursivePrint(void)
{
	std::cout << BLUE << "---------- SERVER ----------" << std::endl;
	::printMap(*this);
	std::list<ParsingLocation>::iterator it;
	for (it = this->locations.begin(); it != this->locations.end(); ++it)
	{
		it->recursivePrint();
	}
}

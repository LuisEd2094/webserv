#include "../../includes/ParsingServer.hpp"
#include "../../includes/ParsingLocation.hpp"
#include "../../includes/Parsing.hpp"

ParsingServer::ParsingServer(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ParsingElement(begin, eof, statementEnd)
{
}

void	ParsingServer::createNestedElement(std::string param0, std::string param1)
{
	if (param0 == "location")
	{
		ParsingLocation element(this->statementBegin, this->eof, this->statementEnd);
		element.parse();
		element.insert(std::make_pair("__elemType__", param0));
		element.insert(std::make_pair("__elemArgument__", param1));
		this->locations.push_back(element);
	}
	else
	{
		std::cout << RED << "Invalid configuration element" << param0 << END << std::endl;
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

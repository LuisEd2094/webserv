#include "../../includes/ParsingGlobal.hpp"
#include "../../includes/Parsing.hpp"

ParsingGlobal::ParsingGlobal(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ParsingElement(begin, eof, statementEnd)
{
	std::cout << "::::: ParsingGlobal constructor called :::::" << ">" << std::endl;
}

void	ParsingGlobal::createNestedElement(std::string param0, std::string param1)
{
	std::cout << "global nested: " << param0 << "  " << param1 << std::endl;
	if (param0 == "server")
	{
		ParsingServer element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		this->servers.push_back(element);
	}
	else
	{
		std::cout << "user is stupid" << std::endl;
		throw std::exception();
	}
}

void	ParsingGlobal::recursivePrint(void)
{
	std::cout << TUR << "Call ParsingGlobal" << END << std::endl;
	std::list<ParsingServer>::iterator it;
	for (it = this->servers.begin(); it != this->servers.end(); ++it)
		it->recursivePrint();
}

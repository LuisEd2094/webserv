#include "../../includes/ParsingCgi.hpp"
#include "../../includes/Parsing.hpp"

ParsingCgi::ParsingCgi(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ParsingElement(begin, eof, statementEnd)
{
	std::cout << "::::: ParsingCgi constructor called :::::" << std::endl;
}

void	ParsingCgi::createNestedElement(std::string param0, std::string param1)
{
		std::cout << "cgi nested: " << param0 << "  " << param1 << std::endl;
		//throw std::exception();
	
}

void	ParsingCgi::recursivePrint(void)
{
	std::cout << YELLOW << "-> Cgi" << END << std::endl;
	::printMap(*this);
}

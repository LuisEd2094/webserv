#include "../../includes/ParsingCgi.hpp"
#include "../../includes/Parsing.hpp"

ParsingCgi::ParsingCgi(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ParsingElement(begin, eof, statementEnd)
{
}

void	ParsingCgi::createNestedElement(std::string param0, std::string param1)
{
		(void)param0;
		(void)param1;
}

void	ParsingCgi::recursivePrint(void)
{
	std::cout << YELLOW << "-> Cgi" << END << std::endl;
	::printMap(*this);
}

#ifndef PARSING_SERVER_HPP
# define PARSING_SERVER_HPP
# include "ParsingElement.hpp"
# include "ParsingLocation.hpp"


class ParsingServer: public ParsingElement
{
	public:
		void	createNestedElement(std::string param0, std::string param1);
		ParsingServer(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		~ParsingServer(){};
		void	recursivePrint(void);
		std::list<ParsingLocation>	locations;
};
#endif

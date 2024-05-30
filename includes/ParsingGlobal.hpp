#ifndef PARSING_GLOBAL_HPP
# define PARSING_GLOBAL_HPP
# include "ParsingElement.hpp"
# include "ParsingServer.hpp"

class ParsingGlobal: public ParsingElement
{
	public:
		static ParsingGlobal parseFromFile(std::string file);
		void	createNestedElement(std::string param0, std::string param1);
		ParsingGlobal(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		~ParsingGlobal(){};
		void	recursivePrint(void);
		std::list<ParsingServer>	servers;
};
#endif

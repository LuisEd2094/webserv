#ifndef CONFIG_GLOBAL_HPP
# define CONFIG_GLOBAL_HPP
# include "ParsingElement.hpp"
# include "ParsingServer.hpp"

class ParsingGlobal: public ParsingElement
{
	public:
		void	createNestedElement(std::string param0, std::string param1);
		ParsingGlobal(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		~ParsingGlobal(){};
		void	recursivePrint(void);
		std::list<ParsingServer>	servers;
};
#endif

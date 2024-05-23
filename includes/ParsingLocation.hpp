#ifndef CONFIG_LOCATION_HPP
# define CONFIG_LOCATION_HPP
# include "ParsingElement.hpp"
# include "ParsingCgi.hpp"

class ParsingLocation: public ParsingElement
{
	public:
		void	createNestedElement(std::string param0, std::string param1);
		ParsingLocation(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		~ParsingLocation(){};
		void	recursivePrint(void);
		//parsing
		//check key value
		//check valid nested
		//initObj
	private:
		std::list<ParsingLocation>	locations;
		std::list<ParsingCgi>		cgis;


};
#endif

#ifndef PARSING_LOCATION_HPP
# define PARSING_LOCATION_HPP
# include "ParsingElement.hpp"
# include "ParsingCgi.hpp"

class ParsingLocation: public ParsingElement
{
	public:
		void	createNestedElement(std::string param0, std::string param1);
		ParsingLocation(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		~ParsingLocation(){};
		void	recursivePrint(void);
		std::list<ParsingLocation>	getLocations() {return this->locations;};
		std::list<ParsingCgi>	getCgis() {return this->cgis;};
		//parsing
		//check key value
		//check valid nested
		//initObj

	private:
		std::list<ParsingLocation>	locations;
		std::list<ParsingCgi>		cgis;
	//ADD  value protedtec


};
#endif

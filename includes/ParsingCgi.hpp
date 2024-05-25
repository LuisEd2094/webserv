#ifndef PARSING_CGI_HPP
# define PARSING_CGI_HPP
# include "ParsingElement.hpp"
class ParsingCgi: public ParsingElement
{
	public:
		void	createNestedElement(std::string param0, std::string param1);
		ParsingCgi(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		~ParsingCgi(){};
		void recursivePrint(void);
		//parsing
		//check key value
		//check valid nested
		//initObj
	private:

};
#endif

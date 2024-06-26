#ifndef PARSING_PARSE_HPP
# define PARSING_PARSE_HPP
# include <map>
# include <list>
# include <string>
# include <exception>
# include <iostream>

class ParsingElement : public std::map<std::string, std::string>
{
	public: 
		ParsingElement(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd) :
			std::map<std::string, std::string>::map(),
			statementBegin(begin), statementEnd(statementEnd), eof(eof)
		{
				this->endStatementChars.push_back(';');
				this->endStatementChars.push_back('{');
				this->endStatementChars.push_back('}');
		};
		virtual ~ParsingElement(){};
		virtual void	recursivePrint(void) = 0;

		void parse();
		std::string::iterator		findEndOfStatement();
		std::string::iterator		endOfStatement();
		static bool					isEndOfStatement(char target);
		char						nextChar(void);
		void						newNestedElement(void);
		virtual void				createNestedElement(std::string param0, std::string param1) = 0;


		class ParsingElementException : public std::exception
		{
			const char* what() const throw()
			{
				return("Error trying to acces out of files boundaries");
			}
		};

	protected:
		std::string::iterator	&statementBegin;
		std::string::iterator	&statementEnd;
		std::string::iterator	&eof;

	private:
		static std::list<char> endStatementChars;
		const static std::string			empty;
		static std::string::iterator		ebgn;
		static std::string::iterator		eend;
		ParsingElement() : statementBegin(ebgn), statementEnd(ebgn), eof(eend){};
};
	const std::string	empty = "";
#endif

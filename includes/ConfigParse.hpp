#ifndef CONFIG_PARSE_HPP
# define CONFIG_PARSE_HPP
# include <map>
# include <list>
# include <string>
# include <exception>
# include <iostream>

class ConfigParse : std::map<std::string, std::string>
{
	public: 
		ConfigParse(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd) :
			std::map<std::string, std::string>::map(),
			statementBegin(begin), statementEnd(statementEnd), eof(eof)
		{
//				this->fileContent = std::string(fileContent);	
//				this->statementBegin = begin;
//				this->eof =  end;
				this->endStatementChars.push_back(';');
				this->endStatementChars.push_back('{');
				this->endStatementChars.push_back('}');
		};
		virtual ~ConfigParse(){};

//		std::string _getFileContent() const { return fileContent; };

		void parse();
		std::string::iterator		findEndOfStatement();
		std::string::iterator		endOfStatement();
		static bool					isEndOfStatement(char target);
		char						nextChar(void);
		void						newNestedElement(void);
		virtual void				createNestedElement(std::string param0, std::string param1) = 0;


		class ConfigParseException : public std::exception
		{
			const char* what() const throw()
			{
				return("Error trying to acces out of files boundaries");
			}
		};

	protected:
//		std::string				fileContent;
		std::string::iterator	&statementBegin;
		std::string::iterator	&statementEnd;
		std::string::iterator	&eof;

	private:
		static std::list<char> endStatementChars;
		
		// Needed to avoid ConfigParse() compilation errors
		const static std::string			empty;
		static std::string::iterator		ebgn;
		static std::string::iterator		eend;
		ConfigParse() : statementBegin(ebgn), statementEnd(ebgn), eof(eend){};
};
	const std::string	empty = "";
#endif

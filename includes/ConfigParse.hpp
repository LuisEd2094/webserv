#ifndef CONFIG_PARSE_HPP
# define CONFIG_PARSE_HPP
# include <map>
# include <list>
# include <string>
# include <exception>

class ConfigParse : std::map<std::string, std::string>
{
	public: 
		ConfigParse(std::string fileContent) : std::map<std::string, std::string>::map(){
				this->fileContent = std::string(fileContent);	
				this->statementBegin = this->fileContent.begin();
				this->eof = this->fileContent.end();
		};

		std::string _getFileContent() const { return fileContent; };

		void parse();
		std::string::iterator		findEndOfStatement();
		std::string::iterator		endOfStatement();
		void						newNestedElement(void);
		static bool					isEndOfStatement(char target);
		char						nextChar(void);

		class ConfigParseException : public std::exception
		{

			const char* what() const throw()
			{
				return("Error trying to acces out of files boundaries");
			}

		};

		//virtual ~ConfigParse();
		//virtual ConfigParse &operator=(ConfigParse &orig) = 0;
		
		std::string::iterator	eof;
	protected:
		std::string fileContent;
		std::string::iterator	statementBegin;
		std::string::iterator	statementEnd;

	private:
		//ConfigParse(ConfigParse &orig){};
		ConfigParse(){};
//		static const char endStatementChars[];
		static std::list<char> endStatementChars;
};

#endif

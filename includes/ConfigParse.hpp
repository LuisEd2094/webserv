#ifndef CONFIG_PARSE_HPP
# define CONFIG_PARSE_HPP
# include <map>
# include <list>
# include <string>

class ConfigParse : std::map<std::string, std::string>
{
	public: 
		ConfigParse(std::string fileContent) {
				this->fileContent = fileContent;	
				this->statementBegin = fileContent.begin();
				this->eof = fileContent.end();

		};
	//	virtual ~ConfigParse();

	//	virtual ConfigParse &operator=(ConfigParse &orig) = 0;
		std::string _getFileContent() const { return fileContent; };

		void parse();
		std::string::iterator findEndOfStatement();
		std::string::iterator endOfStatement();
		void	newNestedElement(void);

	protected:
		std::string fileContent;
		std::string::iterator	statementBegin;
		std::string::iterator	statementEnd;
		std::string::iterator	eof;

	private:
		//ConfigParse(ConfigParse &orig){};
		ConfigParse(){};
//		static const char endStatementChars[];
		static std::list<char> endStatementChars;
};

#endif

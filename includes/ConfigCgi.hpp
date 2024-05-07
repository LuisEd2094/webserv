#ifndef CONFIG_CGI_HPP
# define CONFIG_CGI_HPP
# include "ConfigParse.hpp"
class ConfigCgi: public ConfigParse
{
	public:
		void	createNestedElement(std::string param0, std::string param1);
		ConfigCgi(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		~ConfigCgi(){};
		//parsing
		//check key value
		//check valid nested
		//initObj
	private:

};
#endif

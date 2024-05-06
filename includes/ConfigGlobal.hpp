#ifndef CONFIG_GLOBAL_HPP
# define CONFIG_GLOBAL_HPP
# include "ConfigParse.hpp"
# include "ConfigServer.hpp"

class ConfigGlobal: public ConfigParse
{
	public:
		void	createNestedElement(std::string param0, std::string param1);
		ConfigGlobal(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		//parsing
		//check key value
		//check valid nested
		//initObj
	private:
		std::list<ConfigServer>	servers;
};
#endif

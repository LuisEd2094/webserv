#ifndef CONFIG_SERVER_HPP
# define CONFIG_SERVER_HPP
# include "ConfigParse.hpp"
# include "ConfigLocation.hpp"

class ConfigServer: public ConfigParse
{
	public:
		void	createNestedElement(std::string param0, std::string param1);
		ConfigServer(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		//void holi();
		//parsing
		//check key value
		//check valid nested
		//initObj
	private:
		std::list<ConfigLocation>	locatinos;
};
#endif

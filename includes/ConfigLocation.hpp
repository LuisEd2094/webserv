#ifndef CONFIG_LOCATION_HPP
# define CONFIG_LOCATION_HPP
# include "ConfigParse.hpp"
# include "ConfigCgi.hpp"

class ConfigLocation: public ConfigParse
{
	public:
		void holi();
		void	createNestedElement(std::string param0, std::string param1);
		ConfigLocation(std::string::iterator &begin, std::string::iterator &eof, std::string::iterator &statementEnd);
		//parsing
		//check key value
		//check valid nested
		//initObj
	private:
		std::list<ConfigLocation>	locatinos;
		std::list<ConfigCgi>		cgis;

};
#endif

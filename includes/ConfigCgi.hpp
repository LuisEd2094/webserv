
# ifndef CONFIG_CGI_HPP
# define CONFIG_CGI_HPP

# include "ConfigElement.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"
# include <string>

class ConfigCgi: public ConfigElement
{
	private:
		std::string				errorPage;
		std::list<std::string>	methods;
		std::string				redirection;
		std::string				root; // would be nice to change it to an oject
		bool					dirListing;
		std::list<std::string>	index;



	public:

		void parseKeyVal(std::string key, std::string val);
		ConfigCgi(void){}; // TODO
		ConfigCgi(ParsingServer& obj);
		ConfigCgi &operator=( ConfigCgi& obj);
		~ConfigCgi(){};
};

std::ostream &operator<<(std::ostream &os, const ConfigCgi &obj);

# endif

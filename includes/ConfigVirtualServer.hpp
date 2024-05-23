
# ifndef CONFIG_VIRTUAL_ELEMENT_HPP
# define CONFIG_VIRTUAL_ELEMENT_HPP

# include "ConfigElement.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"
# include <string>

class ConfigVirtualServer: public ConfigElement
{
	private:
		std::string	errorPage;
		int			maxClientBodySize;

	public:
		//set
		void setErrorPage(std::string error);
		void setMaxClientBodySize(int bodySize);
		void setMaxClientBodySize(std::string bodySize);
		//get
		std::string getErrorPage(void) ;
		int getMaxClientBodySize(void) ;

		void parseKeyVal(std::string key, std::string val);
		ConfigVirtualServer(void): errorPage(""), maxClientBodySize(30000){};
		ConfigVirtualServer(ParsingServer& obj);
		ConfigVirtualServer &operator=( ConfigVirtualServer& obj);
		~ConfigVirtualServer(){};
};
std::ostream &operator<<(std::ostream &os, const ConfigVirtualServer &obj);

# endif

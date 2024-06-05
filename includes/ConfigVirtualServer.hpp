
# ifndef CONFIG_VIRTUAL_SERVER_HPP
# define CONFIG_VIRTUAL_SERVER_HPP

# include "ConfigElement.hpp"
# include "ConfigLocation.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"
# include <string>

class ConfigVirtualServer: public ConfigElement
{
	private:
		std::string	errorPage;
		int			maxClientBodySize;
		std::list<std::string> serverNames;
		std::list<ConfigLocation> locations;

	public:
		//set
		void setErrorPage(std::string error);
		void setMaxClientBodySize(int bodySize);
		void setMaxClientBodySize(std::string bodySize);
		void setServerNames(std::string serverName);
		void setLocations(std::list<ConfigLocation> inpLocations) {this->locations = inpLocations;};
		//get
		std::string getErrorPage(void)  const;
		int getMaxClientBodySize(void)  const;
		std::list<std::string> getServerNames(void);
		std::list<ConfigLocation> getLocations(void) {return this->locations;};

		void recursivePrint(void);
		void recursivePrint(int recursiveLvl);

		void parseKeyVal(std::string key, std::string val);
		ConfigVirtualServer(void): errorPage(""), maxClientBodySize(30000){};
		ConfigVirtualServer(ParsingServer& obj);
		ConfigVirtualServer &operator=( ConfigVirtualServer& obj);
		~ConfigVirtualServer(){};
};
std::ostream &operator<<(std::ostream &os, const ConfigVirtualServer &obj);

# endif

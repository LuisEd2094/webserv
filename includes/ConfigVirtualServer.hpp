
#ifndef CONFIG_VIRTUAL_SERVER_HPP
# define CONFIG_VIRTUAL_SERVER_HPP

class ConfigVirtualServer;

# include "ConfigElement.hpp"
# include "ConfigLocation.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"
# include <string>

# include "Client.hpp"

class ConfigVirtualServer: public ConfigElement
{
	private:
		std::string	errorPage;
		unsigned int			maxClientBodySize;
		urlSize					maxUrlLen;
		std::string	host;
		std::string port;
		std::list<std::string> serverNames;
		std::list<ConfigLocation> locations;

	public:
		//set
		void setErrorPage(std::string error);
		void setPort(const std::string &port);
		void setHost(const std::string &host);
		void setMaxClientBodySize(int bodySize);
		void setMaxClientBodySize(std::string bodySize);
		void setMaxUrlLen(std::string len) { this->maxUrlLen = std::atoi(len.c_str()); };
		void setMaxUrlLen(urlSize len) { this->maxUrlLen = len; };
		void setServerNames(std::string &serverName);
		void setLocations(std::list<ConfigLocation> inpLocations) {this->locations = inpLocations;};
		//get
		std::string	getPort(void) const;
		std::string	getHost(void) const;
		std::string getErrorPage(void) const;
		int getMaxClientBodySize(void) const;
		urlSize getMaxUrlLen(void) const { return this->maxUrlLen; };
		std::list<std::string> getServerNames(void);
		std::list<ConfigLocation> getLocations(void) {return this->locations;};

		void recursivePrint(void);
		void recursivePrint(int recursiveLvl);

		bool prepareClient4ResponseGeneration(Client& client);

		void parseKeyVal(std::string key, std::string val);
		ConfigVirtualServer(void): errorPage(""), maxClientBodySize(30000){};
		ConfigVirtualServer(const ConfigVirtualServer& obj) {*this = obj;};
		ConfigVirtualServer(ParsingServer& obj);
		ConfigVirtualServer &operator=(const ConfigVirtualServer& obj);
		~ConfigVirtualServer(){};
};
std::ostream &operator<<(std::ostream &os, const ConfigVirtualServer &obj);

#endif

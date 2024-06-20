
# ifndef CONFIG_CGI_HPP
# define CONFIG_CGI_HPP
class ConfigCgi;
# include <string>
# include "ConfigElement.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"
# include "Path.hpp"

# include "Client.hpp"

class ConfigCgi: public ConfigElement
{
	private:
		std::string							__elemType__;
		std::string							__elemArgument__;
		std::string							extension;
		std::map<std::string, std::string>	metaVar;
		ConfigLocation const				*location;
		Path								execute;

	public:

		ConfigCgi(void); // TODO
		ConfigCgi(ParsingCgi& obj, ConfigLocation parent);
		ConfigCgi(const ConfigCgi& obj);
		ConfigCgi &operator=( const ConfigCgi& obj);
		~ConfigCgi(){};

		void set__elemType__(std::string v){this->__elemArgument__ = v;};
		void set__elemArgument__(std::string v){this->__elemArgument__ = v;};
		void setExtension(std::string &extension);
		void setMetaVar(std::string &metaVar);
		void setLocation(const ConfigLocation &location);
		void setExecute(std::string &execute);


		std::string get__elemType__(void) const {return this->__elemArgument__;};
		std::string get__elemArgument__(void) const {return this->__elemArgument__;};
		std::string getExtension(void) const;
		const std::map<std::string, std::string> &getMetaVar(void) const;
		const ConfigLocation	&getLocation(void) const;
		Path		getExecute(void) const;

		bool prepareClient4ResponseGeneration(Client& client, Path &requestedUrl) const;

		void recursivePrint(int recursiveLvl);
		void setDefaults();
		void parseKeyVal(std::string key, std::string val);
};

std::ostream &operator<<(std::ostream &os, const ConfigCgi &obj);

# endif

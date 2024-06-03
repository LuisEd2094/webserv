
# ifndef CONFIG_CGI_HPP
# define CONFIG_CGI_HPP

# include <string>
# include "ConfigElement.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"

class ConfigCgi: public ConfigElement
{
	private:
		std::string					__elemType__;
		std::string					__elemArgument__;

	public:

		ConfigCgi(void); // TODO
		ConfigCgi(ParsingCgi& obj);
		ConfigCgi(const ConfigCgi& obj);
		ConfigCgi &operator=( const ConfigCgi& obj){ return (*this);};
		~ConfigCgi(){};

		void set__elemType__(std::string v){this->__elemArgument__ = v;};
		void set__elemArgument__(std::string v){this->__elemArgument__ = v;};
		std::string get__elemType__(void) const {return this->__elemArgument__;};
		std::string get__elemArgument__(void) const {return this->__elemArgument__;};

		void recursivePrint(int recursiveLvl);
		void setDefaults();
		void parseKeyVal(std::string key, std::string val);
};

std::ostream &operator<<(std::ostream &os, const ConfigCgi &obj);

# endif

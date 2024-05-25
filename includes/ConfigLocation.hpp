
# ifndef CONFIG_LOCATION_HPP
# define CONFIG_LOCATION_HPP

# include "ConfigElement.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"
# include <string>
# include "ConfigCgi.hpp"


class ConfigLocation: public ConfigElement
{
	private:
		std::string				errorPage;
		std::list<std::string>	methods;
		std::string				redirection;
		std::string				root; // would be nice to change it to an oject
		bool					dirListing;
		std::list<std::string>	index;
		std::list<ConfigCgi>	cgis;

	public:
		void parseKeyVal(std::string key, std::string val);
		ConfigLocation(void){};
		ConfigLocation(ParsingLocation& obj);
		ConfigLocation &operator=( ConfigLocation& obj);
		~ConfigLocation(){};


		void							setErrorPage(std::string inpErrorPage);
		void							setMethods(std::string methods);
		void							setRedirection(std::string methods);
		void							setRoot(std::string methods);
		void							setDirListing(std::string methods);
		void							setIndex(std::string methods);
		void							setCgis(std::string methods);

		std::string						getErrorPage(void);
		std::list<std::string>			getMethods(void);
		std::string						getRedirection(void);
		std::string						getRoot(void);
		bool							getDirListing(void);
		std::list<std::string>			getIndex(void);
		std::list<ConfigCgi>			getCgis(void);
		std::list<ConfigLocation>		getLocations(void);
};
std::ostream &operator<<(std::ostream &os, const ConfigLocation &obj);

# endif

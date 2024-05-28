
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
		std::string				_errorPage;
		std::list<std::string>	_methods;
		std::string				_redirection;
		std::string				_root; // would be nice to change it to an oject
		bool					_dirListing;
		std::list<std::string>	_index;
		std::list<ConfigCgi>	_cgis;
		//Add list configureLocation 

	public:
		void parseKeyVal(std::string key, std::string val);
		ConfigLocation(void);
		ConfigLocation(ParsingLocation& obj);
		ConfigLocation &operator=( const ConfigLocation& obj);
		~ConfigLocation(){};


		void							setErrorPage(std::string inpErrorPage);
		void							setMethods(std::string methods);
		void							setRedirection(std::string redirection);
		void							setRoot(std::string root);
		void							setDirListing(std::string dirListing);
		void							setIndex(std::string index);
		void							setCgis(std::string cgis);//*

		std::string						getErrorPage(void) const;
		std::list<std::string>			getMethods(void) const;
		std::string						getRedirection(void) const;
		std::string						getRoot(void) const;
		bool							getDirListing(void) const;
		std::list<std::string>			getIndex(void) const;
		std::list<ConfigCgi>			getCgis(void) const;//*
		std::list<ConfigLocation>		getLocations(void) const;
};
std::ostream &operator<<(std::ostream &os, const ConfigLocation &obj);

# endif

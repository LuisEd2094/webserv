
# ifndef CONFIG_LOCATION_HPP
# define CONFIG_LOCATION_HPP

# include "ConfigElement.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"
# include <string>
# include "ConfigCgi.hpp"
# include "Path.hpp"

class ConfigLocation: public ConfigElement
{
	private:
		std::string					__elemType__;
		std::string					__elemArgument__;
		std::list<std::string>		_methods;
		Path						_root; // would be nice to change it to an oject
		std::list<std::string>		_index;
		std::string					_redirection;
		std::string					_errorPage;
		bool						_dirListing;
		std::list<ConfigCgi>		_cgis;
		std::list<ConfigLocation>	_locations;
		bool						_inheriting;
		//Add list configureLocation 

	public:
		void parseKeyVal(std::string key, std::string val);
		ConfigLocation(void);
		ConfigLocation(ParsingLocation& obj);
		ConfigLocation(const ConfigLocation& obj);
		ConfigLocation( ParsingLocation& obj, ConfigLocation& father);
		ConfigLocation &operator=( const ConfigLocation& obj);
		~ConfigLocation(){};

		void							setDefaults();
		void							setErrorPage(std::string inpErrorPage);//
		void							setMethods(std::string methods);//
		void							setRedirection(std::string redirection);//
		void							setRoot(std::string root);
		void							setRoot(Path root);
		void							initializeRoot(Path root);
		void							initializeRoot(std::string root);
		void							setDirListing(std::string dirListing);
		void							setDirListing(bool dirListing);
		void							setIndex(std::string index);//
		void							setCgis(std::string cgis);//*

		std::string						getErrorPage(void) const;
		std::list<std::string>			getMethods(void) const;
		std::string						getRedirection(void) const;
		Path							getRoot(void) const;
		bool							getDirListing(void) const;
		std::list<std::string>			getIndex(void) const;
		std::list<ConfigCgi>			getCgis(void) const;//*
		std::list<ConfigLocation>		getLocations(void) const;

		void							recursivePrint(int recursiveLvl);
};
std::ostream &operator<<(std::ostream &os, const ConfigLocation &obj);

# endif

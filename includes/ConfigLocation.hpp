
# ifndef CONFIG_LOCATION_HPP
# define CONFIG_LOCATION_HPP
class ConfigLocation;
# include "ConfigElement.hpp"
# include "ParsingServer.hpp"
# include "Parsing.hpp"
# include <string>
# include "ConfigCgi.hpp"
# include "Path.hpp"


# include "Client.hpp"


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
		Path						_path;
		std::map<ErrorCodes, Path>			_errorPages;
		//Add list configureLocation 

	public:
		void parseKeyVal(std::string key, std::string val);
		ConfigLocation(void);
		ConfigLocation(ParsingLocation& obj);
		ConfigLocation(const ConfigLocation& obj);
		ConfigLocation( ParsingLocation& obj, ConfigLocation& father);
		ConfigLocation &operator=( const ConfigLocation& obj);
		~ConfigLocation(){}

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
		void							setPath(Path &path) { this->_path = path; };//*
		void							setErrorPages(std::string errors);

//		Path							getPath(void) const { std::cout << "returning: " << _path << std::endl; return this->_path;};
		int								size() { return this->_path.size(); };
		const Path& 					getPath(void) const { return this->_path;};
		std::string						getErrorPage(void) const;
		std::list<std::string>			getMethods(void) const;
		std::string						getRedirection(void) const;
		Path							getRoot(void) const;
		bool							getDirListing(void) const;
		std::list<std::string>			getIndex(void) const;
		const std::list<ConfigCgi>			&getCgis(void) const;//*
		std::list<ConfigLocation>		getLocations(void) const;
		std::map<ErrorCodes, Path>				getMapErrorPages(void) const;
		Path							const getErrorPages(ErrorCodes searchError) const;

		bool 							prepareClient4ResponseGeneration(Client& client, Path& trequestedURL);
		bool							checkCGI(Client &client, Path& requestedURL);
		void							recursivePrint(int recursiveLvl);

		static std::list<ConfigLocation>::iterator getBestLocation(Path requestedURL,
			std::string							method,
			std::list<ConfigLocation>::iterator firstLocation,
			std::list<ConfigLocation>::iterator lastLocation);

		static bool getBestLocation( Client &client, Path requestedURL,
			std::list<ConfigLocation>::iterator beginLocation,
			std::list<ConfigLocation>::iterator endLocation);

};
std::ostream &operator<<(std::ostream &os, const ConfigLocation &obj);

# endif

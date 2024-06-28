
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
		Path						_fullUrl;
		std::map<ResponseCodes, Path>	_errorPages;
		ConfigVirtualServer const	*_virtualServer;
		ResponseCodes						_codeRedirections;
		std::list<Uri>						_redirections;
		//Add list configureLocation 

	public:
		void parseKeyVal(std::string key, std::string val);
		ConfigLocation(void);
		ConfigLocation(ParsingLocation& obj, const ConfigVirtualServer &virtualServer);
		ConfigLocation(const ConfigLocation& obj);
		ConfigLocation( ParsingLocation& obj, ConfigLocation& father);
		ConfigLocation &operator=( const ConfigLocation& obj);
		~ConfigLocation(){}

		void							setDefaults();
		void							setErrorPage(std::string inpErrorPage);
		void							setMethods(std::string methods);
		void							setRedirection(std::string redirection);
		void							setRoot(std::string root);
		void							setRoot(Path root);
		void							initializeRoot(Path root);
		void							initializeRoot(std::string root);
		void							setDirListing(std::string dirListing);
		void							setDirListing(bool dirListing);
		void							setIndex(std::string index);
		void							setCgis(std::string cgis);
		void							setPath(Path &path) { this->_path = path; };
		void							setFullUrl(const Path &path);//*
		void							setErrorPages(std::string errors);
		void							setRedirections(const std::string &redirections);
		void							setVirtualServer(const ConfigVirtualServer &server);

		int								size() { return this->_path.size(); };
		const std::string&				get__elemArg__() const { return this->__elemArgument__;};
		const std::string				getErrorPage(ResponseCodes err) const;
		const Path& 					getPath(void) const { return this->_path;};
		const Path& 					getFullUrl(void) const { return this->_fullUrl;};
		std::string						getErrorPage(void) const;
		std::list<std::string>			getMethods(void) const;
		std::string						getRedirection(void) const;
		Path							getRoot(void) const;
		bool							getDirListing(void) const;
		std::list<std::string>			getIndex(void) const;
		const std::list<ConfigCgi>		&getCgis(void) const;
		std::list<ConfigLocation>		getLocations(void) const;
		const std::map<ResponseCodes, Path>	&getMapErrorPages(void) const;
		const Path						getErrorPages(ResponseCodes searchError) const;
		ResponseCodes					getCodeRedirections(void) const;
		const std::list<Uri>			&getRedirections(void) const;
		const ConfigVirtualServer		&getVirtualServer(void) const ;

		bool							checkDirecotry(Client &client) const;
		bool							checkIndex(Client &client) const;
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

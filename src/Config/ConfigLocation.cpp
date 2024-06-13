# include <ConfigLocation.hpp>

ConfigLocation::ConfigLocation(void)
{
	this->setDefaults();
}

ConfigLocation::ConfigLocation(const ConfigLocation& obj)
{
	this->__elemArgument__ = obj.__elemArgument__;
	this->__elemType__ = obj.__elemType__;
	this->_errorPage = obj.getErrorPage();
	this->_methods = obj.getMethods();
	this->_redirection = obj.getRedirection();
	this->_root = obj.getRoot();
	this->_dirListing = obj.getDirListing();
	this->_index = obj.getIndex();
	this->_cgis = obj.getCgis();
	this->_locations = obj.getLocations();
	this->_cgis = obj.getCgis();
	this->_path = obj.getPath();
}

ConfigLocation::ConfigLocation( ParsingLocation& obj, ConfigLocation& father)
{
	*this = father;
	// TODO erase the nested elements from the father
	this->_locations.empty();
	this->_cgis.empty();
	this->_inheriting = true;
	this->nestedPrint = 0;
	this->_dirListing = 0;

	for (std::map<std::string, std::string>::iterator i = obj.begin(); i != obj.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	this->__elemType__ = obj["__elemType__"]; 
	this->__elemArgument__ = obj["__elemArgument__"]; 
	this->_path = Path(this->__elemArgument__);
	this->_inheriting = false;
	if (obj.find("root") == obj.end())
		this->_root.append(obj.find("__elemArgument__")->second);

	std::list<ParsingLocation> locs = obj.getLocations(); 
	for (std::list<ParsingLocation>::iterator location = locs.begin();
		location != locs.end();
		location++
	)
	{
		this->_locations.push_back(ConfigLocation(*location, *this));
	}
	std::list<ParsingCgi> cgis = obj.getCgis(); 
	for (std::list<ParsingCgi>::iterator cgi = cgis.begin();
		cgi != cgis.end();
		cgi++
	)
	{
		this->_cgis.push_back(ConfigCgi(*cgi));
	}
}

/**/
ConfigLocation::ConfigLocation(ParsingLocation& obj)
{
	// TODO erase the nested elements from the father
	this->_locations.empty();
	this->_cgis.empty();
	this->_inheriting = true;
	this->nestedPrint = 0;
	this->_dirListing = 0;

	for (std::map<std::string, std::string>::iterator i = obj.begin(); i != obj.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	this->__elemType__ = obj["__elemType__"]; 
	this->__elemArgument__ = obj["__elemArgument__"]; 
	this->_path = Path(this->__elemArgument__);
	this->_inheriting = false;
	if (obj.find("root") == obj.end())
		this->_root.append(obj.find("__elemArgument__")->second);
	std::list<ParsingLocation> locs = obj.getLocations(); 
	for (std::list<ParsingLocation>::iterator location = locs.begin();
		location != locs.end();
		location++
	)
	{
		this->_locations.push_back(ConfigLocation(*location, *this));
	}
	std::list<ParsingCgi> cgis = obj.getCgis(); 
	for (std::list<ParsingCgi>::iterator cgi = cgis.begin();
		cgi != cgis.end();
		cgi++
	)
	{
		this->_cgis.push_back(ConfigCgi(*cgi));
	}
}

void ConfigLocation::setDefaults()
{
	this->setMethods("");	
	this->setRoot("");	
	this->setIndex("");
	this->setErrorPage("");	
	this->setRedirection("");	
	this->setDirListing(true);	
	this->_path = Path();
}

void ConfigLocation::parseKeyVal(std::string key, std::string val)
{
	if (key == "__elemType__") ;
	else if (key == "__elemArgument__") ;
	else if (key == "errorPage")
		this->setErrorPage(val);	
	else if (key == "methods")
		this->setMethods(val);	
	else if (key == "redirection")
		this->setRedirection(val);	
	else if (key == "root") // if father root = /tmp/sdaf/; then for child root = wololo -> /tmp/sdaf, else for child root = /var -> /var
		this->initializeRoot(val);	
	else if (key == "dirListing")
		this->setDirListing(val);	
	else if (key == "index")
		this->setIndex(val);
	else
		throw ParamError(std::string("Error: location key not found.") + std::string(" key:") + key);
}

void	ConfigLocation::setErrorPage(std::string inpErrorPage)
{
	this->_errorPage = inpErrorPage;
}
std::string	ConfigLocation::getErrorPage(void) const
{
	return (this->_errorPage);
}

void ConfigLocation::setMethods(std::string inpMethods)
{
	_methods = ft_split(inpMethods, ' ');
}

std::list<std::string>	ConfigLocation::getMethods(void) const
{
	return (this->_methods);
}

void	ConfigLocation::setRedirection(std::string redirection)
{
	_redirection = redirection;
}

std::string ConfigLocation::getRedirection(void) const
{
	return (this->_redirection);
}

void ConfigLocation::setRoot(std::string root)
{
	this->_root = Path(root);
	this->_root.setIsFile(false);
}

void ConfigLocation::setRoot(Path root)
{
	this->_root = root;
	this->_root.setIsFile(false);
}

void ConfigLocation::initializeRoot(std::string root)
{
	initializeRoot(Path(root));
}

void ConfigLocation::initializeRoot(Path root)
{
	if (!_inheriting)
	{
		_root = root;
		return ;
	}	
	else if (root.getIsRelative())
	{
		_root.append(root);
	}
	else	
		_root = root;
	_root.setIsFile(false);
	// TODO the file is correct
}

Path ConfigLocation::getRoot(void) const
{
	return (this->_root);
}


void ConfigLocation::setDirListing(bool dirListing)
{
		_dirListing = dirListing;
}

void ConfigLocation::setDirListing(std::string dirListing)
{
		_dirListing = dirListing == "true" || dirListing == "True";
}

bool ConfigLocation::getDirListing(void) const
{
	return (this->_dirListing);
}

void ConfigLocation::setIndex(std::string index)
{
	_index = ft_split(index, ' ');
}

std::list<std::string> ConfigLocation::getIndex(void) const
{
	return (this->_index);
}

void ConfigLocation::setCgis(std::string cgis)
{
	// _cgis = ft_split(cgis, ' ');
	(void)cgis;
}

std::list<ConfigCgi> ConfigLocation::getCgis(void) const
{
	return (this->_cgis);
}

std::list<ConfigLocation> ConfigLocation::getLocations(void) const
{
	return (this->_locations);
}

ConfigLocation &ConfigLocation::operator=(const ConfigLocation& obj)
{
	this->_errorPage = obj.getErrorPage();
	this->_methods = obj.getMethods();
	this->_redirection = obj.getRedirection();
	this->_root = obj.getRoot();
	this->_dirListing = obj.getDirListing();
	this->_index = obj.getIndex();
	this->_cgis = obj.getCgis();

	return (*this);
}

std::ostream &operator<<(std::ostream &os,  std::list<std::string> &listObj)
{
	for (std::list<std::string>::iterator elem = listObj.begin(); elem != listObj.end(); elem++)
	{
		os << *elem << " ";
	}
	return os;
}

void ConfigLocation::recursivePrint(int recursiveLvl)
{
	std::cout << ConfigElement::genSpace(recursiveLvl) << "- Location (" << this->_path << ")" << std::endl;
	recursiveLvl++;

	std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "Methods: " << this->_methods << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "Root: " << this->_root << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "Index: " << this->_index << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "Redirection: " << this->_redirection << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "Error page: " << this->_errorPage << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "Dir listing: " << this->_dirListing << std::endl;

	std::cout << ConfigElement::genSpace(recursiveLvl) << "Locations(" << this->_locations.size() << "):" << std::endl;
	for (std::list<ConfigLocation>::iterator loc = this->_locations.begin(); loc != this->_locations.end();loc++)
	{
		loc->recursivePrint(recursiveLvl);
	}
	std::cout << ConfigElement::genSpace(recursiveLvl) << "CGIS(" << this->_cgis.size() << "):" << std::endl;
	for (std::list<ConfigCgi>::iterator cgi = this->_cgis.begin(); cgi != this->_cgis.end();cgi++)
	{
		cgi->recursivePrint(recursiveLvl);
	}
	// TODO copy code above for cgis instead of locations
}

bool ConfigLocation::prepareClient4ResponseGeneration(Client& client,
	Path &requestedURL
)
{
	bool inBestLocation;

	std::cout << "::: ConfigLocation::prepareClient4ResponseGeneration " << this->getPath() << std::endl;
	std::string url = client.getURL() ;

	inBestLocation = !ConfigLocation::getBestLocation( client, requestedURL,
		this->_locations.begin(),
		this->_locations.end()
	);
	if (inBestLocation)
	{
		client.setPathFile
		(
			static_cast<std::string>(this->_root) +
			static_cast<std::string>(requestedURL)
		);
		std::cout << BLUE << std::endl;
		client.setDefaultHttpResponse(OK);
		if (false)
			;
		else if (Path(client.getURL()).getIsFile())	
		{
			if (client.getPathFile().assertFileExists())
			{
				std::cout << "return file" << std::endl;
				client.setResponseType(FILE_OBJ);
			}
			else if (this->_dirListing && client.getPathFile().assertDirExists())
			{
				std::cout << "should be file return file" << std::endl;
				client.setResponseType(DIR_OBJ);
			}
			else
			{
				std::cout << "unable to serve file" << std::endl;
				client.setDefaultHttpResponse(NOT_FOUND); /// SHOULD BE AN ERROR
			}
		}
		else if (this->_dirListing && client.getPathFile().assertDirExists())	
			{
				std::cout << "returning direcotry" << std::endl;
				client.setResponseType(DIR_OBJ);
			}
		else 
			{
				std::cout << "unable to serve directory" << std::endl;
				client.setDefaultHttpResponse(NOT_FOUND); /// SHOULD BE AN ERROR
			}
		std::cout << END << std::endl;


		
		std::cout << "      BINGO !!!" << std::endl;
		std::cout << "RENSPONSES: NO_FOUND=" << NOT_FOUND << " OK=" << OK << std::endl;
		std::cout << TUR << "      Bestlocation: " << END << (std::string)this->_locations.begin()->getPath()<<  std::endl;
		std::cout << "      Cient  URL: " << client.getURL() << std::endl;
		std::cout << "      Response type: " << ObjectTypesStrings[client.getResponseType()];
		std::cout << "      Default HTTP response: " << client.getDefaultHttpResponse();
		std::cout << "      Path file: " << client.getPathFile();
		std::cout << std::endl;
	}
	std:: cout << TUR << "bestlocation: " << END << this->getPath()<<  std::endl;
	return true;
}

//std::list<ConfigLocation>::iterator ConfigLocation::getBestLocation(Path requestedURL,
//bool ConfigLocation::getBestLocation(Path requestedURL,
//bool getBestLocation( Client &client, Path requestedURL,
	//std::list<ConfigLocation>::iterator beginLocation,
	//std::list<ConfigLocation>::iterator endLocation
//)
bool ConfigLocation::getBestLocation( Client &client, Path requestedURL,
	std::list<ConfigLocation>::iterator beginLocation,
	std::list<ConfigLocation>::iterator endLocation
)
{
//	std::string requestedURL = client.getURL();
	std::string requestMethod = client.getMethod();
//	std::list<ConfigLocation>::iterator location;
	std::list<std::string>				locMethods ;
	std::list<ConfigLocation>::iterator	bestLocation = endLocation;
	int									maxDirMatches = 0;
	std::cout << "··· getBestLocation " << requestedURL << std::endl;
	while (beginLocation != endLocation)
	{
		std::cout << "      Location " << *beginLocation ;
		locMethods = beginLocation->getMethods();
		/*
		if (locMethods.size() == 0)
		{
			beginLocation++ ;
			continue;
		}
		*/
		if (locMethods.size() == 0)
			;
		else if (beginLocation->getPath().included(requestedURL)
			&& std::find(locMethods.begin(), locMethods.end(), requestMethod) != locMethods.end()
			&& beginLocation->getPath().size() > maxDirMatches) 	
		{
			maxDirMatches = beginLocation->getPath().size();
			bestLocation = beginLocation;
			std::cout << "  MATCHED" ;
		}
		std::cout << std::endl;
		beginLocation++ ;
	}
	//return bestLocation;
	if (bestLocation == endLocation)
		return false;

//	Path nextURL(client.getURL());
	std::cout << "      Before pop: " << bestLocation->size() << " " << requestedURL << std::endl;
	requestedURL.popBegin(bestLocation->size());
	std::cout << "      After pop: " << requestedURL << std::endl;
	std::cout << "      First nested location: " << *bestLocation->_locations.begin() << std::endl;
	bestLocation->prepareClient4ResponseGeneration(client, requestedURL);
	return (true);
}

std::ostream &operator<<(std::ostream &os, const ConfigLocation &obj)
{
	os << "ConfigLocation (" << obj.getPath() << "): " << std::endl;
//	os << "  errorPage: " << obj.getErrorPage() << std::endl;
	return os;
}
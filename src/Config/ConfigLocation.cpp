# include <ConfigLocation.hpp>

ConfigLocation::ConfigLocation(void)
{
	this->setDefaults();
}

ConfigLocation::ConfigLocation(const ConfigLocation& obj)
{
	*this = obj;
}

ConfigLocation::ConfigLocation( ParsingLocation& obj, ConfigLocation& father)
{
	*this = father;
	std::map<std::string, std::string>::iterator root;

	this->_locations.empty();
	this->_cgis.empty();
	this->_inheriting = true;
	this->nestedPrint = 0;
	this->_dirListing = 0;

	root = obj.find("root");
	if (root == obj.end())
		this->_root.append(obj.find("__elemArgument__")->second);
	else 
		this->initializeRoot(root->second);
	
	for (std::map<std::string, std::string>::iterator i = obj.begin(); i != obj.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	this->__elemType__ = obj["__elemType__"]; 
	this->__elemArgument__ = obj["__elemArgument__"]; 
	this->_path = Path(this->__elemArgument__);
	this->_fullUrl.append(this->__elemArgument__);
	this->_inheriting = false; //TODO delte?
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
		this->_cgis.push_back(ConfigCgi(*cgi, *this));
		this->_cgis.back().setRoot(this->_root);
	}
}

/**/
ConfigLocation::ConfigLocation(ParsingLocation& obj, const ConfigVirtualServer &server)
{
	// TODO erase the nested elements from the father
	std::map<std::string, std::string>::iterator root;
	this->_locations.empty();
	this->_cgis.empty();
	this->_inheriting = false;
	this->_dirListing = 0;
	this->nestedPrint = 0;
	this->_virtualServer = &server;

	root = obj.find("root");
	if (root == obj.end())
		this->_root.append(obj.find("__elemArgument__")->second);
	else 
		this->initializeRoot(root->second);

	if (obj.find("root") == obj.end())
	{
		this->_root.append(obj.find("__elemArgument__")->second); 
		this->_root.setIsRelative(true);
		this->_root.setIsFile(false);
	}
	for (std::map<std::string, std::string>::iterator i = obj.begin(); i != obj.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	this->__elemType__ = obj["__elemType__"]; 
	this->__elemArgument__ = obj["__elemArgument__"]; 
	this->_path = Path(this->__elemArgument__);
	this->setFullUrl(Path(this->__elemArgument__));
	std::list<ParsingLocation> locs = obj.getLocations(); 
	for (
		std::list<ParsingLocation>::iterator location = locs.begin();
		location != locs.end();
		location++)
	{
		this->_locations.push_back(ConfigLocation(*location, *this));
	}
	std::list<ParsingCgi> cgis = obj.getCgis(); 
	for (std::list<ParsingCgi>::iterator cgi = cgis.begin();
		cgi != cgis.end();
		cgi++
	)
	{
		this->_cgis.push_back(ConfigCgi(*cgi, *this));
		this->_cgis.back().setRoot(this->_root);
	}
}

void ConfigLocation::setDefaults()
{
	this->setMethods("");	
	this->setRoot("./");	
	this->setIndex("");
	this->setErrorPage("");	
	this->setRedirection("");	
	this->setDirListing(true);	
	this->_path = Path();
	this->_fullUrl = Path();
	this->_virtualServer = NULL;
}

void ConfigLocation::parseKeyVal(std::string key, std::string val)
{
	if (key == "__elemType__") ;
	else if (key == "__elemArgument__") ;
	else if (key == "errorPage")
	{
		this->setErrorPage(val);
		this->setErrorPages(val);
	}
	else if (key == "methods")
		this->setMethods(val);	
	else if (key == "redirection")
	{
		this->setRedirection(val);
		this->setRedirections(val);
	}
	else if (key == "root")
		;	
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
	_methods = ft_split<std::list<std::string> >(inpMethods, ' ');
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
	}	
	else if (root.getIsRelative())
	{
		_root.append(root);
	}
	else	
		_root = root;
	_root.setIsFile(false);
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
	_index = ft_split<std::list<std::string> >(index, ' ');
}

std::list<std::string> ConfigLocation::getIndex(void) const
{
	return (this->_index);
}

void ConfigLocation::setCgis(std::string cgis)
{
	(void)cgis;
}

void	ConfigLocation::setVirtualServer(const ConfigVirtualServer &server){ this->_virtualServer = &server; } 
const ConfigVirtualServer &ConfigLocation::getVirtualServer(void) const
{
	if (this->_virtualServer == NULL)
		throw std::exception();
	return *this->_virtualServer;
}

const std::list<ConfigCgi> &ConfigLocation::getCgis(void) const
{
	return (this->_cgis);
}

std::list<ConfigLocation> ConfigLocation::getLocations(void) const
{
	return (this->_locations);
}

void	ConfigLocation::setFullUrl(const Path &url)
{
	this->_fullUrl = url;
	if (this->getFullUrl().toStr().size() > MAX_URL_SIZE)
		throw ParamError("Location url is to large: " + this->getFullUrl().toStr());
}

void	ConfigLocation::setErrorPages(std::string errors)
{
	std::list<std::string> split = ft_split<std::list<std::string> >(errors, ' ');
	for (std::list<std::string>::iterator i = split.begin(); i != split.end(); i++)
	{
		std::string ErrorKey = ::getKey(*i, '-');
		std::string ErrorValue =  ::getValue(*i, '-');

		if (ErrorKey == "ERROR" or ErrorValue == "ERROR")
			throw ParamError(std::string("Error: invalid Error page Code: all"));
		bool isNum = true;
		for (size_t i = 0; i < ErrorKey.length() && isNum; i++)
			isNum = std::isdigit(ErrorKey[i]);
		if (ErrorKey.length() != 3 or !isNum)
			throw ParamError(std::string("Error: Syntax errorPage:")
			+ std::string(ErrorKey.length() != 3 ? " invalid " : " it is not a number ") + ErrorKey);
		int ErrorKeyNum = std::atoi(ErrorKey.c_str());

		ResponseCodes error = Response::getErrorCodeFromInt(ErrorKeyNum);
		if (error == INVALID_CODE)
			throw ParamError(std::string("Error: invalid Error page Code: " + ErrorKey + "."));
		
		Path errorPath(ErrorValue);
		if (errorPath.getIsRelative())
			this->_errorPages[error] = Path(_root).append(errorPath);
		else
			this->_errorPages[error] = errorPath;
	}	
}

void	ConfigLocation::setRedirections(const std::string &redirections)
{
	std::list<std::string> code = ft_split<std::list<std::string> >(redirections, ' ');
	if (code.size() < 2)
		throw ParamError(std::string("Error: invalid redirection"));
	code.front() = ::cutSpace(code.front());
	bool isNum = true;
	for (size_t i = 0; i < code.front().length() && isNum; i++)
		isNum = std::isdigit(code.front()[i]);
	if (code.front().length() != 3 or !isNum)
		throw ParamError(std::string("Error: Syntax redirections:")
		+ std::string(code.front().length() != 3 ? " invalid " : " it is not a number ") + code.front());
	int codeNum = std::atoi(code.front().c_str());
	ResponseCodes codeRedirection = Response::getErrorCodeFromInt(codeNum);
	if (codeRedirection == INVALID_CODE)
		throw ParamError(std::string("Error: invalid: code not available " + code.front() + "."));
	if (codeRedirection != MULTIPLE_REDIRECTS and code.size() > 2)
		throw ParamError(std::string("Error: invalid Error: no is Multiple redirects " + code.front() + "."));
	this->_codeRedirections = codeRedirection;
	for (std::list<std::string>::iterator iter = (++code.begin()); iter != code.end(); iter++)
		this->_redirections.push_back(Uri::Parse(*iter));
}

const std::map<ResponseCodes, Path> &ConfigLocation::getMapErrorPages(void) const
{
	return (this->_errorPages);
}

const Path ConfigLocation::getErrorPages(ResponseCodes searchError) const
{
	std::map<ResponseCodes, Path>::const_iterator it = this->_errorPages.find(searchError);
	if (it == this->_errorPages.end())
	{
		return (Path(""));
	}
	return (it->second);
}

ResponseCodes ConfigLocation::getCodeRedirections(void) const
{
	return (this->_codeRedirections);
}

const std::list<Uri> &ConfigLocation::getRedirections(void) const
{
	return (this->_redirections);
}

ConfigLocation &ConfigLocation::operator=(const ConfigLocation& obj)
{
	this->__elemArgument__ = obj.__elemArgument__;
	this->__elemType__ = obj.__elemType__;
	this->_errorPage = obj.getErrorPage();
	this->_errorPages = obj._errorPages;
	this->_methods = obj.getMethods();
	this->_redirection = obj.getRedirection();
	this->_root = obj.getRoot();
	this->_dirListing = obj.getDirListing();
	this->_index = obj.getIndex();
	this->_cgis = obj.getCgis();
	this->_locations = obj.getLocations();
	this->_cgis = obj.getCgis();
	this->_path = obj.getPath();
	this->_codeRedirections = obj.getCodeRedirections();
	this->_redirections = obj.getRedirections();
	this->_fullUrl = obj.getFullUrl();
	for (std::list<ConfigCgi>::iterator cgi = this->_cgis.begin();
		cgi != this->_cgis.end();
		cgi++
	)
	{
		cgi->setLocation(*this);
		std::cout << PURPLE << "This: " << this << " Cgi's: " << &cgi->getLocation() << END << std::endl;
	}

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
	std::cerr << ConfigElement::genSpace(recursiveLvl) << "- Location (" << this->_path << ") [" << this << "]" << std::endl;
	recursiveLvl++;
	std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "FullUrl: " << this->_fullUrl << std::endl;
	std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "Methods: " << this->_methods << std::endl;
	std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "Root: " << this->_root << std::endl;
	std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "Index: " << this->_index << std::endl;
	std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "Redirection: " << this->_redirection << std::endl;
	std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "Error pages: " << std::endl;
	printMap(this->_errorPages, recursiveLvl + 1);
	std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "Dir listing: " << this->_dirListing << std::endl;

	// << ConfigElement::genSpace(recursiveLvl) << "Locations(" << this->_locations.size() << "):" << std::endl;
	for (std::list<ConfigLocation>::iterator loc = this->_locations.begin(); loc != this->_locations.end();loc++)
	{
		loc->recursivePrint(recursiveLvl);
	}
	// << ConfigElement::genSpace(recursiveLvl) << "CGIS(" << this->_cgis.size() << "):" << std::endl;
	for (std::list<ConfigCgi>::iterator cgi = this->_cgis.begin(); cgi != this->_cgis.end();cgi++)
	{
		cgi->recursivePrint(recursiveLvl);
	}
}

bool ConfigLocation::checkIndex(Client &client) const
{
	Path indexPath;

	if (this->_index.size() == 0)
		return (false);

	for (
			std::list<std::string>::const_iterator indexFile = this->_index.begin(); 
			indexFile != this->_index.end();
			indexFile++
	)
	{
		indexPath  =  Path(client.getPathFile()).append(Path(*indexFile));
		std::cout << BLUE << "TESTING: " << indexPath << " " << indexPath.assertFileExists() << END << std::endl;
		if  (indexPath.assertFileExists())
		{
			client.setPathFile(indexPath);
			return (true);
		}
	}
	std::cout << BLUE << "DIDNT FIND: " << indexPath << END << std::endl;

	return (false);
}

bool ConfigLocation::prepareClient4ResponseGeneration(Client& client,
	Path &requestedURL
)
{
	bool inBestLocation;

	// std::cerr << "::: ConfigLocation::prepareClient4ResponseGeneration " << this->getPath() << std::endl;
	std::string url = client.getURL() ;

	inBestLocation = !ConfigLocation::getBestLocation( client, requestedURL,
		this->_locations.begin(),
		this->_locations.end()
	);

	if (inBestLocation)
	{
		if (this->checkCGI(client, requestedURL))
			return (true); 
		client.setPathFile
		(
			static_cast<std::string>(this->_root) +
			static_cast<std::string>(requestedURL)
		);
		client.setConfigElement(this);
		client.setDefaultHttpResponse(OK);
		if (this->getRedirection().size() > 0)
		{
			std::cerr << "It's redirection" << std::endl;
			client.setResponseType(REDIRECT_OBJ);
			client.setDefaultHttpResponse(OK);
			return true;
		}
		else if (client.getAction() == DELETE)
			return (true);
		else if (client.getAction() == POST)
		{
			client.setResponseType(FILE_OBJ);
			client.setDefaultHttpResponse(OK);
			return (true);
		}
		else if (client.getPathFile().assertFileExists())
			client.setResponseType(FILE_OBJ);
		else if (checkIndex(client))
		{
			client.setResponseType(FILE_OBJ);
			client.setDefaultHttpResponse(OK);
		}
		else if (this->_dirListing && client.getPathFile().assertDirExists())	
			client.setResponseType(DIR_OBJ);
		else 
			client.setDefaultHttpResponse(NOT_FOUND);

		std::cerr << END << std::endl;
		std::cerr << "      BINGO !!!" << std::endl;
		std:: cerr << TUR << "bestlocation: " << END << this->getPath()<<  std::endl;
		std::cerr << "      Cient  URL: " << client.getURL() << std::endl;
		std::cerr << "      Response type: " << ObjectTypesStrings[client.getResponseType()] << std::endl;
		std::cerr << "      Default HTTP response: " << client.getDefaultHttpResponse() << std::endl;
		std::cerr << "      Path file: " << client.getPathFile()<< std::endl;;
	}
	std::cerr << std::endl;
	return true;
}

bool	ConfigLocation::checkCGI(Client &client, Path& requestedURL)
{
	std::list<ConfigCgi>::const_iterator cgi;

	for(cgi = this->getCgis().begin(); cgi != this->getCgis().end(); cgi++)
	{
		if (cgi->prepareClient4ResponseGeneration(client, requestedURL))	
		{
			std::cerr << "Returning a CGI: " << std::endl;
			std::cerr << "      Cient  URL: " << client.getURL() << std::endl;
			std::cerr << "      Response type: " << ObjectTypesStrings[client.getResponseType()] << std::endl;
			std::cerr << "      Path file: " << client.getPathFile() << std::endl;
			std::cerr << "      Default HTTP response: " << client.getDefaultHttpResponse() << std::endl;
			
			return (true);
		}
	}
	return (false);
}

bool ConfigLocation::getBestLocation( Client &client, Path requestedURL,
	std::list<ConfigLocation>::iterator beginLocation,
	std::list<ConfigLocation>::iterator endLocation
)
{
	std::string requestMethod = client.getMethod();
	std::list<std::string>				locMethods ;
	std::list<ConfigLocation>::iterator	bestLocation = endLocation;
	int									maxDirMatches = 0;
	//std::cerr << "··· getBestLocation " << requestedURL << std::endl;
	while (beginLocation != endLocation)
	{
		//std::cerr << "      Location " << *beginLocation ;
		locMethods = beginLocation->getMethods();
		Path temp = beginLocation->getPath();
		std::cerr << temp.included(requestedURL) << ", "
			<< (temp.size() >= maxDirMatches) << "; "
			<< (std::find(locMethods.begin(), locMethods.end(), requestMethod) != locMethods.end()) <<	std::endl;
		if (temp.included(requestedURL)
			&& temp.size() >= maxDirMatches) 	
		{
			if (locMethods.size() == 0 || std::find(locMethods.begin(), locMethods.end(), requestMethod) != locMethods.end())
			{
				maxDirMatches = temp.size();
				bestLocation = beginLocation;
			}
			else
			{
				client.setDefaultHttpResponse(METHOD_NOT_ALLOWED);
			    client.setResponseType(FILE_OBJ);
				client.setConfigElement(&(*beginLocation));
			}
		}
		beginLocation++ ;
	}
	if (bestLocation == endLocation)
		return false;

	requestedURL.popBegin(bestLocation->size());
	bestLocation->prepareClient4ResponseGeneration(client, requestedURL);
	return (true);
}

const std::string ConfigLocation::getErrorPage(ResponseCodes err) const
{
	Path pathy_pathon;
	
	std::string val = this->_errorPage;
	try
	{
		pathy_pathon = _errorPages.at(err);
	}
	catch (std::out_of_range &e)
	{
		(void)e;
		return (""); 
	}
	pathy_pathon.setIsFile(true);

	return pathy_pathon;
}
std::ostream &operator<<(std::ostream &os, const ConfigLocation &obj)
{
	os << "ConfigLocation (" << obj.getPath() << "): " << std::endl;
	return os;
}


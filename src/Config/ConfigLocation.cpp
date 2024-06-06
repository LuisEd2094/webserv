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
}

ConfigLocation::ConfigLocation( ParsingLocation& obj, ConfigLocation& father)
{
	*this = father;
	// TODO erase the nested elements from the father
	this->_locations.empty();
	this->_cgis.empty();
	this->_inheriting = true;
	this->nestedPrint = 0;

	for (std::map<std::string, std::string>::iterator i = obj.begin(); i != obj.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	this->__elemType__ = obj["__elemType__"]; 
	this->__elemArgument__ = obj["__elemArgument__"]; 
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
ConfigLocation::ConfigLocation( ParsingLocation& obj)
{
	// TODO erase the nested elements from the father
	this->_locations.empty();
	this->_cgis.empty();
	this->_inheriting = true;
	this->nestedPrint = 0;

	for (std::map<std::string, std::string>::iterator i = obj.begin(); i != obj.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	this->__elemType__ = obj["__elemType__"]; 
	this->__elemArgument__ = obj["__elemArgument__"]; 
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
	for (std::list<std::string>::iterator iter = _methods.begin(); iter != _methods.end(); iter++)
		std::cout << YELLOW << "word: " << END << *iter << std::endl;
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
}

void ConfigLocation::setRoot(Path root)
{
	this->_root = root;
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
	std::cout << (std::string) _root << std::endl;
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
	std::cout << ConfigElement::genSpace(recursiveLvl) << "- Location (" << this->__elemArgument__ << ")" << std::endl;
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

std::ostream &operator<<(std::ostream &os, const ConfigLocation &obj)
{
	os << "ConfigLocation: " << std::endl;
	os << "  errorPage: " << obj.getErrorPage() << std::endl;
	return os;
}


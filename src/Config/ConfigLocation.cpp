# include <ConfigLocation.hpp>

ConfigLocation::ConfigLocation(void)
{
	_dirListing = false;
}

ConfigLocation::ConfigLocation( ParsingLocation& obj)
{

	for (std::map<std::string, std::string>::iterator i = obj.begin(); i != obj.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
}

void ConfigLocation::parseKeyVal(std::string key, std::string val)
{
	if (key == "errorPage")
		this->setErrorPage(val);	
	else if (key == "methods")
		this->setMethods(val);	
	else if (key == "redirection")
		this->setRedirection(val);	
	else if (key == "root")
		this->setRoot(val);	
	else if (key == "dirListing")
		this->setDirListing(val);	
	else if (key == "index")
		this->setIndex(val);
	else
		throw ParamError(std::string("Error: key not found.") + std::string(" key:") + key);
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
	//hacer split con iteradores y find
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
	_root = root;
}

std::string ConfigLocation::getRoot(void) const
{
	return (this->_root);
}

void ConfigLocation::setDirListing(std::string dirListing)
{
	//check with Joan
	if (dirListing[0])
		_dirListing = true;
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

std::ostream &operator<<(std::ostream &os,  ConfigLocation &obj)
{
	os << "VirtualServer: " << std::endl;
	os << "  errorPage: " << obj.getErrorPage() << std::endl;
	return os;
}

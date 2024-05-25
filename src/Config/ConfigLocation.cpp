# include <ConfigLocation.hpp>

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
	this->errorPage = inpErrorPage;
}
std::string	ConfigLocation::getErrorPage(void)
{
	return (this->errorPage);
}

void ConfigLocation::setMethods(std::string inpMethods)
{
	//hacer split con iteradores y find
	
}

std::list<std::string>	ConfigLocation::getMethods(void)
{
	return (this->methods);
}



ConfigLocation &ConfigLocation::operator=(ConfigLocation& obj)
{
	return (*this);
}

void ConfigLocation::setErrorPage(std::string error)
{
	this->errorPage = error;
}


std::ostream &operator<<(std::ostream &os,  ConfigLocation &obj)
{
	os << "VirtualServer: " << std::endl;
	os << "  maxbodySize: " << obj.getMaxClientBodySize() << std::endl;
	os << "  errorPage: " << obj.getErrorPage() << std::endl;
	return os;
}

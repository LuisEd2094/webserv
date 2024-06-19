#include "ConfigVirtualServer.hpp"
#include "ConfigLocation.hpp"

ConfigVirtualServer::ConfigVirtualServer( ParsingServer& parsed) : 
	serverNames(), 
	locations()
{
	this->nestedPrint = 0;
	this->errorPage = "";
	this->maxClientBodySize = 30000;//
	this->maxUrlLen = 500;//
	for (std::map<std::string, std::string>::iterator i = parsed.begin(); i != parsed.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	for (std::list<ParsingLocation>::iterator location = parsed.locations.begin();
		location != parsed.locations.end(); location++)
	{
		this->locations.push_back(ConfigLocation(*location, *this));
	}
}

void ConfigVirtualServer::parseKeyVal(std::string key, std::string val)
{
	if (key == "errorPage")
		this->setErrorPage(val);	
	else if (key == "maxClientBodySize")
		this->setMaxClientBodySize(val);
	else if (key == "port")
		this->setPort(val);
	else if (key == "host")
		this->setHost(val);
	else if (key == "serverNames")
		this->setServerNames(val);
	else if (key == "maxUrlLen")
		this->setMaxUrlLen(val);
	else
		throw ParamError(std::string("Error: server config key not found.") + std::string(" key:") + key);
}

ConfigVirtualServer &ConfigVirtualServer::operator=(const ConfigVirtualServer& obj)
{
	this->setMaxClientBodySize(obj.getMaxClientBodySize());
	this->setErrorPage(obj.getErrorPage());
	this->serverNames = obj.serverNames;
	this->locations = obj.locations;	
	this->maxUrlLen = obj.maxUrlLen;	
	return (*this);
}

void ConfigVirtualServer::setHost(const std::string &host)
{
	this->host = host;
	this->setServerNames(this->host);
}

void ConfigVirtualServer::setPort(const std::string &port)
{
	this->port = port;
}

void ConfigVirtualServer::setErrorPage(std::string error)
{
	this->errorPage = error;
}

void ConfigVirtualServer::setMaxClientBodySize(std::string bodySize)
{
	try
	{
		this->setMaxClientBodySize(std::atoi(bodySize.c_str()));
	}
	catch (std::exception& e)
	{
		throw ParamError(std::string("Error on bodySize value: ") + bodySize);
	}
}

void ConfigVirtualServer::setMaxClientBodySize(int bodySize)
{
	if (bodySize <= 0)
	{
		throw ParamError(std::string("body size must be greater than 0"));
	}
	this->maxClientBodySize = bodySize;
}

std::string ConfigVirtualServer::getErrorPage(void) const 
{
	return (this->errorPage);
}

int ConfigVirtualServer::getMaxClientBodySize(void) const 
{
	return (this->maxClientBodySize);
}

void	ConfigVirtualServer::setServerNames(std::string &serverName)
{
    std::istringstream input(serverName);
    std::string word;
    
    while (std::getline(input, word, ' '))
        this->serverNames.push_back(word);
}

std::list<std::string> ConfigVirtualServer::getServerNames(void)
{
	return (this->serverNames);
}

std::string ConfigVirtualServer::getPort(void) const
{
	return (this->port);
}

std::string ConfigVirtualServer::getHost(void) const
{
	return (this->host);
}

//std::ostream &operator<<(std::ostream &os, ConfigVirtualServer &obj)
void ConfigVirtualServer::recursivePrint()
{
	ConfigVirtualServer::recursivePrint(0);
}

void ConfigVirtualServer::recursivePrint(int recursiveLvl)
{
	std::cout << ConfigElement::genSpace(recursiveLvl) << "::: VirtualServer ::: " << std::endl;
	recursiveLvl++;
	std::cout << ConfigElement::genSpace(recursiveLvl) << "ServerNames: " <<  containerToString(this->getServerNames()) << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) << "maxBodySize: " << this->getMaxClientBodySize() << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) << "errorPage: " << this->getErrorPage() << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) << "Locations(" << this->locations.size() << "):" << std::endl;
	std::cout << ConfigElement::genSpace(recursiveLvl) << "maxUrlLen(" << this->maxUrlLen << "):" << std::endl;
	for (std::list<ConfigLocation>::iterator loc = this->locations.begin(); loc != this->locations.end();loc++)
	{
		loc->recursivePrint(recursiveLvl);
	}
}

bool ConfigVirtualServer::prepareClient4ResponseGeneration(Client& client)
{
	std::cout << "::: ConfigVIrtualServer::prepareClient4ResponseGeneration" << std::endl;
	std::cout << "::: " << client.getHost() << std::endl;
	std::list<std::string>::iterator server_name;

	std::cout << RED << "client.getHost():" << END << client.getHost() << std::endl;
	server_name = std::find(this->serverNames.begin(), this->serverNames.end(), client.getHost());
	if (server_name == this->serverNames.end())
	{
		std::cout << YELLOW << "client name should be " << END << client.getHost() << std::endl;
		std::cout << TUR << *this->getLocations().begin() << std::endl;
		
		return (false);
	}

 ////
	//return  ConfigLocation::getBestLocation(client.getURL(), client.getMethod(), 
	Path url(client.getURL());
	std::cout << "len locations" << this->locations.size() << std::endl;
	return  ConfigLocation::getBestLocation(
		client, Path(client.getURL()),
//		client, url,
		this->locations.begin(),
		this->locations.end()
	);
/*
	std::list<ConfigLocation>::iterator bestLocation;
	bestLocation = ConfigLocation::getBestLocation(client.getURL(), client.getMethod(), 
		this->locations.begin(),
		this->locations.end());
	if (bestLocation == this->getLocations().end())
		return (false);
	Path nextURL(client.getURL());
	nextURL.popBegin(bestLocation->size());
	bestLocation->prepareClient4ResponseGeneration(client, nextURL);
	return (true);
*/
	// return (true);

	// if (false)
	// {
	// 	std::list<ConfigLocation>::iterator location;
	// 	std::list<std::string>				locMethods ;
	// 	std::list<ConfigLocation>::iterator	bestLocation = this->locations.end();
	// 	Path								requestedURL = client.getURL();
	// 	int									maxDirMatches = 0;
	// 	for(location = this->locations.begin(); location != this->locations.end(); location++)
	// 	{
	// 		locMethods = location->getMethods();
	// 		if (locMethods.size() == 0)
	// 			continue;
	// 		if (location->getPath().included(requestedURL)
	// 			&& std::find(locMethods.begin(), locMethods.end(), client.getMethod()) != locMethods.end()
	// 			&& location->getPath().size() > maxDirMatches) 	
	// 		{
	// 			maxDirMatches = location->getPath().size();
	// 			bestLocation = location;
	// 		}
	// 	}
	// 	if (bestLocation == this->locations.end())
	// 		return (false);
	// 	return (bestLocation->prepareClient4ResponseGeneration(client));
	// }
	// return (true);
}

std::ostream &operator<<(std::ostream &os, const ConfigVirtualServer &obj)
{
	os << "::: VirtualServer ::: " << std::endl;
	os << "maxBodySize: " << obj.getMaxClientBodySize() << std::endl;
	os << "errorPage: " << obj.getErrorPage() << std::endl;
	os << "Locations: ";
	return os;
}

#include "ConfigVirtualServer.hpp"
#include "ConfigLocation.hpp"

ConfigVirtualServer::ConfigVirtualServer( ParsingServer& parsed) : 
	serverNames(), 
	locations()
{
	this->nestedPrint = 0;
	this->errorPage = "";
	this->maxClientBodySize = 30000;
	for (std::map<std::string, std::string>::iterator i = parsed.begin(); i != parsed.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	for (std::list<ParsingLocation>::iterator location = parsed.locations.begin();
		location != parsed.locations.end(); location++)
	{
		this->locations.push_back(ConfigLocation(*location, *this));
	}

	std::cout << GREEN << "::::: AFTER COPY :::::" << END << std::endl;
	this->recursivePrint();
	std::cout << GREEN << "::::: ENDED :::::" << END << std::endl;
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
	else
		throw ParamError(std::string("Error: server config key not found.") + std::string(" key:") + key);
}

ConfigVirtualServer &ConfigVirtualServer::operator=(const ConfigVirtualServer& obj)
{
	this->setMaxClientBodySize(obj.getMaxClientBodySize());
	this->setErrorPage(obj.getErrorPage());
	this->serverNames = obj.serverNames;
	this->locations = obj.locations;	
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

void ConfigVirtualServer::recursivePrint()
{
	ConfigVirtualServer::recursivePrint(0);
}

void ConfigVirtualServer::recursivePrint(int recursiveLvl)
{
	std::cerr << ConfigElement::genSpace(recursiveLvl) << "- VirtualServer" << std::endl;
	recursiveLvl++;
	for (std::list<ConfigLocation>::iterator loc = this->locations.begin(); loc != this->locations.end();loc++)
	{
		loc->recursivePrint(recursiveLvl);
	}
}

bool ConfigVirtualServer::prepareClient4ResponseGeneration(Client& client)
{
	// std::cerr << "::: ConfigVIrtualServer::prepareClient4ResponseGeneration" << std::endl;
	// std::cerr << "::: " << client.getHost() << std::endl;
	std::list<std::string>::iterator server_name;
	server_name = std::find(this->serverNames.begin(), this->serverNames.end(), client.getHost());
	if (server_name == this->serverNames.end())
	{
		return (false);
	}
	Path url(client.getURL());
	return  ConfigLocation::getBestLocation(
		client, Path(client.getURL()),
		this->locations.begin(),
		this->locations.end()
	);
}

std::ostream &operator<<(std::ostream &os, const ConfigVirtualServer &obj)
{
	os << "::: VirtualServer ::: " << std::endl;
	os << "maxBodySize: " << obj.getMaxClientBodySize() << std::endl;
	os << "errorPage: " << obj.getErrorPage() << std::endl;
	os << "Locations: ";
	return os;
}

#include "ConfigVirtualServer.hpp"
#include "ConfigLocation.hpp"

ConfigVirtualServer::ConfigVirtualServer( ParsingServer& parsed)
{
	this->nestedPrint = 0;
	this->errorPage = "";
	this->maxClientBodySize = 30000;//
	for (std::map<std::string, std::string>::iterator i = parsed.begin(); i != parsed.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
	for (std::list<ParsingLocation>::iterator location = parsed.locations.begin();
		location != parsed.locations.end(); location++)
	{
		this->locations.push_back(ConfigLocation(*location));
	}
}

void ConfigVirtualServer::parseKeyVal(std::string key, std::string val)
{
	if (key == "errorPage")
		this->setErrorPage(val);	
	else if (key == "maxClientBodySize")
		this->setMaxClientBodySize(val);
	else if (key == "host" || key == "port");
	else if (key == "serverNames")
		this->setServerNames(val);
	else
		throw ParamError(std::string("Error: server config key not found.") + std::string(" key:") + key);
}

ConfigVirtualServer &ConfigVirtualServer::operator=(ConfigVirtualServer& obj)
{
	this->setMaxClientBodySize(obj.getMaxClientBodySize());
	this->setErrorPage(obj.getErrorPage());
	return (*this);
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

void	ConfigVirtualServer::setServerNames(std::string serverName)
{
	this->serverNames = ft_split(serverName, ' ');
}

std::list<std::string> ConfigVirtualServer::getServerNames(void)
{
	return (this->serverNames);
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
	for (std::list<ConfigLocation>::iterator loc = this->locations.begin(); loc != this->locations.end();loc++)
	{
		loc->recursivePrint(recursiveLvl);
	}
}

bool ConfigVirtualServer::prepareClient4ResponseGeneration(Client& client)
{
	std::list<std::string>::iterator server_name;
	
	server_name = std::find(this->serverNames.begin(), this->serverNames.end(), client.getHost());
	if (server_name == this->serverNames.end())
		return (false);
	std::list<ConfigLocation>::iterator location;
	for(location = this->locations.begin(); location != this->locations.end(); location++)
	{
		if (location->prepareClient4ResponseGeneration(client))
			break ;
	}
	return (true);
}

std::ostream &operator<<(std::ostream &os, const ConfigVirtualServer &obj)
{
	os << "::: VirtualServer ::: " << std::endl;
	os << "maxBodySize: " << obj.getMaxClientBodySize() << std::endl;
	os << "errorPage: " << obj.getErrorPage() << std::endl;
	os << "Locations: ";
	return os;
}

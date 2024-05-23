#include "../../includes/ConfigVirtualServer.hpp"

ConfigVirtualServer::ConfigVirtualServer( ParsingServer& obj)
{
	this->errorPage = "";
	this->maxClientBodySize = 30000;
	for (std::map<std::string, std::string>::iterator i = obj.begin(); i != obj.end(); i++)
	{
		this->parseKeyVal(i->first, i->second);
	}
}

void ConfigVirtualServer::parseKeyVal(std::string key, std::string val)
{
	if (key == "errorPage")
		this->setErrorPage(val);	
	else if (key == "maxClientBodySize")
		this->setMaxClientBodySize(val);
	else
		throw ParamError(std::string("Error: key not found.") + std::string(" key:") + key);
}

ConfigVirtualServer &ConfigVirtualServer::operator=(ConfigVirtualServer& obj)
{
	this->setMaxClientBodySize(obj.getMaxClientBodySize());
	this->setErrorPage(obj.getErrorPage());
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

std::string ConfigVirtualServer::getErrorPage(void) 
{
	return (this->errorPage);
}

int ConfigVirtualServer::getMaxClientBodySize(void) 
{
	return (this->maxClientBodySize);
}

std::ostream &operator<<(std::ostream &os,  ConfigVirtualServer &obj)
{
	os << "VirtualServer: " << std::endl;
	os << "  maxbodySize: " << obj.getMaxClientBodySize() << std::endl;
	os << "  errorPage: " << obj.getErrorPage() << std::endl;
	//for (std::list<ConfigLocation>)
}

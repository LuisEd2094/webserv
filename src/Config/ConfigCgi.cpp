#include "ConfigCgi.hpp"

ConfigCgi::ConfigCgi(void)
{
    this->setDefaults();
}

ConfigCgi::ConfigCgi(ParsingCgi& obj, const ConfigLocation &parent)
{
	this->location = &parent;
    for (ParsingCgi::iterator cgi = obj.begin(); cgi != obj.end(); cgi++)
    {
        this->parseKeyVal(cgi->first, cgi->second);
    }
    this->extension = this->get__elemArgument__();
}

ConfigCgi::ConfigCgi(const ConfigCgi& obj)
{
    *this = obj;
}

bool ConfigCgi::prepareClient4ResponseGeneration(Client& client, Path &requestedUrl) const
{   
	// << "::: ConigCgi::prepareClient4ResponseGeneration " << this->getExtension() << std::endl;
  
    if (requestedUrl.getExtension() != this->getExtension())
    {
        return (false);
    }
    // << "Appending root: " << this->getRootAsString() << std::endl;
    // << "real root: " << this->getRoot() << std::endl;
    // << "REAL root: " << this->root << std::endl;
    client.setPathFile
    (
        (this->getRootAsString()) +
        static_cast<std::string>(requestedUrl)
    );
    client.setResponseType(CGI_OBJ);
    client.setConfigElement(this);
    if (client.getPathFile().assertFileExists())
        client.setDefaultHttpResponse(OK);
    else
        client.setDefaultHttpResponse(NOT_FOUND);
    client.setExecute(execute);
    return true;
}

void ConfigCgi::parseKeyVal(std::string key, std::string val)
{
    if (key == "__elemType__")
        this->__elemType__ = val;
    else if (key == "__elemArgument__")
        this->__elemArgument__ = val;
    else if (key == "extension:")
        this->setExtension(val);
    else if (key == "metaVar")
        this->setMetaVar(val);
    else if (key == "execute")
        this->setExecute(val);
    else
		throw ParamError(std::string("Error: cgi key not found.") + std::string(" key:") + key);
}

void ConfigCgi::setDefaults()
{
    this->__elemType__ = "";
    this->__elemArgument__ = "";
	this->location = NULL;
}

void ConfigCgi::setExtension(std::string &extension)
{
    this->extension = extension;

}

void ConfigCgi::setMetaVar(std::string &metaVar)
{
    std::list<std::string> split = ft_split<std::list<std::string> >(metaVar, ' ');
    std::list<std::string>::iterator line = split.begin();
    for (; line != split.end(); line++)
    {
        std::list<std::string> tmp = ft_split<std::list<std::string> >(*line, '=');
        this->metaVar[tmp.front()] = tmp.back();
    }
    if (this->metaVar.size() > MAX_METAVAR)
        throw ParamError(std::string("Error: max MetaVars: " + toString(MAX_METAVAR)));


}

void ConfigCgi::setLocation(const ConfigLocation &location)
{
	this->location = &location;
}

void ConfigCgi::setExecute(std::string &execute)
{
    this->execute = execute;
}

std::string ConfigCgi::getExtension(void) const
{
    return (this->extension);
}


const std::map<std::string, std::string> &ConfigCgi::getMetaVar(void) const
{
    return (this->metaVar);
}
const ConfigLocation &ConfigCgi::getLocation(void) const
{
	if (this->location == NULL)
		throw std::exception();
	return *(this->location);
}

Path ConfigCgi::getExecute(void) const
{
    return (this->execute);
}

void ConfigCgi::recursivePrint(int recursiveLvl)
{

	std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "CGI (" << this->get__elemArgument__() << ") [" << this << "]" << std::endl;
    recursiveLvl++;

    std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "parent: " << this->location << std::endl;
    std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "extension: " << this->getExtension() << std::endl;

    std::cerr << ConfigElement::genSpace(recursiveLvl) ;
	std::cerr << "execute: " << this->getExecute() << std::endl;
    std::cerr << ConfigElement::genSpace(recursiveLvl) << "ErrorPages: " << std::endl;
    recursiveLvl++;
    printMap(this->location->getMapErrorPages(), recursiveLvl);
    recursiveLvl--;

    std::cerr << ConfigElement::genSpace(recursiveLvl) ;
    std::cerr << "metaVars(" << this->getMetaVar().size() << "):" << std::endl;
    recursiveLvl++;
    std::map<std::string, std::string>::const_iterator metaVar = this->getMetaVar().begin();
    for (; metaVar != this->getMetaVar().end(); metaVar++)
    {
        std::cerr << ConfigElement::genSpace(recursiveLvl) ;
        std::cerr << metaVar->first << ": " << metaVar->second << std::endl;
    }
}

ConfigCgi &ConfigCgi::operator=( const ConfigCgi& obj)
{
    this->__elemType__ = obj.__elemType__;
    this->__elemArgument__ = obj.__elemArgument__;
    this->extension = obj.extension;
    this->metaVar = obj.metaVar;
    this->execute = obj.execute;
    this->root = obj.root;
    this->location = obj.location;
    return (*this);
}

std::ostream &operator<<(std::ostream &os, const ConfigCgi &cgi)
{
    os << "CGI: " << cgi.get__elemArgument__() ;
    os << "extension: " << cgi.getExtension();
    os << "execute: " << cgi.getExecute();
    cgi.getMetaVar();
    std::map<std::string, std::string>::const_iterator metaVar = cgi.getMetaVar().begin();
    os << "metaVar:";
    for (; metaVar != cgi.getMetaVar().end(); metaVar++)
        os << " " << metaVar->first << ": " << metaVar->second;
    return (os);
}




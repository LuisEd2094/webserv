#include "ConfigCgi.hpp"

ConfigCgi::ConfigCgi(void)
{
    this->setDefaults();
}

ConfigCgi::ConfigCgi(ParsingCgi& obj)
{
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

bool ConfigCgi::prepareClient4ResponseGeneration(Client& client)
{   
	std::cout << "Preparing client on request" << client.getURL() << std::endl;	
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
}

void ConfigCgi::setExtension(std::string &extension)
{
    this->extension = extension;

}

void ConfigCgi::setMetaVar(std::string &metaVar)
{
    std::list<std::string> split = ft_split(metaVar, ' ');
    std::list<std::string>::iterator line = split.begin();
    for (; line != split.end(); line++)
    {
        std::list<std::string> tmp = ft_split(*line, '=');
        this->metaVar[tmp.front()] = tmp.back();
    }
}

void ConfigCgi::setExecute(std::string &execute)
{
    this->execute = execute;
}

std::string ConfigCgi::getExtension(void) const
{
    return (this->extension);
}

Path ConfigCgi::getExecute(void) const
{
    return (this->execute);
}

const std::map<std::string, std::string> &ConfigCgi::getMetaVar(void) const
{
    return (this->metaVar);
}

void ConfigCgi::recursivePrint(int recursiveLvl)
{

	std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "CGI (" << this->get__elemArgument__() << ")" << std::endl;
    recursiveLvl++;
    std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "extension: " << this->getExtension() << std::endl;

    std::cout << ConfigElement::genSpace(recursiveLvl) ;
	std::cout << "execute: " << this->getExecute() << std::endl;
    std::cout << ConfigElement::genSpace(recursiveLvl) ;
    std::cout << "metaVars:";
    recursiveLvl++;
    std::map<std::string, std::string>::const_iterator metaVar = this->getMetaVar().begin();

    std::cout << "len: " << this->getMetaVar().size()<< std::endl;
    for (; metaVar != this->getMetaVar().end(); metaVar++)
    {
        std::cout << ConfigElement::genSpace(recursiveLvl) ;
        std::cout << metaVar->first << ": " << metaVar->second << std::endl;
    }
}

ConfigCgi &ConfigCgi::operator=( const ConfigCgi& obj)
{
    this->__elemType__ = obj.__elemType__;
    this->__elemArgument__ = obj.__elemArgument__;
    this->extension = obj.extension;
    this->metaVar = obj.metaVar;
    this->execute = obj.execute;
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




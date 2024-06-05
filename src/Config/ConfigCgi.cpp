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
}

ConfigCgi::ConfigCgi(const ConfigCgi& obj)
{
    this->__elemArgument__ = obj.__elemArgument__;
    this->__elemType__ = obj.__elemType__;
}

void ConfigCgi::recursivePrint(int recursiveLvl)
{
	std::cout << ConfigElement::genSpace(recursiveLvl) ;
    recursiveLvl++;
    std::cout << "- CGI: " << this->__elemArgument__ << std::endl;

}







void ConfigCgi::parseKeyVal(std::string key, std::string val)
{
    std::cout << "key: " << key;
    std::cout << "  val: " << val;
    if (key == "__elemType__")
        this->__elemType__ = val;
    else if (key == "__elemArgument__")
    {
        std::cout << ":::::::::::::::::::::::::: sdaf" << std::endl;
        this->__elemArgument__ = val;
    }
    else if (key == "smth");
    else
		throw ParamError(std::string("Error: cgi key not found.") + std::string(" key:") + key);
}

void ConfigCgi::setDefaults()
{
    this->__elemType__ = "";
    this->__elemArgument__ = "";
}

std::ostream &operator<<(std::ostream &os, const ConfigCgi &cgi)
{
    os << "CGI: " << cgi.get__elemArgument__() ;
    return (os);
}
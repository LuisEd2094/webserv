#include "Path.hpp"

Path::~Path()
{
}

Path::Path()
{
        //std::cout << "Path defaul constructor called" << std::endl;
        //this->directories = std::list<std::string>();
        this->isRelative = false;
        //std::cout << "Path defaul constructor finished" << std::endl;
}

Path::Path(std::string pathStr)
{
    //std::cout << "Path string constructor called" << std::endl;
    this->isRelative = pathStr[0] == '/';
    this->directories = ft_split(pathStr, '/');
    while (this->normalize());
    //std::cout << "Path string constructor finished" << std::endl;
}

Path::Path(const Path &orig)
{
    //std::cout << "Path copy constructor called" <<std::endl;
    //orig.getIsRelative();
    //std::cout << "------------------->" << std::endl;
    //std::cout << *orig.directories.begin() << std::endl;
    //std::cout << "------------------->" << std::endl;
    this->directories = orig.directories;
    this->isRelative = orig.getIsRelative();
    //std::cout << "Path copy constructor finished" << std::endl;
}

Path &Path::operator=(const Path &orig)
{
    this->directories = orig.directories;
    return *this;
}
void Path::popBegin(int ammount)
{
    std::cout << "          POPING " << ammount << std::endl;
    while (ammount--)
    {
        std::cout << "          pop " << ammount << std::endl;
        this->directories.erase(this->directories.begin());         
    }
    std::cout << "          POPED " << ammount << std::endl;
}
void Path::deleteAndBack(std::list<std::string>::iterator &currFile)
{
    std::list<std::string>::iterator del = currFile--; 
    this->directories.erase(del);
}

Path::operator std::string()
{
    std::list<std::string>::iterator dir;
    std::string result;

    if (!this->isRelative)
        result += "/";
    for (dir = this->directories.begin(); dir != this->directories.end(); dir++)
    {
        result += *dir;
        result += "/";
    }
    return (result);
}

void Path::append(Path tail)
{
    for (std::list<std::string>::iterator te = tail.directories.begin(); te != tail.directories.end(); te++)
            this->directories.push_back(*te);
}

int Path::normalize(void)
{
    std::list<std::string>::iterator currFile = this->directories.begin();
    bool modified = false;

    while (currFile != this->directories.end())
    {
        if (*currFile == "")
        {
            deleteAndBack(currFile);
            modified = true;
        }
        else if (*currFile == ".")
        {
            this->deleteAndBack(currFile);
            modified = true;
        }
        else if (*currFile == "..")
        {
            if (currFile == this->directories.begin())
            {
                throw InvalidPathException();
                return 0;
            }
            this->deleteAndBack(currFile);
            this->deleteAndBack(currFile);
            modified = true;
        }
        currFile++;
    }
    return modified;
}

bool Path::includes(const Path &compPath) const
{
       return compPath.included(*this);
}


bool Path::included(const Path &compPath) const
{
    std::list<std::string>::const_iterator included, includer;

    included =  this->directories.begin();
    includer =  compPath.directories.begin();

    while (included != this->directories.end() && includer != compPath.directories.end())
    {
        if (*included != *includer)
            return (false);
        included++ ; includer++;
    }
    return (included == this->directories.end());
}


std::ostream &operator<<(std::ostream &os, const Path &obj)
{
    os <<  (std::string) (const_cast<Path &>(obj));
    return os;
}


#include "Path.hpp"

Path::~Path()
{
}

Path::Path()
{
}

Path::Path(std::string pathStr)
{
        this->isRelative = pathStr[0] == '/';
        this->directories = ft_split(pathStr, '/');
        while (this->normalize());
}

Path::Path(const Path &orig)
{
    this->directories = orig.directories;
    this->isRelative = orig.getIsRelative();
}

Path &Path::operator=(const Path &orig)
{
    this->directories = orig.directories;
    return *this;
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

// TODO too much variables
    Path &fkk = const_cast<Path &>(obj);
    std::string pathstr = (std::string) fkk; 
    os << pathstr;
    return os;
}


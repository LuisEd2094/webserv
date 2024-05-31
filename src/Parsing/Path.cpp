#include "Path.hpp"

Path::~Path()
{
}

Path::Path()
{
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

Path::Path(std::string pathStr)
{
        this->isRelative = pathStr[0] == '/';
        this->directories = ft_split(pathStr, '/');
        while (this->normalize());
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

void Path::deleteAndBack(std::list<std::string>::iterator &currFile)
{
    std::list<std::string>::iterator del = currFile--; 
    this->directories.erase(del);
}

Path::operator std::string()
{
    std::list<std::string>::iterator begin;
    std::string result;

    if (!this->isRelative)
        result += "/";
    for (begin = this->directories.begin(); begin != this->directories.end(); begin++)
    {
        result += *begin;
        result += "/";
    }
    return (result);
}
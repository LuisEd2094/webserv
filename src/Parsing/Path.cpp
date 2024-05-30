#include "Path.hpp"

Path::~Path()
{
}

Path::Path()
{
}

Path::Path(Path &orig)
{
    this->directories = orig.directories;
}

Path &Path::operator=(Path &orig)
{
    this->directories = orig.directories;
    return *this;
}

Path::Path(std::string pathStr)
{

        this->directories = ft_split(pathStr, '/');
        while (this->normalize());
}

int Path::normalize(void)
{
    std::list<std::string>::iterator currFile = this->directories.begin();
    bool lastEmpty = true;
    bool modified = false;

    while (currFile != this->directories.end())
    {
        if (*currFile == "")
        {
            if (lastEmpty)
                deleteAndBack(currFile);
            modified = lastEmpty;
            lastEmpty = !lastEmpty;
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

/*
void Path::normalize2dot(std::list<std::string>::iterator &currFile)
{
    std::list<std::string>::iterator del = currFile; 
    del--;
    this->directories.erase(del);
    del = currFile--;
    this->directories.erase(del);
    //std::list<std::string>::iterator tmp = currFile--; 
    //this->directories.erase(currFile);
    //currFile = tmp;
    //tmp = currFile--;
    //this->directories.erase(tmp);
}
*/
Path::operator std::string()
{
    std::list<std::string>::iterator begin;
    std::string result;

    for (begin = this->directories.begin(); begin != this->directories.end(); begin++)
    {
        result += "/";
        result += *begin;
    }
    return (result);
}
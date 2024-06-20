#include "Path.hpp"

Path::~Path()
{
}

Path::Path()
{
        //std::cerr << "Path defaul constructor called" << std::endl;
        //this->directories = std::list<std::string>();
        this->isRelative = false;
        this->isFile = false;
        //std::cerr << "Path defaul constructor finished" << std::endl;
}

Path::Path(std::string pathStr)
{
    //std::cerr << "Path string constructor called" << std::endl;
    this->isRelative = pathStr[0] != '/';
    this->isFile = pathStr[pathStr.length()-1] != '/';
    this->directories = ft_split<std::list<std::string> >(pathStr, '/');
    while (this->normalize());
    //std::cerr << "Path string constructor finished" << std::endl;
}

Path::Path(const Path &orig)
{
    *this = orig;
}

Path &Path::operator=(const Path &orig)
{
    this->directories = orig.directories;
    this->isRelative = orig.getIsRelative();
    this->isFile = orig.getIsFile();
    return *this;
}

std::string Path::getExtension(void)
{
    if (this->size() == 0)
        return ("");
    std::string last = this->directories.back();
    std::list<std::string>   splited = ft_split<std::list<std::string> >(last, '.');
    if (splited.size() == 1)  // sdaf/fasd
        return ("");
    if (splited.size() == 2 && splited.front() == "") 
        return ("");
    else 
        return ("." + splited.back());
}

void Path::popBegin(int ammount)
{
    std::cerr << "          POPING " << ammount << std::endl;
    while (ammount-- && this->size())
    {
        std::cerr << "          pop " << ammount << std::endl;
        this->directories.erase(this->directories.begin());         
    }
    std::cerr << "          POPED " << ammount << std::endl;
}

void Path::popBack(int ammount)
{
    std::cerr << "          POPINGB " << ammount << std::endl;
    while (ammount-- && this->size())
    {
        std::cerr << "          popb " << ammount << std::endl;
        this->directories.pop_back();         
    }
    std::cerr << "          POPEDB " << ammount << std::endl;
}
void Path::deleteAndBack(std::list<std::string>::iterator &currFile)
{
    std::list<std::string>::iterator del = currFile--; 
    this->directories.erase(del);
}

Path::operator std::string()
{
    std::list<std::string>::iterator dir;
    std::string result("");
    if (this->isRelative)
        result += "./";
    else
        result += "/";
    for (dir = this->directories.begin(); dir != this->directories.end(); dir++)
    {
        if (dir != this->directories.begin())
            result += "/";
        result += *dir;
    }
    if (!this->isFile && this->directories.size() > 0)
        result += "/";
    return (result);
}

Path::operator std::string() const
{
    return ((std::string) Path(*this));
}

void Path::append(Path tail)
{
    // if (this->isFile)
    //     {
    //     std::cerr << RED << "HEEEYY MDFK appending to file !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << END;
    //     return ;

    //     }
    for (std::list<std::string>::iterator te = tail.directories.begin(); te != tail.directories.end(); te++)
            this->directories.push_back(*te);
    this->setIsFile(tail.getIsFile());
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
            // if (this->size() > 1)
            // {
            //     this->deleteAndBack(currFile);
            //     modified = true;
            // }
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

Path			Path::getFile(void) const
{
    if (this->size() == 0)
        return Path("");
    return this->directories.back();
}
Path			Path::getDir(void) const
{
    if (this->size() == 0)
        return Path("");
    Path ret(*this);
    ret.popBack(1);
    return (ret);
}

bool Path::assertDirExists(void) const
{
    std::cerr << "Checking dir exists: " << *this << std::endl;
    struct stat info;
    std::string relativePathStr = std::string("./") + PATH_TO_C_STR(*this); 
    //int result = stat(this->c_str(), &info);

    
    int result = stat(relativePathStr.c_str(), &info);

    if (result == -1)
    {
        std::cerr << "ERRRORRR" << std::endl;
        return (false);
    }
        std::cerr << "OKKKK" << std::endl;
    return (S_ISDIR(info.st_mode));
}

bool Path::assertFileExists(void) const
{
    std::cerr << GREEN;
    std::cerr << "Checking file exists: " << *this << std::endl;
    std::string relativePathStr = std::string("./") + PATH_TO_C_STR(*this); 
    struct stat info;
    int result = stat(relativePathStr.c_str(), &info);
    std::cerr << "Result: " << result << std::endl;
    std::cerr << END;
    if (result == -1)
        return (false);
    return (S_ISREG(info.st_mode));
}

std::ostream &operator<<(std::ostream &os, const Path &obj)
{
    os <<  (std::string) (const_cast<Path &>(obj));
    return os;
}


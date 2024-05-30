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

    std::cout << TUR << "Holiiiiiii" << END << std::endl;
        this->directories = ft_split(pathStr, '/');
        this->normalize();
}

void Path::normalize(void)
{
    std::list<std::string>::iterator begin = this->directories.begin();

    while (begin != this->directories.end())
    {
        //std::cout << TUR << "looping:" << END << *begin << std::endl;
        //std::cout << TUR << *begin <<  END << std::endl;
        if (*begin == ".")
            this->normalize1dot(begin);
        if (*begin == "..")
        {
            this->normalize2dot(begin);
        }
        begin++;
    }
}

void Path::normalize1dot(std::list<std::string>::iterator &curFile)
{
    this->directories.erase(curFile);
}

void Path::normalize2dot(std::list<std::string>::iterator &begin)
{
    std::list<std::string>::iterator tmp = begin--; 
    this->directories.erase(begin);
    begin = tmp;
    tmp = begin--;
    this->directories.erase(tmp);
}

Path::operator std::string()
{
    std::list<std::string>::iterator begin;
    std::string result;


    std::cout << RED << "size: " <<  this->directories.size() << END << std:: endl;
    for (begin = this->directories.begin(); begin != this->directories.end(); begin++)
    {
        result += "/";
        result += *begin;
        std::cout << YELLOW << "result: " << END <<  result << std:: endl;
    }
    std::cout << BLUE << "result: " <<  END << result << std:: endl;
    return (result);
}
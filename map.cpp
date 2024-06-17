#include <iostream>
#include <map>
#include <list>
#include <sstream>
#include <sys/stat.h>

std::list<std::string> ft_split(const std::string& str, char delimiter)
{
    std::list<std::string> list;
    std::istringstream input(str);
    std::string word;
    
    while (std::getline(input, word, delimiter))
    {
        if (!word.empty())
            list.push_back(word);
    }
    return (list);
}

typedef struct deleted_files
{
    bool is_dir;
    std::map<std::string, deleted_files> files;
    void add_to_delete(const std::string&);
}   deleted_files;

void deleted_files::add_to_delete(const std::string& path)
{    
    std::list<std::string> split = ft_split(path, '/');
    std::list<std::string>::iterator it = split.begin();
    deleted_files *temp = this; 
    struct stat info;
    int result = stat(path.c_str(), &info);


    for (; it != split.end(); it++)
    {
        if (temp->files.find(*it) == temp->files.end()) {
            // Create a new entry if it doesn't exist
            temp->files[*it] = deleted_files();
        }
        // Move to the next level in the hierarchy
        temp = &(temp->files[*it]);
        std::cout << *it << std::endl;
    }
    (*temp).is_dir = S_ISDIR(info.st_mode);

    std::cout<< (*temp).is_dir << std::endl;
}

int main( int argc, char * argv[])
{
    std::string path;
    deleted_files deleted;
    if (argc > 1)
    {
        path = argv[1];
    }
    else
    {
        path = "/home/luis/proyects/webserv/";
    }
    struct stat info;
    int result = stat(path.c_str(), &info);
    std::list<std::string> split = ft_split(path, '/');

    std::list<std::string>::iterator it = split.begin();
    for (; it != split.end(); it++)
    {
        std::cout << *it << std::endl;
    }
    deleted.add_to_delete(path);
    deleted.add_to_delete(path);
    deleted.add_to_delete(path + "TODO.md");
    deleted.add_to_delete(path + "Another.md");
    deleted.add_to_delete("home/bin/Another.md");

    std::cout << path << std::endl;
}
#include <iostream>
#include <map>
#include <sstream>
#include <sys/stat.h>

typedef struct deletedFiles
{
    bool is_dir;
    std::map<std::string, deletedFiles> files;
    void addToDelete(const std::string&);
    void removeFromDeleted(const std::string&);
    bool checkIfDeleted(const std::string&);
    deletedFiles();
}   deletedFiles;


deletedFiles::deletedFiles() : is_dir(0) {}

void deletedFiles::removeFromDeleted(const std::string& path)
{
    deletedFiles *temp = this;
    deletedFiles *last_found = temp;
    std::string     last_word;
    std::istringstream input(path);
    std::string word; 


    while (std::getline(input, word, '/'))
    {
        /*getLine doesn't skip the first word if the string starts with /*/
        if (word.empty())
            continue;
        if (temp->files.find(word) == temp->files.end())
            return;
        if (temp->files.size() > 1)
        {
            last_found = temp;
            last_word = word;
        }
        temp = &(temp->files[word]);
    }
    if (last_found->files[last_word].is_dir)
    {
        last_found->files[last_word].is_dir = false;
    }
    else
    {
        last_found->files.erase(last_word);
    }

}

bool deletedFiles::checkIfDeleted(const std::string& path)
{
    deletedFiles *temp = this;
    std::istringstream input(path);
    std::string word; 

    while (std::getline(input, word, '/'))
    {
        /*getLine doesn't skip the first word if the string starts with /*/
        if (word.empty())
            continue;
        if (temp->files.find(word) == temp->files.end())
            return (temp->is_dir);
        temp = &(temp->files[word]);
    }
    return temp->is_dir || temp->files.empty();
}

void deletedFiles::addToDelete(const std::string& path)
{    
    deletedFiles *temp = this; 
    struct stat info;
    int result = stat(path.c_str(), &info);

    std::istringstream input(path);
    std::string word;


    while (std::getline(input, word, '/'))
    {
        if (word.empty())
            continue;
        if (temp->files.find(word) == temp->files.end()) {
            // Create a new entry if it doesn't exist
            temp->files[word] = deletedFiles();
        }
        // Move to the next level in the hierarchy
        temp = &(temp->files[word]);
        std::cout << word << std::endl;
    }
    (*temp).is_dir = S_ISDIR(info.st_mode);
}

int main( int argc, char * argv[])
{
    std::string path;
    deletedFiles deleted;
    if (argc > 2)
    {
        path = argv[1];
    }
    else
    {
        path = "/workspaces/webserv/";
    }

    deleted.addToDelete(path);
    deleted.addToDelete(path);
    deleted.addToDelete(path + "TODO.md");
    deleted.addToDelete(path + "Another.md");
    deleted.addToDelete("home/bin/Another.md");
    deleted.addToDelete("home/bin/thisisAnother.md");
    deleted.addToDelete("home/notbin/Another.md");


    std::cout << deleted.checkIfDeleted("home/bin/Another.md") << " SHOULD BE 1 " << std::endl;
    std::cout << deleted.checkIfDeleted("home/bin/thisisAnother.md") << " SHOULD BE 1 "  << std::endl;
    std::cout << deleted.checkIfDeleted("home/notbin/Another.md")<< " SHOULD BE 1 " << std::endl;

    deleted.removeFromDeleted("home/bin/Another.md");

    std::cout << deleted.checkIfDeleted("home/bin/Another.md") << " SHOULD BE 0 " << std::endl;
    std::cout << deleted.checkIfDeleted("home/bin/thisisAnother.md") << " SHOULD BE 1 "  << std::endl;
    std::cout << deleted.checkIfDeleted("home/notbin/Another.md")<< " SHOULD BE 1 " << std::endl;

    deleted.removeFromDeleted("home/bin/thisisAnother.md");

    std::cout << deleted.checkIfDeleted("home/bin/Another.md") << " SHOULD BE 0 " << std::endl;
    std::cout << deleted.checkIfDeleted("home/bin/thisisAnother.md") << " SHOULD BE 0 "  << std::endl;
    std::cout << deleted.checkIfDeleted("home/bin/") << " SHOULD BE 0 "  << std::endl;

    std::cout << deleted.checkIfDeleted("home/notbin/Another.md")<< " SHOULD BE 1 " << std::endl;

    std::cout << deleted.checkIfDeleted(path) << " SHOULD BE 1 " << std::endl;
    std::cout << deleted.checkIfDeleted(path + "TODO.md") << " SHOULD BE 1 "  << std::endl;
    std::cout << deleted.checkIfDeleted(path + "notdeletedbutparentwas")<< " SHOULD BE 1 " << std::endl;
    std::cout << deleted.checkIfDeleted(std::string("/this/is/anew/path")) << " SHOULD BE 0 " << std::endl;
    std::cout << deleted.checkIfDeleted(std::string("home/is/anew/path")) << " SHOULD BE 0 " << std::endl;

    std::cout << deleted.checkIfDeleted(path) << " SHOULD BE 1 " << std::endl;
    deleted.removeFromDeleted(path);
    deleted.removeFromDeleted("/this/was/never/deleted");

    std::cout << deleted.checkIfDeleted(path) << " SHOULD BE 0 " << std::endl;
    std::cout << deleted.checkIfDeleted(path + "TODO.md") << " SHOULD BE 1 "  << std::endl;
    deleted.removeFromDeleted(path + "TODO.md");
    std::cout << deleted.checkIfDeleted(path + "TODO.md") << " SHOULD BE 0 "  << std::endl;
    deleted.removeFromDeleted(path + "Another.md");
    std::cout << deleted.checkIfDeleted(path + "TODO.md") << " SHOULD BE 0 "  << std::endl;
    std::cout << deleted.checkIfDeleted(path + "Another.md") << " SHOULD BE 0 "  << std::endl;
    std::cout << deleted.checkIfDeleted(path) << " SHOULD BE 0 "  << std::endl;

    std::cout << deleted.checkIfDeleted(path + "notdeletedbutparentwas")<< " SHOULD BE 0 " << std::endl;

    std::cout << path << std::endl;
}
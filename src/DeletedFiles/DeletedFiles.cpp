#include <DeletedFiles.hpp>

deletedFiles::deletedFiles() : is_dir(0) {}

bool deletedFiles::removeFromDeleted(const std::string& path)
{
    deletedFiles *temp = this;
    deletedFiles *last_found = temp;
    std::string     last_word("");
    std::istringstream input(path);
    std::string word; 

    while (std::getline(input, word, '/'))
    {
        if (last_word.empty())
            last_word = word;
        /*getLine doesn't skip the first word if the string starts with /*/
        if (word.empty())
            continue;
        if (temp->files.find(word) == temp->files.end())
            return false;
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
    return true;

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

bool deletedFiles::addToDelete(const std::string& path)
{    
    deletedFiles *temp = this; 
    struct stat info;
    int result = stat(path.c_str(), &info);

    if (result == -1)
        return (false);

    std::istringstream input(path);
    std::string word;


    while (std::getline(input, word, '/'))
    {
        if (word.empty())
            continue;
        if (temp->files.find(word) == temp->files.end()) {
            temp->files[word] = deletedFiles();
        }
        temp = &(temp->files[word]);
    }
    (*temp).is_dir = S_ISDIR(info.st_mode);
    return (true);
}

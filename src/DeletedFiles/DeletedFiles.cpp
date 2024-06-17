#include <DeletedFiles.hpp>

deletedFiles::deletedFiles() : is_dir(0) {}

bool deletedFiles::removeFromDeleted(const std::string& path)
{
    deletedFiles *temp = this;
    deletedFiles *last_found;
    std::string     last_word;
    std::istringstream input(path);
    std::string word; 

    while (std::getline(input, word, '/'))
    {
        /*getLine doesn't skip the first word if the string starts with /*/
        if (word.empty())
            continue;
        if (temp->files.find(word) == temp->files.end())
            return false;
        last_found = temp;
        last_word = word;
        temp = &(temp->files[word]);
    }
    if (last_found->files[last_word].files.empty())
    {
        last_found->files.erase(last_word);
    }
    else
    {
        last_found->files[last_word].is_dir = false;
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
            // Create a new entry if it doesn't exist
            temp->files[word] = deletedFiles();
        }
        // Move to the next level in the hierarchy
        temp = &(temp->files[word]);
        std::cout << word << std::endl;
    }
    (*temp).is_dir = S_ISDIR(info.st_mode);
    return (true);
}
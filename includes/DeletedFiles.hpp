#ifndef DELETEDFILES_HPP
# define DELETEDFILES_HPP

# include <iostream>
# include <map>
# include <sstream>
# include <sys/stat.h>

typedef struct deletedFiles
{
    bool is_dir;
    std::map<std::string, deletedFiles> files;
    bool addToDelete(const std::string&);
    bool removeFromDeleted(const std::string&);
    bool checkIfDeleted(const std::string&);
    deletedFiles();
}   deletedFiles;

#endif
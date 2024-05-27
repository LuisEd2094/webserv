#ifndef AUX_HPP
# define AUX_HPP
# include <iostream>
# include <cstdlib>
# include <map>


void exitError(const std::string& error);

enum Actions {
    WAIT,
    GET,
    POST, 
    DELETE
};



#endif

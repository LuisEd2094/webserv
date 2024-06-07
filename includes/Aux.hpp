#ifndef AUX_HPP
# define AUX_HPP
# include <iostream>
# include <cstdlib>
# include <map>
# include <sstream>


# define CONTENTLENGTH "Content-Length: "
# define CRNL "\r\n"

template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template<typename T>
std::size_t hexStringToSizeT(const T& value)
{
    std::size_t x;

    std::stringstream ss;
    ss << std::hex << value;
    ss >> x;
    return x;
}

void exitError(const std::string& error);
std::string setContentLenHTTP(const std::string& http, const std::string& body);
std::string addCRNL(std::string&  http);

enum Actions {
    WAIT,
    GET,
    POST, 
    DELETE
};



#endif

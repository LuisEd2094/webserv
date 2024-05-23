#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <sstream>



# define OK             "HTTP/1.1 200 OK\r\n"
# define NOT_FOUND      "HTTP/1.1 404 Not Found\r\n"
# define INTERNAL_ERROR "HTTP/1.1 500 Internal Server Error\r\n"
# define CONTENTLENGTH "Content-Length: "
# define CRNL "\r\n"


template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif

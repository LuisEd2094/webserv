#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <sstream>



# define OK             "HTTP/1.1 200 OK\r\n"
# define NOT_FOUND      "HTTP/1.1 404 Not Found\r\n"
# define BODY_NOT_FOUND "<!DOCTYPE html><html>\n<head><title>404 Not Found</title></head>\n<body>\n<h1>Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>"
# define TIMEOUT_408    "HTTP/1.1 408 Request Timeout\r\nConnection: close\r\n"
# define TIMEOUT_BODY   "<!DOCTYPE html><html>\n<head><title>408 Request Timeout/title></head>\n<body>\n<h1>Request Timeout</h1>\n  <p>The server did not receive a complete request within the specified timeout period.</p>\n</body>\n</html>\n"


# define INTERNAL_ERROR "HTTP/1.1 500 Internal Server Error\r\n"
# define CONTENTLENGTH "Content-Length: "
# define CRNL "\r\n"

std::string generateHTTP(const std::string& http, const std::string& body);


template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif

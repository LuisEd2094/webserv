#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <sstream>
# include <map>





# define HTTP_OK        "HTTP/1.1 200 HTTP_OK\r\n"

# define NOT_FOUND_HTTP "HTTP/1.1 404 Not Found\r\n"
# define NOT_FOUND_TILE "404 Not Found"
# define NOT_FOUND_BODY "The requested URL was not found on this server."
 
# define TIMEOUT_HTTP   "HTTP/1.1 408 Request Timeout\r\nConnection: close\r\n"
# define TIMEOUT_TITLE  "408 Request Timeout"
# define TIMEOUT_BODY   "The server did not receive a complete request within the specified timeout period."


# define TEMPLATE "<!DOCTYPE html><html>\n<head><title></title></head>\n<body>\n<p></p>\n</body>\n</html>\n"

# define INTERNAL_ERROR_HTTP    "HTTP/1.1 500 Internal Server Error\r\n"
# define INTERNAL_ERROR_TITLE  "500 Internal Server Error"
# define INTERNAL_ERROR_BODY   "The server ran into a problem while executing your request."

# define CONTENTLENGTH "Content-Length: "
# define CRNL "\r\n"


enum Codes
{
    OK,
    REQUEST_TIMEOUT,
    NOT_FOUND,
    INTERNAL_SERVER_ERROR 
};

typedef struct Responses
{
    std::string _http;
    std::string _title;
    std::string _body;
    Responses(const std::string&, const std::string&, const std::string&);

} Responses;


typedef std::map<Codes, Responses > responsesMap;


typedef struct fullResponse
{
    std::string _http;
    std::string _body;
    fullResponse(const Responses&);
} fullResponse;

typedef std::map<Codes, fullResponse > fullResponsesMap; 

// One to create HTTP and add size from body
// One to create both HTTP and BODY if none is provided in config

class Response
{
    public:
        static void initDefaultMap();
        static const fullResponse& getDefault(Codes code, const std::string& body);
        static const fullResponse& getDefault(Codes code);

    private:
        static void createFullResponses();
        static responsesMap defaults;
        static fullResponsesMap fullResponses;
};

std::string setContentLenHTTP(const std::string& http, const std::string& body);


template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <sstream>
# include <map>





# define HTTP_OK        "HTTP/1.1 200 HTTP_OK\r\n"

# define NOT_FOUND      "HTTP/1.1 404 Not Found\r\n"
# define BODY_NOT_FOUND "<!DOCTYPE html><html>\n<head><title>404 Not Found</title></head>\n<body>\n<h1>Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>"
 
# define TIMEOUT_HTTP   "HTTP/1.1 408 Request Timeout\r\nConnection: close\r\n"
# define TIMEOUT_TITLE  "408 Request Timeout"
# define TIMEOUT_BODY   "The server did not receive a complete request within the specified timeout period."


# define TEMPLATE "<!DOCTYPE html><html>\n<head><title></title></head>\n<body>\n<p></p>\n</body>\n</html>\n"

# define INTERNAL_ERROR "HTTP/1.1 500 Internal Server Error\r\n"
# define CONTENTLENGTH "Content-Length: "
# define CRNL "\r\n"


enum Codes
{
    OK,
    REQUEST_TIMEOUT 
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
    fullResponse(const std::string&, const std::string&);
    fullResponse(const Responses&);
} fullResponse;

typedef std::map<Codes, fullResponse > fullResponsesMap; 

// One to create HTTP and add size from body
// One to create both HTTP and BODY if none is provided in config

class Response
{
    public:
        static void initDefaultMap();
        static const fullResponse& getDefault(Codes code);
        // static fullResponse* getDefault(Codes code, const std::string& body);


    private:
        static void createFullResponses();
        static responsesMap defaults;

        static fullResponsesMap fullResponses;
};

std::string generateHTTP(const std::string& http, const std::string& body);


template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#endif

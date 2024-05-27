#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <sstream>
# include <map>
# include <cstring>



enum ErrorCodes
{
    OK,
    BAD_REQUEST,
    FORBIDDEN,
    NOT_FOUND,
    METHOD_NOT_ALLOWED,
    NOT_ACCEPTABLE,
    REQUEST_TIMEOUT,
    CONFLICT,
    GONE,
    LENGTH_REQUIRED,
    PAYLOAD,
    URI_TOO_LONG,
    MEDIA_TYPE,
    EXPECTATION,
    UPGRADE,


    INTERNAL_SERVER_ERROR,
    GATEWAY_TIMEOUT 
};

# define HTTP_OK        "HTTP/1.1 200 HTTP_OK\r\n"

/*4xx ERRORS*/


# define BAD_REQUEST_HTTP   "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n"
# define BAD_REQUEST_TITLE  "400 Bad Request"
# define BAD_REQUEST_BODY   "Your request cannot be processed. 👎"

# define FORBIDDEN_HTTP   "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\n"
# define FORBIDDEN_TITLE  "403 Forbidden"
# define FORBIDDEN_BODY   "You do not have permission to access this resource. 👎"

# define NOT_FOUND_HTTP "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n"
# define NOT_FOUND_TILE "404 Not Found"
# define NOT_FOUND_BODY "The requested URL was not found on this server.👎"
 

# define METHOD_HTTP   "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\n"
# define METHOD_TITLE  "405 Method Not Allowed"
# define METHOD_BODY   "The requested method is not allowed for the requested resource.👎"

/*What happens if they don't accept html as a response?*/
# define NOT_ACCEPTABLE_HTTP   "HTTP/1.1 406 Not Acceptable\r\nContent-Type: text/html\r\n"
# define NOT_ACCEPTABLE_TITLE  "406 Not Acceptable"
# define NOT_ACCEPTABLE_BODY   "The server cannot generate a response that is acceptable to the client.👎"

# define TIMEOUT_HTTP   "HTTP/1.1 408 Request Timeout\r\nContent-Type: text/html\r\nConnection: close\r\n"
# define TIMEOUT_TITLE  "408 Request Timeout"
# define TIMEOUT_BODY   "The server did not receive a complete request within the specified timeout period.👎"

# define CONFLICT_HTTP      "HTTP/1.1 409 Conflict\r\nContent-Type: text/html\r\n"
# define CONFLICT_TITLE     "409 Conflict"
# define CONFLICT_BODY      "The request could not be completed due to a conflict with the current state of the resource.👎"


# define GONE_HTTP      "HTTP/1.1 410 Gone\r\nContent-Type: text/html\r\n"
# define GONE_TITLE     "410 Gone"
# define GONE_BODY      "The resource you are looking for has been permanently removed.👎"

# define LENGTH_HTTP      "HTTP/1.1 411 Length Required\r\nContent-Type: text/html\r\n"
# define LENGTH_TITLE     "411 Length Required"
# define LENGTH_BODY      "The server cannot process the request because it lacks a valid Content-Length header.👎"

# define PAYLOAD_HTTP      "HTTP/1.1 413 Payload Too Large\r\nContent-Type: text/html\r\n"
# define PAYLOAD_TITLE     "413 Payload Too Large"
# define PAYLOAD_BODY      "The request entity is too large for the server to process.👎"

# define URI_TOO_LONG_HTTP      "HTTP/1.1 414 URI Too Long\r\nContent-Type: text/html\r\n"
# define URI_TOO_LONG_TITLE     "414 URI Too Long"
# define URI_TOO_LONG_BODY      "The URI requested by the client is longer than the server is willing to interpret.👎"

# define MEDIA_HTTP      "HTTP/1.1 415 Unsupported Media Type\r\nContent-Type: text/html\r\n"
# define MEDIA_TITLE     "415 Unsupported Media Type"
# define MEDIA_BODY      "The server refuses to accept the request because the payload format is in an unsupported format.👎"

# define MEDIA_HTTP      "HTTP/1.1 415 Unsupported Media Type\r\nContent-Type: text/html\r\n"
# define MEDIA_TITLE     "415 Unsupported Media Type"
# define MEDIA_BODY      "The server refuses to accept the request because the payload format is in an unsupported format.👎"


# define EXPECT_HTTP      "HTTP/1.1 417 Expectation Failed\r\nContent-Type: text/html\r\n"
# define EXPECT_TITLE     "417 Expectation Failed"
# define EXPECT_BODY      "The server cannot meet the requirements of the Expect request-header field.👎"

# define UPGRADE_HTTP      "HTTP/1.1 426 Upgrade Required\r\nUpgrade: HTTP/1.1\r\nContent-Type: text/html\r\n"
# define UPGRADE_TITLE     "426 Upgrade Required"
# define UPGRADE_BODY      "The server refuses to process the request using the current protocol.👎"



/*5xx ERRORS*/

# define INTERNAL_ERROR_HTTP    "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n"
# define INTERNAL_ERROR_TITLE  "500 Internal Server Error"
# define INTERNAL_ERROR_BODY   "The server ran into a problem while executing your request. 💀"

# define GATEWAY_TIMEOUT_HTTP   "HTTP/1.1 504 Gateway Timeout\r\nContent-Type: text/html\r\n"
# define GATEWAY_TIMEOUT_TITLE  "504 Gateway Timeout"
# define GATEWAY_TIMEOUT_BODY   "The server, acting as a gateway or proxy, did not receive a timely response from the upstream server. 💀"


# define TEMPLATE "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n<title></title>\n</head>\n<body>\n<p></p>\n</body>\n</html>\n"
# define CONTENTLENGTH "Content-Length: "
# define CRNL "\r\n"




typedef struct Responses
{
    std::string _http;
    std::string _title;
    std::string _body;
    Responses(const std::string&, const std::string&, const std::string&);

} Responses;


typedef std::map<ErrorCodes, Responses > responsesMap;


typedef struct fullResponse
{
    std::string _http;
    std::string _body;
    fullResponse(const Responses&);
} fullResponse;

typedef std::map<ErrorCodes, fullResponse > fullResponsesMap; 

// One to create HTTP and add size from body
// One to create both HTTP and BODY if none is provided in config

class Response
{
    public:
        static void initDefaultMap();
        static const fullResponse& getDefault(ErrorCodes code, const std::string& body);
        static const fullResponse& getDefault(ErrorCodes code);

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

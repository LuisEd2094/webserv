#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <map>
# include <cstring>
# include <Aux.hpp>
# include <vector>

enum RedirectionCodes
{
    /* 3xx */
    MULTIPLE_CHOICES, //300
    MOVED_PERMANENTLY,
    FOUND,
    SEE_OTHER,
    USE_PROXY,
    TEMPORARY_REDIRECT
};

enum ResponseCodes
{
    INVALID_CODE,
    /*1xx*/
    CONTINUE,
    /*2xx*/
    OK,
    /*3xx*/
    MULTIPLE_REDIRECTS,
    /*4xx*/
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

    /*5xx*/
    INTERNAL_SERVER_ERROR,
    NOT_IMPLEMENTED,
    BAD_GATEWAY,
    SERVICE_UNAVAILABLE,
    GATEWAY_TIMEOUT, 
    VERSION_NOT_SUPPORTED
};

# define IS_ERROR_CODE(code) code >= BAD_REQUEST && code <= VERSION_NOT_SUPPORTED

/* 1xx Others*/

# define CONTINUE_HTTP        "HTTP/1.1 100 Continue\r\n\r\n"


/* 2xx OK*/

# define HTTP_OK               "HTTP/1.1 200 OK\r\n"

/* 3xx ERRORS*/

# define MULTIPLE_REDIRECTS_HTTP "HTTP/1.1 300 Multiple Choices\r\nContent-Type: text/html\r\n"

/*4xx ERRORS*/


# define BAD_REQUEST_HTTP   "HTTP/1.1 400 Bad Request\r\n"
# define BAD_REQUEST_TITLE  "400 Bad Request"
# define BAD_REQUEST_BODY   "Your request cannot be processed. 👎"

# define FORBIDDEN_HTTP   "HTTP/1.1 403 Forbidden\r\n"
# define FORBIDDEN_TITLE  "403 Forbidden"
# define FORBIDDEN_BODY   "You do not have permission to access this resource. 👎"

# define NOT_FOUND_HTTP "HTTP/1.1 404 Not Found\r\n"
# define NOT_FOUND_TILE "404 Not Found"
# define NOT_FOUND_BODY "The requested URL was not found on this server.👎"
 

# define METHOD_HTTP   "HTTP/1.1 405 Method Not Allowed\r\n"
# define METHOD_TITLE  "405 Method Not Allowed"
# define METHOD_BODY   "The requested method is not allowed for the requested resource.👎"

/*What happens if they don't accept html as a response?*/
# define NOT_ACCEPTABLE_HTTP   "HTTP/1.1 406 Not Acceptable\r\n"
# define NOT_ACCEPTABLE_TITLE  "406 Not Acceptable"
# define NOT_ACCEPTABLE_BODY   "The server cannot generate a response that is acceptable to the client.👎"

# define TIMEOUT_HTTP   "HTTP/1.1 408 Request Timeout\r\nConnection: close\r\n"
# define TIMEOUT_TITLE  "408 Request Timeout"
# define TIMEOUT_BODY   "The server did not receive a complete request within the specified timeout period.👎"

# define CONFLICT_HTTP      "HTTP/1.1 409 Conflict\r\n"
# define CONFLICT_TITLE     "409 Conflict"
# define CONFLICT_BODY      "The request could not be completed due to a conflict with the current state of the resource.👎"


# define GONE_HTTP      "HTTP/1.1 410 Gone\r\n"
# define GONE_TITLE     "410 Gone"
# define GONE_BODY      "The resource you are looking for has been permanently removed.👎"

# define LENGTH_HTTP      "HTTP/1.1 411 Length Required\r\n"
# define LENGTH_TITLE     "411 Length Required"
# define LENGTH_BODY      "The server cannot process the request because it lacks a valid Content-Length header.👎"

# define PAYLOAD_HTTP      "HTTP/1.1 413 Payload Too Large\r\n"
# define PAYLOAD_TITLE     "413 Payload Too Large"
# define PAYLOAD_BODY      "The request entity is too large for the server to process.👎"

# define URI_TOO_LONG_HTTP      "HTTP/1.1 414 URI Too Long\r\n"
# define URI_TOO_LONG_TITLE     "414 URI Too Long"
# define URI_TOO_LONG_BODY      "The URI requested by the client is longer than the server is willing to interpret.👎"

# define MEDIA_HTTP      "HTTP/1.1 415 Unsupported Media Type\r\n"
# define MEDIA_TITLE     "415 Unsupported Media Type"
# define MEDIA_BODY      "The server refuses to accept the request because the payload format is in an unsupported format.👎"

# define EXPECT_HTTP      "HTTP/1.1 417 Expectation Failed\r\n"
# define EXPECT_TITLE     "417 Expectation Failed"
# define EXPECT_BODY      "The server cannot meet the requirements of the Expect request-header field.👎"

# define UPGRADE_HTTP      "HTTP/1.1 426 Upgrade Required\r\nUpgrade: HTTP/1.1\r\n"
# define UPGRADE_TITLE     "426 Upgrade Required"
# define UPGRADE_BODY      "The server refuses to process the request using the current protocol.👎"



/*5xx ERRORS*/

# define INTERNAL_ERROR_HTTP    "HTTP/1.1 500 Internal Server Error\r\n"
# define INTERNAL_ERROR_TITLE  "500 Internal Server Error"
# define INTERNAL_ERROR_BODY   "The server ran into a problem while executing your request. 💀"


# define NOT_IMPLEMENTED_HTTP   "HTTP/1.1 501 Not Implemented\r\nConnection: close\r\n"
# define NOT_IMPLEMENTED_TITLE  "501 Not Implemented"
# define NOT_IMPLEMENTED_BODY   "The server does not support this method. 💀"

# define BAD_GATEWAY_HTTP   "HTTP/1.1 502 Bad Gateway\r\n"
# define BAD_GATEWAY_TITLE  "502 Bad Gateway"
# define BAD_GATEWAY_BODY   "We encountered an error from the Gateway."

# define SERVICE_UNAVAILABLE_HTTP   "HTTP/1.1 503 Service Unavailable\r\n"
# define SERVICE_UNAVAILABLE_TITLE  "503 Bad Service Unavailable"
# define SERVICE_UNAVAILABLE_BODY   "We are currently not available, try again later."


# define GATEWAY_TIMEOUT_HTTP   "HTTP/1.1 504 Gateway Timeout\r\n"
# define GATEWAY_TIMEOUT_TITLE  "504 Gateway Timeout"
# define GATEWAY_TIMEOUT_BODY   "The server, acting as a gateway or proxy, did not receive a timely response from the upstream server. 💀"

# define VERSION_NOT_SUPPORTED_HTTP   "HTTP/1.1 505 HTTP Version Not Supported\r\n"
# define VERSION_NOT_SUPPORTED_TITLE  "505 HTTP Version Not Supported"
# define VERSION_NOT_SUPPORTED_BODY   "This server only supports HTTP 1.1, please make sure your request are in the correct version."





# define TEMPLATE "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n<title></title>\n</head>\n<body>\n<p></p>\n</body>\n</html>\n"


# define REDIRECT_TEMPLATE "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n<title>\nRedirecting...</title>\n</head>\n<body></body></html>"


typedef struct Responses
{
    std::string _http;
    std::string _title;
    std::string _body;
    Responses(const std::string&, const std::string&, const std::string&);

} Responses;


typedef std::map<ResponseCodes, Responses > responsesMap;


typedef struct defaultResponse
{
    std::string _http;
    std::string _body;
    defaultResponse(const Responses&);
} defaultResponse;

typedef std::map<ResponseCodes, defaultResponse > defaultResponsesMap; 

// One to create HTTP and add size from body
// One to create both HTTP and BODY if none is provided in config

class Response
{
    public:
        static void initDefaultMap();
        static const defaultResponse& getDefault(ResponseCodes code, const std::string& body);
        static const defaultResponse& getDefault(ResponseCodes code);
        static const std::string& getHttpFirtsLine(ResponseCodes code);
        static void initErrorsHttp();
        static void initErrorsMap();
        static  ResponseCodes getErrorCodeFromInt(int);

    private:
        static void createDefaultResponses();
        static responsesMap defaults;
        static defaultResponsesMap defaultResponses;
        static std::vector<std::string>    _vectorError;
        static std::map<int, ResponseCodes>    _mapError;

};





#endif

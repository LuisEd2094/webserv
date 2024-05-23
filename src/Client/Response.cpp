#include <ProgramConfigs.hpp>

std::string generateHTTP(const std::string& http, const std::string& body)
{
    /*Checks body and generates Content-Len, can be used to generate other header lines in the future*/
    std::string new_http(http);
    if (!body.empty())
    {
        new_http.append(CONTENTLENGTH + toString(body.length()) + CRNL);
    }
    new_http.append(CRNL);
    return new_http;   
}

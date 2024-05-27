#include <ProgramConfigs.hpp>
responsesMap Response::defaults;
fullResponsesMap Response::fullResponses;


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

// fullResponse* Response::getDefault(Codes code, const std::string& body)
// {
//     responsesMap::iterator it = defaults.find(code);

//     if (it != defaults.end())
//     {
//         fullResponse* res = new fullResponse(generateHTTP(it->second._http, body), body);
//         return res;
//     }
//     return NULL;  
// }



const fullResponse& Response::getDefault(Codes code)
{
    fullResponsesMap::iterator it = fullResponses.find(code);

    return it->second;
}

fullResponse::fullResponse(const std::string& http, const std::string& body):
    _http(http),
    _body(body)
{

}


fullResponse::fullResponse(const Responses& obj) : _body(TEMPLATE)
{
        //fullResponse*res = new fullResponse();
        _body.insert(_body.find("<title>") + std::strlen("<title>"), obj._title);
        _body.insert(_body.find("<p>") + std::strlen("<p>"), obj._body);
        _http = generateHTTP(obj._http, _body);
}


Responses::Responses(const std::string& http, const std::string& title, const std::string& body):
    _http(http),
    _title(title),
    _body(body)
{

}

void Response::createFullResponses()
{
    responsesMap::iterator it = defaults.begin();

    for (; it != defaults.end(); ++it)
    {
        fullResponses.insert(std::make_pair(it->first, fullResponse(it->second)));
    }

}

void    Response::initDefaultMap()
{
    defaults.insert(std::make_pair(OK, Responses(HTTP_OK, "", "")));
    defaults.insert(std::make_pair(REQUEST_TIMEOUT, Responses(TIMEOUT_HTTP, TIMEOUT_TITLE, TIMEOUT_BODY)));

    createFullResponses();
}

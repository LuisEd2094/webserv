#include <ProgramConfigs.hpp>
responsesMap Response::defaults;
fullResponsesMap Response::fullResponses;


std::string setContentLenHTTP(const std::string& http, const std::string& body)
{
    /*Checks body and generates Content-Len, can be used to generate other header lines in the future*/
    std::string new_http(http);

    new_http.append(CONTENTLENGTH + toString(body.length()) + CRNL);
    new_http.append(CRNL);
    return new_http;   
}

const fullResponse& Response::getDefault(Codes code)
{
    fullResponsesMap::iterator it = fullResponses.find(code);

    return it->second;
}


fullResponse::fullResponse(const Responses& obj) : _body(obj._title.empty() ? "" : TEMPLATE)
{
        //fullResponse*res = new fullResponse();
        if (!obj._title.empty())
        {
            _body.insert(_body.find("<title>") + std::strlen("<title>"), obj._title);
            _body.insert(_body.find("<p>") + std::strlen("<p>"), obj._body);
        }
        _http = setContentLenHTTP(obj._http, _body);
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
    defaults.insert(std::make_pair(INTERNAL_SERVER_ERROR, Responses(INTERNAL_ERROR_HTTP, INTERNAL_ERROR_TITLE, INTERNAL_ERROR_BODY)));
    defaults.insert(std::make_pair(NOT_FOUND, Responses(NOT_FOUND_HTTP, NOT_FOUND_TILE, NOT_FOUND_BODY)));

    createFullResponses();
    defaults.clear();
}

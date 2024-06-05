#include <ProgramConfigs.hpp>
responsesMap Response::defaults;
defaultResponsesMap Response::defaultResponses;




const defaultResponse& Response::getDefault(ErrorCodes code)
{
    defaultResponsesMap::iterator it = defaultResponses.find(code);

    return it->second;
}


defaultResponse::defaultResponse(const Responses& obj) : _body(obj._title.empty() ? "" : TEMPLATE)
{
        //defaultResponse*res = new defaultResponse();
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

void Response::createDefaultResponses()
{
    responsesMap::iterator it = defaults.begin();

    for (; it != defaults.end(); ++it)
    {
        defaultResponses.insert(std::make_pair(it->first, defaultResponse(it->second)));
    }

}

void    Response::initDefaultMap()
{
    defaults.insert(std::make_pair(OK, Responses(HTTP_OK, "", "")));
    /*4xx errors*/
    defaults.insert(std::make_pair(BAD_REQUEST, Responses(BAD_REQUEST_HTTP, BAD_REQUEST_TITLE, BAD_REQUEST_BODY)));
    defaults.insert(std::make_pair(FORBIDDEN, Responses(FORBIDDEN_HTTP, FORBIDDEN_TITLE, FORBIDDEN_BODY)));
    defaults.insert(std::make_pair(NOT_FOUND, Responses(NOT_FOUND_HTTP, NOT_FOUND_TILE, NOT_FOUND_BODY)));
    defaults.insert(std::make_pair(METHOD_NOT_ALLOWED, Responses(METHOD_HTTP, METHOD_TITLE, METHOD_BODY)));
    defaults.insert(std::make_pair(NOT_ACCEPTABLE, Responses(NOT_ACCEPTABLE_HTTP, NOT_ACCEPTABLE_TITLE, NOT_ACCEPTABLE_BODY)));
    defaults.insert(std::make_pair(REQUEST_TIMEOUT, Responses(TIMEOUT_HTTP, TIMEOUT_TITLE, TIMEOUT_BODY)));
    defaults.insert(std::make_pair(CONFLICT, Responses(CONFLICT_HTTP, CONFLICT_TITLE, CONFLICT_BODY)));
    defaults.insert(std::make_pair(GONE, Responses(GONE_HTTP, GONE_TITLE, GONE_BODY)));
    defaults.insert(std::make_pair(LENGTH_REQUIRED, Responses(LENGTH_HTTP, LENGTH_TITLE, LENGTH_BODY)));
    defaults.insert(std::make_pair(PAYLOAD, Responses(PAYLOAD_HTTP, PAYLOAD_TITLE, PAYLOAD_BODY)));
    defaults.insert(std::make_pair(URI_TOO_LONG, Responses(URI_TOO_LONG_HTTP, URI_TOO_LONG_TITLE, URI_TOO_LONG_BODY)));
    defaults.insert(std::make_pair(MEDIA_TYPE, Responses(MEDIA_HTTP, MEDIA_TITLE, MEDIA_BODY)));
    defaults.insert(std::make_pair(EXPECTATION, Responses(EXPECT_HTTP, EXPECT_TITLE, EXPECT_BODY)));
    defaults.insert(std::make_pair(UPGRADE, Responses(UPGRADE_HTTP, UPGRADE_TITLE, UPGRADE_BODY)));


    /*5xx ERRORS*/
    defaults.insert(std::make_pair(INTERNAL_SERVER_ERROR, Responses(INTERNAL_ERROR_HTTP, INTERNAL_ERROR_TITLE, INTERNAL_ERROR_BODY)));
    defaults.insert(std::make_pair(NOT_IMPLEMENTED, Responses(NOT_IMPLEMENTED_HTTP, NOT_IMPLEMENTED_TITLE, NOT_IMPLEMENTED_BODY)));
    defaults.insert(std::make_pair(GATEWAY_TIMEOUT, Responses(GATEWAY_TIMEOUT_HTTP, GATEWAY_TIMEOUT_TITLE, GATEWAY_TIMEOUT_BODY)));
    defaults.insert(std::make_pair(VERSION_NOT_SUPPORTED, Responses(VERSION_NOT_SUPPORTED_HTTP, VERSION_NOT_SUPPORTED_TITLE, VERSION_NOT_SUPPORTED_BODY)));
    defaults.insert(std::make_pair(BAD_GATEWAY, Responses(BAD_GATEWAY_HTTP, BAD_GATEWAY_TITLE, BAD_GATEWAY_BODY)));

    createDefaultResponses();
    defaults.clear();
}
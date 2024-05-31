#include <Client.hpp>


const int Client::getFD() const
{
    return _fd;
}


const std::string& Client::getURL()
{
    return  _parser_http.getRequested();
}

const std::string& Client::getResponseType() const
{
    return _response_type;
}


const std::string& Client::getBody() const
{
    return _in_body;
}

Server* Client::getServer()
{
    return _server;
}

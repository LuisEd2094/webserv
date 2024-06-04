#include <Client.hpp>


int Client::getFD() const
{
    return _fd;
}


const std::string& Client::getURL() const
{
    return  _parser_http.getRequested();
}

ObjectTypes Client::getResponseType() const
{
    return _response_type;
}


const std::string& Client::getBody() const
{
    return _in_body;
}

Server* Client::getServer() const
{
    return _server;
}

const std::string&  Client::getPathFile() const
{
    return _path_to_file;
}


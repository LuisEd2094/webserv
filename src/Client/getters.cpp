#include <Client.hpp>


int Client::getSocket()
{
    return _fd;
}


const std::string& Client::getURL()
{
    return  _parser_http.getRequested();
}

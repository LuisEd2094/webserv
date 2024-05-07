#include <Client.hpp>


const int Client::getFD() const
{
    return _fd;
}


const std::string& Client::getURL()
{
    return  _parser_http.getRequested();
}

#include <Client.hpp>

void   Client::setHTTPResponse(const std::string &message, BaseHandler *obj)
{
    std::map<BaseHandler*,  ClientHandler *>::iterator it = _map.find(obj);
    if (it != _map.end())
    {
        it->second->setHTTPResponse(message);
    }
    if (it->second == _queue.front())
    {
        Overseer::setListenAction(_fd, POLLIN | POLLOUT);
    }
}

void Client::setBodyResponse(const std::string &message, BaseHandler *obj)
{
    std::map<BaseHandler*,  ClientHandler *>::iterator it = _map.find(obj);
    if (it != _map.end())
    {
        it->second->setBodyResponse(message);
        if (it->second == _queue.front())
        {
            Overseer::setListenAction(_fd, POLLIN | POLLOUT);
        }
    }

}

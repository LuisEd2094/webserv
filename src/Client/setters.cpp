#include <Client.hpp>

void   Client::setHTTPResponse(const std::string &message, BaseHandler *obj)
{
    std::map<BaseHandler*,  ClientHandler *>::iterator it = _response_objects_map.find(obj);
    if (it != _response_objects_map.end())
    {
        it->second->setHTTPResponse(message);
        if (it->second == _response_objects_queue.front())
        {
            Overseer::setListenAction(_fd, IN_AND_OUT);
        }
    }

}

void Client::setBodyResponse(const std::string &message, BaseHandler *obj)
{
    std::map<BaseHandler*,  ClientHandler *>::iterator it = _response_objects_map.find(obj);
    if (it != _response_objects_map.end())
    {
        it->second->setBodyResponse(message);
        if (it->second == _response_objects_queue.front())
        {
            Overseer::setListenAction(_fd, IN_AND_OUT);
        }
    }

}

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

void    Client::addObject(BaseHandler * obj)
{
    if (obj)
    {
        ClientHandler* new_handler = new ClientHandler();
        DirectResponse* direct_object = dynamic_cast<DirectResponse *>(obj);
        
        _response_objects_queue.push(new_handler);
        if (direct_object)
        {
            handleDirectObj(direct_object, new_handler);
        }
        else
        {
            _response_objects_map[obj] = new_handler;
        }
    }
}



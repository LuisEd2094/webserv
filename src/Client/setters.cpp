#include <Client.hpp>

void Client::setFullResponse(const fullResponse& response, BaseHandler* obj)
{
    std::map<BaseHandler*,  RequestHandler *>::iterator it = _response_objects_map.find(obj);
    if (it != _response_objects_map.end())
    {
        it->second->setHTTPResponse(response._http);
        it->second->setBodyResponse(response._body);
        checkFirstQueue(it->second);
    }

}


void   Client::setHTTPResponse(const std::string &message, BaseHandler *obj)
{
    std::map<BaseHandler*,  RequestHandler *>::iterator it = _response_objects_map.find(obj);
    if (it != _response_objects_map.end())
    {
        it->second->setHTTPResponse(message);
        checkFirstQueue(it->second);
    }

}

void Client::setBodyResponse(const std::string &message, BaseHandler *obj)
{
    std::map<BaseHandler*,  RequestHandler *>::iterator it = _response_objects_map.find(obj);
    if (it != _response_objects_map.end())
    {
        it->second->setBodyResponse(message);
    }
}

void    Client::addObject(BaseHandler   * obj)
{
    if (obj)
    {
        RequestHandler* new_handler = new RequestHandler();
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


void  Client::setResponseType(const std::string& type )
{
    _response_type = type;
}




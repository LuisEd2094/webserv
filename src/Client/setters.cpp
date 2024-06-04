#include <Client.hpp>


void    Client::addHeader(const std::string &new_element)
{
    _http_addons.push(new_element);
}

void    Client::addHeader(const std::queue<std::string> &queue)
{
    _http_addons = queue;
}


void Client::setdefaultResponse(const defaultResponse& response, BaseHandler* obj)
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

void    Client::addObject(BaseHandler * obj)
{
    if (obj)
    {
        RequestHandler* new_handler = new RequestHandler(*this);
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


void  Client::setResponseType(ObjectTypes type )
{
    _response_type = type;
}

void Client::setPathFile(const std::string& path)
{
    _path_to_file = path;
}




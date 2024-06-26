#include <Client.hpp>


void Client::setHttpAnswerDirectory(std::string value)
{
    this->_httpAnswerDirectory = value;
}

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
        DirectResponse* no_fd_obj = dynamic_cast<DirectResponse *>(obj);
        
        _response_objects_queue.push(new_handler);
        if (no_fd_obj)
        {
            handleDirectObj(no_fd_obj, new_handler);
        }
        else
        {
            _response_objects_map[obj] = new_handler;
        }
    }
}

void    Client::addErrorFileHandlerToExistingRequest(BaseHandler* old_requester, BaseHandler* new_requester)
{
    /*Need to check if it's a direct object to set the client to out mode, otherwise, replace it on the map*/
    RequestHandler* old_handler = _response_objects_map[old_requester];
    DirectResponse* no_fd_obj = dynamic_cast<DirectResponse *>(new_requester);
    if (no_fd_obj)
    {
        handleDirectObj(no_fd_obj, old_handler);
    }   
    else
    {
        _response_objects_map[new_requester] = old_handler;
        _response_objects_map[old_requester] = NULL;
    }
}


void  Client::setResponseType(ObjectTypes type )
{
    _response_type = type;
}

void Client::setPathFile(const Path& path)
{
    _path_to_file = path;
    _path_to_file_str = static_cast<std::string>(const_cast<Path&>(_path_to_file));
    while (_path_to_file.normalize()) ;
}


void Client::setDefaultHttpResponse(ResponseCodes code)
{
    _defaultHttp = Response::getHttpFirtsLine(code);
    _error_code = code;
}

void Client::addURLRedirection(const std::string& url)
{
    _redirection_urls.push(url);
}

void Client::setConfigElement(const ConfigElement* configElement) 
{
    this->_configElement = configElement;
}

void Client::setExecute(const Path& execute) 
{
    _execute = execute;
}

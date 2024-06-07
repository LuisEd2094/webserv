#include <Server.hpp>
#include <DirectResponse.hpp>
#include <Client.hpp>

//Exception
class Client::clientException : public std::exception
{
    private:
        std::string _err;
    public:
        clientException(const std::string& err) : _err(err) {};
        ~clientException() throw() {};
        const char *what() const throw()
        {
            return (_err.c_str());
        }
};

//public:
Client::Client(Server *server) : BaseHandler()
{
    if (!server)
        return;
    _addrlen = sizeof(_remoteaddr);
    _fd = accept(server->getFD(), (struct sockaddr *)&_remoteaddr,&_addrlen);
    if (_fd == -1) 
    {
        throw Client::clientException("accept" + static_cast<std::string>(strerror(errno)));
    }
    _server = server;
    _response_type = NOT_SET;
    _action = WAIT;
    _pending_read = false;
    _keep_alive = KEEP_ALIVE;
    _size_to_append = 0;
    _can_read = true;
    _error = false;
    _is_chunked = false;
}

Client::~Client() 
{
    while (!_response_objects_queue.empty())
    {
        delete _response_objects_queue.front();
        _response_objects_queue.pop();
    }
    close(_fd);
}


int Client::Action (int event)
{
    if (event & POLLIN)
    {
        if (_can_read)
        {
            readFromFD();
            if (_result < 0)
                return (-1);
        }

    }
    if (event & POLLOUT)
    {
        return sendResponse();
    }
    if (event & POLLHUP)
    {
        std::cout << "POLL HUP" << std::endl;
        return 0;
    }
    return _result;
}

void    Client::addClosingError(ErrorCodes error)
{
    /*This functions adds the error object to the queue and sets the client to a state
        where it'd close the connection after finishing ending the queue */
    addObject(BaseHandler::createObject(_server->getErrorResponseObject(error)));
    _error = true;
    _can_read = false;
    _pending_read = false;
    Overseer::setListenAction(_fd, JUST_OUT);
}

void    Client::addClosingErrorObject(ErrorCodes error)
{
    addClosingError(error);
}


void Client::readFromFD()
{
    _result = recv(_fd, _in_message, sizeof(_in_message), 0);

    if (_result > 0)
    {
        //recv > 0
        // Http todavia _parse_http me indica si no termina.
        // post/get/delete > _in_container, hasta que terminas
        // post > in_body, parseeer URl, METHod, Client dame el body


        if (!_parser_http.getEndRead()) // and nott MAX HEADER SIZE?
        {
            _in_container.append((const char *)_in_message, _result);
            while (_parser_http.getPos()  != _in_container.length())
            {
                if (!_pending_read)
                    _pending_read = true;
                if (_action == WAIT)
                {
                    int parser_method = _parser_http.checkMethod(_in_container);
                    if (!parser_method) //check method returns 0 on success
                    {
                        updateMethodAction();
                    }
                    else
                    {
                        /*
                            if parseer method wrong (not GET, POST or DELETE) then its a 501
                            Error fortmat == Bad Request
                            ERROR_HEADER == BAD_REQUEST
                            if ERROR_VERSION == HTTP Version Not Supported if 1.1+
                        */
                        if (parser_method != WARNING)
                        {
                            if (parser_method == ERROR_FORMAT || parser_method ==  ERROR_HEADER)
                            {
                                addClosingError(BAD_REQUEST);
                            }
                            else if (parser_method == ERROR_METHOD)
                            {
                                addClosingError(NOT_IMPLEMENTED);
                            }
                            else if (parser_method == ERROR_VERSION)
                            {
                                addClosingError(VERSION_NOT_SUPPORTED);
                            }
                        }
                        break;
                    }
                }
                if (_action != WAIT) 
                {
                    int parsingMessage = _parser_http.parsingHeader(_in_container);

                    if (parsingMessage == WARNING) // parsingHeader return 1 on failure
                        break;
                    else if (parsingMessage) // parsingHeader returns something different to warning when error
                    {
                            if (parsingMessage == ERROR_FORMAT)
                            {
                                addClosingError(BAD_REQUEST);
                            }
                            break;
                    } 
                    parseForHttp();
                }
            }
        }
        else if (_action == POST)
        {
            _size_to_append = std::min(_result, static_cast<int>(_content_length - _in_body.length()));
            _in_body.append((const char *)_in_message, _size_to_append);
            if (_in_body.length() == _content_length ) // we done with body || if chunked, if read size is 0
            {  
                resetClient(true);
            }


        }
       
        // else if (_action == POST)
        // {
        //     _in_body.insert(_in_body.end(), _in_message, _in_message + _result);
        // }      
        // if (!_pending_read)
        // {
        //     if (_action == WAIT)
        //     {
        //         if (!_parser_http.checkMethod(_in_container)) //check method returns 0 on success
        //         {
        //             if (!_server->validateAction(*this))
        //             {
        //                 _action = GET;
        //                 return;
        //             }
        //             updateMethodAction();
        //         }
        //     }
        //     if (_action != WAIT) 
        //     {
        //         _parser_http.parsingHeader(_in_container); // ParseingHeader should return true/false each time. Should return TRUE when all HTTP has been parseed "\r\n\r\n", false otherwise
        //         parseForHttp();
        //     }
        // // }
        // else if (_action == POST)
        // {
        //     _in_body.insert(_in_body.end(), _in_message, _in_message + _result);
        // }

    }
    else
    {
        if (_result == 0)
            _result = _keep_alive;
    }
}


void Client::checkFirstQueue(RequestHandler *obj)
{
    if (_response_objects_queue.front() == obj)
    {
        Overseer::setListenAction(_fd, IN_AND_OUT);
    }
}

void Client::handleDirectObj(DirectResponse* direct_object, RequestHandler *new_handler)
{
    if (direct_object->has_http())
    {
        new_handler->setHTTPResponse(direct_object->get_http());
    }
    if (direct_object->has_body())
    {
        new_handler->setBodyResponse(direct_object->get_body());
    }
    checkFirstQueue(new_handler);
    delete direct_object; // I delete this since it wont be going to the FD POLL
}

bool Client::checkPostHeaderInfo()
{
    /*Get post data from Header so we can check it on server to validate it.*/
    const std::string& content_length = _parser_http.getMapValue("Content-Length");
    const std::string& transfer_encoding = _parser_http.getMapValue("Transfer-Encoding");
    if ((content_length != "not found" && transfer_encoding != "not found") || \
        (content_length == "not found" && transfer_encoding == "not found" ))
    {
        /*Close connection since we can't have both content len and transfer enconding 
        and we wont accept a connection without at least one of them */
        addClosingError(LENGTH_REQUIRED); 
        return false;
    }
    if (content_length != "not found")
    {
        _content_length = std::atoi(content_length.c_str());
    }
    else
    {
        _is_chunked = true;
    }
    return true;
}

void Client::parseForHttp()
{
    if (_parser_http.getEndRead())
    {
        if (_action == POST)
        {
            if (!checkPostHeaderInfo())
                return;
        }
        if (!_server->validateAction(*this))
        {
            return;
        }
        std::cout << _in_container << std::endl;
        _in_container.erase(0, _parser_http.getPos() + _parser_http.getEndSize());
        if (_parser_http.getMapValue("Connection") == "keep-alive")
        {
            _keep_alive = true;
        }
        if(_action == POST)
        {
            /* this should check what type of post they are doing, could be a contentlen or a chunked sent */

            _in_body.append(_in_container, 0, std::min(_content_length, _in_container.length()));
            if (_in_body.size() == _content_length ) // we done with body || if chunked, if read size is 0
            {
                resetClient(true);
            }
            else
            {
                /*  
                    Clear _in_container if _in_body doesn't == content_lengthght, since that means we are missing data
                    and we will be getting it next read.
                    If we get the full body in a single read, reset will remove body size from in container
                */
                _in_container.clear();
            }
        }
        else 
        {
            resetClient(false);
        }
    }
}

void Client::updateMethodAction()
{
    const std::string & method = _parser_http.getMethod();
      
    if (method == "GET")
        _action = GET;
    else if (method == "POST")
        _action = POST;
    else if (method == "DELETE")
        _action = DELETE;
}







/*This should also go to the server?*/
int Client::saveInBodyAsFile()
{
    if (_pending_read && _in_body.size() >= _content_length) // check _pending_read just in case we got the first line but not the full http.
    {
        std::ofstream outfile("output_file.jpeg", std::ios::binary);
        if (outfile.is_open())
        {
            outfile.write(_in_body.data(), _in_body.size());
            outfile.close();
            std::cout << "Binary data written to file.\n";
            _action = GET;
            return sendResponse();
        } 
        else
        {
            std::cerr << "Error opening file for writing.\n";
            return (-1);
        }
    }
    return (1);
}

void Client::removeFirstObj()
{
    delete _response_objects_queue.front();
    _response_objects_queue.pop();
    if (!_response_objects_queue.empty())
    {
        if(!(_response_objects_queue.front()->has_http()))
        {
            Overseer::setListenAction(_fd, JUST_IN);
        }
    }
    else
    {
        if (_error)
        {
            Overseer::setListenAction(_fd, 0);
        }
        else
        {
            Overseer::setListenAction(_fd, JUST_IN);
        }
    }
}
 
int Client::sendResponse()
{
    RequestHandler * request = _response_objects_queue.front();
    if (request && request->pendingSend())
    {
        if ((_result = send(_fd, request->getToSend(), request->getChunkSize(), 0) ) == -1)
            return (-1);
        request->updateBytesSent(_result);
        if (request->isFinished())
        {
            removeFirstObj();
            return (!_response_objects_queue.empty() || _keep_alive);
        }
        else
        {
            return (1);
        }
    }
    return (1);
}

void Client::resetClient(bool has_body)
{
    _server->getResponse(*this);

    if (has_body)
    {
        /* gives seg fault*/
        /*If _size_to_append == 0 it means we got the full body in the first read with HTTP*/
        if (_size_to_append == 0)
        {
            _in_container.erase(0, _in_body.length());
        }
        else if (static_cast<int>(_size_to_append) != _result)
        {
            /*If size to append is not == 0 it means we went through at least a second read
                if that append size doesnt equal _result, it means we appended less than the characters we read
                so we can assume that there might be a new http request after the body of the first one.
            */
            _in_container = ((const char *)&(_in_message[_size_to_append + 1]));         
        }
        _in_body.clear();
    }
    _http_addons.empty();
    _pending_read = false;
    _action = WAIT;
    _content_length = 0;
    _response_type = NOT_SET;
    _size_to_append = 0;
    _is_chunked = false;
    _parser_http.resetParsing();
    

}
bool Client::checkObjTimeOut()
{
    /* Should have a different time out setting for open connections that are not being used*/
/*     char *error_buffer[1];
 */    if((_pending_read || _error) && checkTimeOut())
    {
        //When time out, client will stop listening for incoming messages, will send everything it has
        // in queue and thenn close connection.
        if (_pending_read)
        {
            addObject(BaseHandler::createObject(_server->getErrorResponseObject(REQUEST_TIMEOUT)));
            Overseer::setListenAction(_fd, JUST_OUT);
        }
        else
        {
            return true;
        }
        _keep_alive = false;
        return false;
    }
/*     else if (!checkTimeOut() && !_pending_read && recv(_fd, error_buffer, sizeof(error_buffer), MSG_DONTWAIT) == -1)
    {
        return true;   
    } */
    return false;
}

//private:
Client::Client () {}
Client::Client (const Client& rhs) {*this = rhs;}
Client& Client::operator= (const Client& rhs) {(void)rhs; return *this;}






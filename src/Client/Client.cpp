#include <Client.hpp>
#include <Server.hpp>
#include <DirectResponse.hpp>
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
    _server = server;
    _addrlen = sizeof(_remoteaddr);
    _fd = accept(server->getFD(), (struct sockaddr *)&_remoteaddr,&_addrlen);
    if (_fd == -1) 
    {
        throw Client::clientException("accept" + static_cast<std::string>(strerror(errno)));
    }
    _action = WAIT;
    _pending_read = false;
    _keep_alive = true;
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
    if (_response_objects_queue.front() == new_handler)
    {
        Overseer::setListenAction(_fd, IN_AND_OUT);
    }
    delete direct_object; // I delete this since it wont be going to the FD POLL
}

void Client::parseForHttp()
{
    if (_parser_http.getEndRead())
    {
        if(_action == POST)
        {
            std::string content_len = _parser_http.getMapValue("Content-Length");
            if (content_len != "not found")
            {
                _content_length = std::atoi(content_len.c_str());
            }
        }
        _server->getResponse(*this);
        _parser_http.resetParsing();
        _pending_read = false;
        _action = WAIT;
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

void Client::readFromFD()
{
    _result = recv(_fd, _in_message, sizeof(_in_message), 0);

    if (_result > 0)
    {
        _in_http.append((const char *)_in_message, _result);
        while (_parser_http.getPos()  != _in_http.length())
        {
            if (!_pending_read)
                _pending_read = true;
            if (_action == WAIT)
            {
                if (!_parser_http.checkMethod(_in_http)) //check method returns 0 on success
                {
                    if (!_server->validateAction(*this))
                    {
                        _action = GET;
                        return;
                    }
                    updateMethodAction();
                }
                else
                {
                    break;
                }
            }
            if (_action != WAIT) 
            {
                if (_parser_http.parsingHeader(_in_http)) // parsingHeader return 1 on failure
                    break;
                parseForHttp();
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
        //         if (!_parser_http.checkMethod(_in_http)) //check method returns 0 on success
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
        //         _parser_http.parsingHeader(_in_http); // ParseingHeader should return true/false each time. Should return TRUE when all HTTP has been parseed "\r\n\r\n", false otherwise
        //         parseForHttp();
        //     }
        // // }
        // else if (_action == POST)
        // {
        //     _in_body.insert(_in_body.end(), _in_message, _in_message + _result);
        // }

    }
}



bool Client::checkObjTimeOut()
{
    if(_pending_read && checkTimeOut())
    {
        BaseHandler * obj = createObject(Response::getDefault(REQUEST_TIMEOUT));
        addObject(obj);
        _keep_alive = false;
        return false;
    }
    return false;
}

int Client::Action (int event)
{
    if (event & POLLIN)
    {
        readFromFD();
        if (_result < 0)
            return (-1);
        if (_action  == POST)
        {
            return executePostAction();
        }
    }
    else if (event & POLLOUT)
    {
        return executeGetAction();
    }
    else if (event & POLLHUP)
    {
        std::cout << "POLL HUP" << std::endl;
        return 1;
    }
    return _result;
}

//getters


Client::~Client() 
{
    while (!_response_objects_queue.empty())
    {
        delete _response_objects_queue.front();
        _response_objects_queue.pop();
    }
    close(_fd);
}


int Client::executePostAction()
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
            return executeGetAction();
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
        if(_response_objects_queue.front()->has_http())
        {
            Overseer::setListenAction(_fd, IN_AND_OUT);
        }
        else
        {
            Overseer::setListenAction(_fd, JUST_IN);
        }
    }
    else
    {
        Overseer::setListenAction(_fd, JUST_IN);
    }
}
 
int Client::executeGetAction()
{
    // SUPPOSE WE ALREADY KNOW THE HTTP IS DONE;

    // AFTER GET IS DONE WE SHOULD CHECK AND SEE WHAT THE MESSAGE WE ARE GOING TO SEND IS.

    // std::string http = "HTTP/1.1 200 HTTP_OK\r\n"
    //     "Content-Type: text/plain\r\n"
    //     "Content-Length: 13\r\n"
    //     "\r\n"
    //     "Hello, world!\r\n\0";


    RequestHandler * client = _response_objects_queue.front();
/* 
    if (client->has_body())
    {
        _out_body = client->getBody();
        _C_type_body = _out_body.c_str();
        _body_response_len = _out_body.length();
    } */

    if (client->pendingSend())
    {
        if ((_result = send(_fd, client->getToSend(), client->getChunkSize(), 0) ) == -1)
            return (-1);
        client->updateBytesSent(_result);
        if (client->isFinished())
        {
            removeFirstObj();
            return (!_response_objects_queue.empty() || _keep_alive);
        }
        else
        {
            return (1);
        }
    }
/*     if (client->has_body())
    {
        if (!_out_body.empty() && _HTTP_bytes_sent >= _HTTP_response_len)
        {
            chunk_size = (_body_response_len - _body_bytes_sent) > SEND_SIZE ? SEND_SIZE : _body_response_len - _body_bytes_sent;
            if ((_result = send(_fd, _C_type_body + _body_bytes_sent, chunk_size, 0)) == -1)
                return (-1);
            _body_bytes_sent += _result;
            if (_body_bytes_sent >= _body_response_len)
            {
                removeFirstObj();
                return (0);
            }
            else
            {
                return (1);
            }

        }
    }
 */

    return (1);
}

//private:
Client::Client () {}
Client::Client (const Client& rhs) {*this = rhs;}
Client& Client::operator= (const Client& rhs) {(void)rhs; return *this;}






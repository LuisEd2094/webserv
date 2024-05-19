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
    _found_http = false;
    _HTTP_bytes_sent = 0;
    _requested_response = false;
}

void Client::handleDirectObj(DirectResponse* direct_object, ClientHandler * new_handler)
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
    delete direct_object; // I delete this since it wont be going to the FD POLL, so t
}

void Client::parseForHttp()
{
    std::size_t found = _in_http.find("\r\n\r\n");

    if (found != std::string::npos)
    {
        if(_action == POST)
        {
            std::string content_len = _parser_http.getMapValue("Content-Length");
            if (content_len != "not found")
            {
                _content_length = std::atoi(content_len.c_str());
            }
        }
        BaseHandler* newObject = _server->getResponse(*this);
        if (newObject)
        {
            ClientHandler* new_handler = new ClientHandler();
            DirectResponse* direct_object = dynamic_cast<DirectResponse *>(newObject);
            _response_objects_queue.push(new_handler);
            if (direct_object)
            {
                handleDirectObj(direct_object, new_handler);
            }
            else
            {
                _response_objects_map[newObject] = new_handler;
            }
        }
        _in_http = _in_http.substr(0, found + 4); // remove any extra characters you may have

        _found_http = true;
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
        if (!_found_http)
        {
            _in_http.append((const char *)_in_message);
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
            }
            if (_action != WAIT) 
            {
                _parser_http.parsingHeader(_in_http); // ParseingHeader should return true/false each time. Should return TRUE when all HTTP has been parseed "\r\n\r\n", false otherwise
                parseForHttp();
            }
        }
        else if (_action == POST)
        {
            _in_body.insert(_in_body.end(), _in_message, _in_message + _result);
        }

    }
}



bool Client::checkTimeOut()
{
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
        return 0;
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
    if (_found_http && _in_body.size() >= _content_length) // check _found_http just in case we got the first line but not the full http.
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
}
 
int Client::executeGetAction()
{
    // SUPPOSE WE ALREADY KNOW THE HTTP IS DONE;

    // AFTER GET IS DONE WE SHOULD CHECK AND SEE WHAT THE MESSAGE WE ARE GOING TO SEND IS.

    // std::string http = "HTTP/1.1 200 OK\r\n"
    //     "Content-Type: text/plain\r\n"
    //     "Content-Length: 13\r\n"
    //     "\r\n"
    //     "Hello, world!\r\n\0";

    int chunk_size;

    ClientHandler * client = _response_objects_queue.front();

    if (client->has_body())
    {
        _out_body = client->getBody();
        _C_type_body = _out_body.c_str();
        _body_response_len = _out_body.length();
    }

    if (client->has_http())
    {
        _HTTP_response = client->getHTTP();
        _C_type_HTTP = _HTTP_response.c_str();
        _HTTP_response_len = _HTTP_response.length();
        if (!_HTTP_response.empty() && _HTTP_response_len > _HTTP_bytes_sent) // Send if once we have a message pending. might come from an error from server or a response from getResponse.
        {
            chunk_size = (_HTTP_response_len - _HTTP_bytes_sent) > SEND_SIZE ? SEND_SIZE : _HTTP_response_len - _HTTP_bytes_sent;
            if ((_result = send(_fd, _C_type_HTTP + _HTTP_bytes_sent, chunk_size, 0) ) == -1)
                return (-1);
            _HTTP_bytes_sent += _result;
            if (_HTTP_bytes_sent >= _HTTP_response_len && _out_body.empty())
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
    if (client->has_body())
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


    return (1);
}

//private:
Client::Client () {}
Client::Client (const Client& rhs) {*this = rhs;}
Client& Client::operator= (const Client& rhs) {(void)rhs; return *this;}






#include <Client.hpp>
#include <Server.hpp>

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
Client::Client(Server *server)
{
    if (!server)
        return;
    _server = server;
    _addrlen = sizeof(_remoteaddr);
    _fd = accept(server->getSocket(), (struct sockaddr *)&_remoteaddr,&_addrlen);
    if (_fd == -1) 
    {
        throw Client::clientException("accept" + static_cast<std::string>(strerror(errno)));
    }
    _action = WAIT;
    _found_http = false;
    _HTTP_bytes_sent = 0;
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

            //std::cout << _parser_http.getMapValue("patata") << std::endl;
        }
        else if (_action == POST)
        {
            _in_body.insert(_in_body.end(), _in_message, _in_message + _result);
        }

    }
}

int Client::clientAction( int event )
{
    if (event & POLLIN)
    {
        readFromFD();
        if (_result < 0)
            return (-1);
    }
    if (event & POLLOUT && _HTTP_response.empty())
        return (1);
    switch (_action)
    {
        case WAIT: //This is in case we dont get the full verb in the first read
            return (1);
        case GET:
            return executeGetAction();
            break;

        case POST:
            return executePostAction();
            break;

        case DELETE:
            break;

    }
    return _result;
}


//getters


Client::~Client() 
{
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

int Client::executeGetAction()
{
    // SUPPOSE WE ALREADY KNOW THE HTTP IS DONE;

    // AFTER GET IS DONE WE SHOULD CHECK AND SEE WHAT THE MESSAGE WE ARE GOING TO SEND IS.

    // std::string http = "HTTP/1.1 200 OK\r\n"
    //     "Content-Type: text/plain\r\n"
    //     "Content-Length: 13\r\n"
    //     "\r\n"
    //     "Hello, world!\r\n\0";

    if (_found_http && _HTTP_response.empty()) //call server once we get everything from the parser.
    {
        _server->getResponse(*this);
    }
    if (!_HTTP_response.empty()) // Send if once we have a message pending. might come from an error from server or a response from getResponse.
    {
        int chunck_size = (_HTTP_response_len - _HTTP_bytes_sent) > SEND_SIZE ? SEND_SIZE : _HTTP_response_len - _HTTP_bytes_sent;
        if ((_result = send(_fd, _C_type_HTTP + _HTTP_bytes_sent, chunck_size, 0) ) == -1)
            return (-1);
        if (_result == 0)
            return (0);
        if (_result + _HTTP_bytes_sent >= _HTTP_response_len && _out_body.empty())
        {
            return (0);
        }
        else
        {
            _HTTP_bytes_sent += _result;
        }
        if (!_out_body.empty() && _HTTP_bytes_sent >= _HTTP_response_len)
        {
            chunck_size = (_body_response_len - _body_bytes_sent) > SEND_SIZE ? SEND_SIZE : _body_response_len - _body_bytes_sent;
            if ((_result = send(_fd, _C_type_body + _body_bytes_sent, chunck_size, 0)) == -1)
                return (-1);
            if (_result == 0)
                return (0);
            if (_result + _body_bytes_sent >= _body_response_len)
            {
                return (0);
            }
            else
            {
                _body_bytes_sent += _result;
            }

        }
    }
    return (_result);
}

//private:
Client::Client () {}
Client::Client (const Client& rhs) {*this = rhs;}
Client& Client::operator= (const Client& rhs) {(void)rhs; return *this;}

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
    _has_msg_pending = false;
    _found_http = false;
    _bytes_sent = 0;
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

void Client::getMethodAction()
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
                    if (!_server->validateAction(_parser_http.getMethod(), _parser_http.getRequested(), _msg_pending))
                    {
                        _action = GET;
                        _has_msg_pending = true;
                        _msg_to_send = _msg_pending.c_str();
                        _msg_pending_len = std::strlen(_msg_to_send);
                        return;
                    }
                    getMethodAction();
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

int Client::clientAction( int action )
{
    if (action & POLLIN)
    {
        readFromFD();
        if (_result < 0)
            return (-1);
    }
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


int Client::getSocket()
{
    return _fd;
}


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

    if (_found_http && _msg_pending.empty()) //call server once we get everything from the parser.
    {
        if (_server->getResponse(_parser_http.getMethod(), _parser_http.getRequested(), _msg_pending, *this))
        {
            _msg_to_send = _msg_pending.c_str();
            _msg_pending_len = std::strlen(_msg_to_send);
        }
    }
    if (!_msg_pending.empty()) // Send if once we have a message pending. might come from an error from server or a response from getResponse.
    {
        _msg_to_send = _msg_pending.c_str();
        _msg_pending_len = std::strlen(_msg_to_send);
        int chunck_size = (_msg_pending_len - _bytes_sent) > SEND_SIZE ? SEND_SIZE : _msg_pending_len - _bytes_sent;
        if ((_result = send(_fd, _msg_to_send + _bytes_sent, chunck_size, 0) ) == -1)
            return (-1);
        if (_result == 0)
            return (0);
        if (_result + _bytes_sent >= std::strlen(_msg_to_send))
        {
            return (0);
        }
        else
        {
            _bytes_sent += _result;
        }
    }
    return (_result);
}

//private:
Client::Client () {}
Client::Client (const Client& rhs) {*this = rhs;}
Client& Client::operator= (const Client& rhs) {(void)rhs; return *this;}

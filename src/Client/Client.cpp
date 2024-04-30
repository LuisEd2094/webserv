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
    _bytes_sent = 0;
}


void Client::testingParse()
{
    std::size_t i = 0;
    for (; i < _result ; ++i)
    {
        if (_in_message[i] == '\n') // found end of first line
        {
            if (_in_http.find("GET") != std::string::npos)
                _action = GET;
            else if (_in_http.find("POST") != std::string::npos)
                _action = POST;
            else if (_in_http.find("DELETE") != std::string::npos)
                _action = DELETE;
            else
                return; //RETURN ERROR?
            break;
        }
    }
}


void Client::readFromFD()
{
    _result = recv(_fd, _in_message, sizeof(_in_message), 0);
    if (_result >= 0)
        _in_http.append(_in_message, std::strlen(_in_message));

    std::cout << _in_http << std::endl;
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
        case WAIT:
            //EXAMPLE FOR GET
            //SHOULD DO CLIENT RECV AND THEN CHECK WHAT ACTION IT HAS TO TAKE

            // this->executeGetAction -> should parse Header.
            // this->parseData() --> parseData should set _actions to get post delete
            // this->clientAction() so that it executes the correct order

            testingParse();
        case GET:
            return executeGetAction();
            break;

        case POST:
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
        // std::size_t result = recv(_fd, _in_message, sizeof(_in_message), 0);
    // if (result == -1)
    //     std::cerr << "recv: " + static_cast<std::string>(strerror(errno)) << std::endl;
    // else
    // {
    //     _in_http.append((char *)_in_message, result);

    //     std::size_t end_http = _in_http.find("\r\n\r\n"); // Ubuntu Curl sends this at the end of http
    //     if (end_http != std::string::npos)
    //     {
    //         std::cout << "HTTP HEADER:\n" << _in_http.substr(0, end_http + 4) << std::endl;
    //         std::size_t content_length_start = _in_http.find("Content-Length: ");
    //         if (content_length_start != std::string::npos)
    //         {
    //             content_length_start += std::strlen("Content-Length: ");
    //             std::size_t content_length_end = _in_http.find("\r\n", content_length_start);
    //             if (content_length_end != std::string::npos)
    //             {
    //                 std::string content_length_str = _in_http.substr(content_length_start, content_length_end - content_length_start);
    //                 int content_length = std::atoi(content_length_str.c_str()); // check values
    //                 if (_in_http.size() >= end_http + 4 + content_length) // 4 for "\r\n\r\n"
    //                 {
    //                    commenting to keep track. If no boundary found, no http extra, direct transfer
    //                      std::size_t boundary_start = _in_http.find("boundary=");
    //                     boundary_start += std::strlen("boundary=");
    //                     std::size_t boundary_end = _in_http.find("\r\n", boundary_start);

    //                     std::string boundary_str = _in_http.substr(boundary_start, boundary_end - boundary_start + 2);

    //                     std::cout << "FOUND BOUNDARY:" << boundary_str;

    //                     std::size_t other = std::strlen("Content-Disposition: form-data; name=\"image\"; filename=\"image.jpeg\"");
    //                     other += std::strlen("Content-Type: image/jpeg");
    //                     other = 0;

    //                     std::string binary_data = _in_http.substr( other + end_http + 4 , content_length);
    //                     std::cout << binary_data << std::endl;
    //                     std::ofstream outfile("output_file.jpeg", std::ios::binary);
    //                     if (outfile.is_open())
    //                     {
    //                         outfile.write(binary_data.c_str(), binary_data.size());
    //                         outfile.close();
    //                         std::cout << "Binary data written to file.\n";
    //                     }
    //                     else
    //                     {
    //                         std::cerr << "Error opening file for writing.\n";
    //                     }
    //                                     Clear the _in_http buffer and append any remaining data
    //                     _in_http.clear();
    //                     if (_in_http.size() > end_http + 4 + content_length)
    //                     {
    //                         _in_http.append(_in_message + end_http + 4 + content_length, _in_http.size() - (end_http + 4 + content_length));
    //                     }
    //                 }


    //             }


    //         }

    //     }

    // }
    return (0);
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

    if (_has_msg_pending == false  && _in_http.find("\r\n\r\n") != std::string::npos)
    {
        _msg_pending = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, world!\r\n\0";
        _msg_to_send = _msg_pending.c_str();
        _msg_pending_len = std::strlen(_msg_to_send);
        _has_msg_pending = true;
    }
    if (_has_msg_pending)
    {
        int chunck_size;
        if ((_msg_pending_len - _bytes_sent) > SEND_SIZE)
            chunck_size = SEND_SIZE;
        else
            chunck_size = _msg_pending_len - _bytes_sent;
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

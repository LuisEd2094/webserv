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
    _has_msg_pending = true;
    _bytes_sent = 0;
}
int Client::getSocket()
{
    return _fd;
}


Client::~Client() 
{
    close(_fd);
}

int Client::recvData()
{
    std::size_t result = recv(_fd, _in_message, sizeof(_in_message), 0);
    if (result == -1)
        std::cerr << "recv: " + static_cast<std::string>(strerror(errno)) << std::endl;
    else
    {
        _in_http.append((char *)_in_message, result);
        std::size_t end_http = _in_http.find("\r\n\r\n"); // Ubuntu Curl sends this at the end of http
        if (end_http != std::string::npos)
        {
            std::cout << "HTTP HEADER:\n" << _in_http.substr(0, end_http + 4) << std::endl;
            std::size_t content_length_start = _in_http.find("Content-Length: ");
            if (content_length_start != std::string::npos)
            {
                content_length_start += std::strlen("Content-Length: ");
                std::size_t content_length_end = _in_http.find("\r\n", content_length_start);
                if (content_length_end != std::string::npos)
                {
                    std::string content_length_str = _in_http.substr(content_length_start, content_length_end - content_length_start);
                    int content_length = std::atoi(content_length_str.c_str()); // check values
                    if (_in_http.size() >= end_http + 4 + content_length) // 4 for "\r\n\r\n"
                    {
                        /*commenting to keep track. If no boundary found, no http extra, direct transfer*/
/*                         std::size_t boundary_start = _in_http.find("boundary=");
                        boundary_start += std::strlen("boundary=");
                        std::size_t boundary_end = _in_http.find("\r\n", boundary_start);

                        std::string boundary_str = _in_http.substr(boundary_start, boundary_end - boundary_start + 2);

                        std::cout << "FOUND BOUNDARY:" << boundary_str;
 */
                        std::size_t other = std::strlen("Content-Disposition: form-data; name=\"image\"; filename=\"image.jpeg\"");
                        other += std::strlen("Content-Type: image/jpeg");
                        other = 0;

                        std::string binary_data = _in_http.substr( other + end_http + 4 , content_length);
                        std::cout << binary_data << std::endl;
                        std::ofstream outfile("output_file.jpeg", std::ios::binary);
                        if (outfile.is_open())
                        {
                            outfile.write(binary_data.c_str(), binary_data.size());
                            outfile.close();
                            std::cout << "Binary data written to file.\n";
                        }
                        else
                        {
                            std::cerr << "Error opening file for writing.\n";
                        }
                                        // Clear the _in_http buffer and append any remaining data
                        _in_http.clear();
                        if (_in_http.size() > end_http + 4 + content_length)
                        {
                            _in_http.append(_in_message + end_http + 4 + content_length, _in_http.size() - (end_http + 4 + content_length));
                        }
                    }


                }


            }

        }

    }
    if (result == 0)
    {
        //std::cout << _in_http << std::endl;
    }
    return (result);
}

bool    Client::hasPending()
{
    return _has_msg_pending;
}


//MSG_NOSIGNAL NO PIPE
void Client::sendBatch()
{
    const char *msg_to_send = _msg_pending.c_str();


    std::size_t result;
    if ((result = send(_fd, msg_to_send + _bytes_sent, SEND_SIZE, MSG_NOSIGNAL)) == -1)
        std::cerr << "send: " + static_cast<std::string>(strerror(errno)) << std::endl;
    else if (result + _bytes_sent != std::strlen(msg_to_send))
    {
        _bytes_sent += result;
    }
    else
    {
        _has_msg_pending = false;
        //close connection?
    }   
}

//std::strlen(http.c_str())
void Client::firstSendData(const std::string& http)
{
    const char *msg_to_send = http.c_str();
    std::size_t result;
    if ((result = send(_fd, msg_to_send, SEND_SIZE, MSG_NOSIGNAL) )== -1)
        std::cerr << "send: " + static_cast<std::string>(strerror(errno)) << std::endl;
    if (result != std::strlen(msg_to_send))
    {
        _msg_pending = http;
        _bytes_sent = result;
    }
    else
    {
        //close connection?
    }
}


//private:
Client::Client () {}
Client::Client (const Client& rhs) {*this = rhs;}
Client& Client::operator= (const Client& rhs) {(void)rhs; return *this;}

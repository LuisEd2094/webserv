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

    int bytes_rec = recv(_fd, _msg, sizeof(_msg), 0);
    if (bytes_rec > 0)
    {
        _msg[bytes_rec] = '\0'; // Null-terminate the received data

        std::cout << _msg << std::endl;     
    }
    return (bytes_rec);
}

void Client::sendData(std::string http)
{
    if (send(_fd, http.c_str(), std::strlen(http.c_str()), 0) == -1)
        std::cerr << "send: " + static_cast<std::string>(strerror(errno)) << std::endl;
}


//private:
Client::Client () {}
Client::Client (const Client& rhs) {*this = rhs;}
Client& Client::operator= (const Client& rhs) {(void)rhs; return *this;}

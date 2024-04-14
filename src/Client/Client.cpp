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

    int bytes_rec = recv(_fd, _msg, sizeof(_msg), 0);
    if (bytes_rec > 0)
    {
        _msg[bytes_rec] = '\0'; // Null-terminate the received data

        std::cout << _msg << std::endl;     
    }
    return (bytes_rec);
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
    if ((result = send(_fd, msg_to_send + _bytes_sent, 5, MSG_NOSIGNAL)) == -1)
        std::cerr << "send: " + static_cast<std::string>(strerror(errno)) << std::endl;
    if (result + _bytes_sent != std::strlen(msg_to_send))
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

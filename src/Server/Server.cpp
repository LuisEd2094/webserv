#include <Server.hpp>
#include <Client.hpp>
#include <ClientHandler.hpp>
#include <DirectResponse.hpp>

//Exceptions

class Server::socketException: public std::exception
{
    private:
        std::string _err;
    public:
        socketException(const std::string& err) : _err(err) {};
        ~socketException() throw() {};
        const char *what() const throw()
        {
            return (_err.c_str());
        }
};

//Public Methods

Server::Server(t_confi* confi) : 
    BaseHandler(),
    _backlog(confi->backlog),
    _ip(confi->ip),
    _port(confi->port)
{
    _fd = confi->socket;
    std::memset(&(_hints), 0, sizeof(_hints));
    _hints.ai_family = confi->hints.ai_family; //takes ipv4 and ipv6
    _hints.ai_socktype = confi->hints.ai_socktype; // TCP stream sockets
    _hints.ai_flags = confi->hints.ai_flags;

    this->initSocket();
}

Server::~Server()
{
    close(_fd);
}


int                Server::Action(int event)
{
    (void)event;
    Client *newClient = new Client(this);
    Overseer::addToPfds(newClient);
    return (1);

}


bool Server::validateAction(Client& client)
{
    // check method and url against config.
    const std::string url = client.getURL();
    if (url == "/" or url == "/nolen.py" or url == "/index.html")
        return true;
    else
    {
        client.setHTTPResponse("HTTP/1.1 404 Not Found\r\n"
                        "Content-Length: 0\r\n"
                        "\r\n\r\n", this);
        return (false);
    }
}

BaseHandler* Server::getResponse(Client & client)
{
    //CGI?
    // We assume we called validateAction before reaching this point.
    const std::string & url = client.getURL();
    

    // Validar la accion a tomar.

    // CGI, llaamar un archivo, direct listing

    // server -> como localizar la accion a realizar.

    if (url == "/")
    {
        return DirectResponse::createNewDirect("HTTP/1.1 200 OK\r\n"
                    "\r\n", "HOLA");
    }
    else if (url == "/index.html")
    {
        try
        {
            return FileReader::createNewFileReader(client);
        }
        catch(const std::exception& e)
        {
            return DirectResponse::createNewDirect("HTTP/1.1 500 Internal Server Error\r\n", "");
        }  
    }
    else if (url == "/nolen.py")
    {
        try
        {
            return CGI::createNewCGI(client);
        }
        catch(const std::exception& e)
        {
            return DirectResponse::createNewDirect("HTTP/1.1 500 Internal Server Error\r\n", ""); // Should call an object to create the http
        }
        
    }
    return NULL;

}

bool Server::checkTimeOut()
{
    return false;
}

void Server::initSocket()
{
    int status;

    if ((status = getaddrinfo(_ip.empty() ? NULL : _ip.c_str(), _port.c_str(), &_hints, &_servinfo)) != 0) 
            throw Server::socketException("getaddrinfo error: " + static_cast<std::string>(gai_strerror(status)));

    struct addrinfo *p;
    int yes = 1;
    // need to check what serverinfo list has
    for (p = _servinfo; p != NULL; p = p->ai_next)
    {
        if ((_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cerr << "socket error: " + static_cast<std::string>(strerror(errno)) << std::endl;
            continue ; 
            // freeaddrinfo(_servinfo);
            // throw Server::socketException("socket error: " + static_cast<std::string>(strerror(errno)));
        };
        if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            close(_fd);
            std::cerr << "socket error: " + static_cast<std::string>(strerror(errno)) << std::endl;
            continue ;
        } 
                
        // bind it to the port we passed in to getaddrinfo():
        
        if (bind(_fd, p->ai_addr, p->ai_addrlen) != 0)
        {
            close(_fd);
            std::cerr << "bind error: " + static_cast<std::string>(strerror(errno)) << std::endl;
            continue;
            // freeaddrinfo(_servinfo);
            // throw Server::socketException("bind error:  "+ static_cast<std::string>(strerror(errno)));
        }
        break;
    }

    freeaddrinfo(_servinfo); // all done with this structure
    if (p == NULL)
        throw Server::socketException("server: failed to bind");

    if (listen(_fd, _backlog) == -1)
        throw Server::socketException("listen error: " + static_cast<std::string>(strerror(errno)));
}

std::ostream &operator<<(std::ostream &os,  Server &obj)
{
	os << "Server: " << std::endl;
}

//Private Methods

Server::Server(){}      
Server::Server(const Server& rhs ) { *this = rhs; }
Server& Server::operator=(const Server& rhs) {(void)rhs; return *this;}

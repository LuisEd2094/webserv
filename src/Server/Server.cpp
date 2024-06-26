#include <Server.hpp>
#include <Client.hpp>
#include <RequestHandler.hpp>
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
    _ip(confi->ip == "localhost" ? "" : confi->ip), //If config file host:locahost then IP == ""
    _port(confi->port)
{
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
    try 
    {
        Client *newClient = new Client(this);

        if (Overseer::addToPfds(newClient))
            newClient->addClosingErrorObject(SERVICE_UNAVAILABLE);   
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return (1);
}


bool Server::validateAction(Client& client)
{
    bool status = this->prepareClient4ResponseGeneration(client);
    return status;
}

bool Server::prepareClient4ResponseGeneration(Client& client)
{
    std::list<ConfigVirtualServer>::iterator server;
    client.setDefaultHttpResponse(NOT_FOUND);
    client.setResponseType(FILE_OBJ);
    for (server = this->virtualServers.begin(); server != this->virtualServers.end(); server++)
    {
        if (server->prepareClient4ResponseGeneration(client))
            break ;
    }
    return (server != this->virtualServers.end());

}


bool Server::checkObjTimeOut()
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
            continue ;
        if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            close(_fd);
            continue ;
        }  
        // bind it to the port we passed in to getaddrinfo():
        if (bind(_fd, p->ai_addr, p->ai_addrlen) != 0)
        {
            close(_fd);
            continue;
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
    (void)obj;
	os << "Server: " << std::endl;
    return os;
}

//Private Methods

Server::Server(){}      

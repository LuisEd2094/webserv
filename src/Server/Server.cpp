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
    Client *newClient = new Client(this);
    Overseer::addToPfds(newClient);
    return (1);

}


bool Server::validateAction(Client& client)
{
    // check method and url against config.
    // GET es valido para esta URL
    // POST valido para uRL etc 
    const std::string url = client.getURL();
    if (url == "/" or url == "/nolen.py" or url == "/index.html")
        return true;
    else
    {
        client.addObject(BaseHandler::createObject(getErrorResponseObject(NOT_FOUND)));
        return (false);
    }
}

void Server::getResponse(Client & client)
{
    //CGI?
    // We assume we called validateAction before reaching this point.
    const std::string & url = client.getURL();
    BaseHandler * response;
    try
    {
        std::cout << client.getBody() << std::endl;
        if (url == "/")
        {
            client.setResponseType(DIRECT_OBJ);
        }
        else if (url == "/index.html")
        {
            client.setResponseType(FILE_OBJ);
        }
        else if (url == "/nolen.py")
        {
            client.setResponseType(CGI_OBJ);
        }
        response = BaseHandler::createObject(client);


    }
    catch(const std::exception& e)
    {
        response = BaseHandler::createObject(getErrorResponseObject(INTERNAL_SERVER_ERROR));
    }
    client.addObject(response);
}


const fullResponse& Server::getErrorResponseObject(ErrorCodes code)
{
    // check code against files in config
    // else returns default error
    // Server map con FullResponseObjs. Full response objs. HTTP y el Contenido del FILE si es que fue indicado en el 
    // confi 
    // 

    return Response::getDefault(code);

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
    (void)obj;
	os << "Server: " << std::endl;
    return os;
}

//Private Methods

Server::Server(){}      
Server::Server(const Server& rhs ) { *this = rhs; }
Server& Server::operator=(const Server& rhs) {(void)rhs; return *this;}

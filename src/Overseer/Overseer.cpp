#include <Overseer.hpp>

//Exceptions
class Overseer::pollException: public std::exception
{
    private:
        std::string _err;
    public:
        pollException(const std::string& err) : _err(err) {};
        ~pollException() throw() {};
        const char *what() const throw()
        {
            return (_err.c_str());
        }
};

//Public
Overseer::Overseer() : _fd_count(0) 
{
    _pfds = new struct pollfd[MAX_FDS];
}


Overseer::~Overseer()
{

    std::map<int, Server *>::iterator it = _servers.begin();
    for (; it != _servers.end(); it++)
    {
        delete it->second;
    }
    _servers.clear();

    std::map<int, Client *>::iterator it2 = _clients.begin();
    for (; it2 != _clients.end(); it2++)
    {
        delete it2->second;
    }
    _clients.clear();
}


void Overseer::removeFromPFDS()
{
    _pfds[_i] = _pfds[_fd_count - 1];
    _fd_count--;
}


void Overseer::addToPfds(int new_fd, int events, int revents)
{
    if (_fd_count != MAX_FDS )
    {
        _pfds[_fd_count].fd = new_fd;
        _pfds[_fd_count].events = events;
        _pfds[_fd_count].revents = revents;
        _fd_count++;
    }
}


void Overseer::saveServer(t_confi* confi)
{

    Server *server = new Server(confi);
    _servers[server->getSocket()] = server;
    addToPfds(server->getSocket(), POLLIN, 0);
}


void Overseer::handleClientAction(Client *client, int action)
{
    int status = client->clientAction(action);
        
    if (status == 0 || status == -1)
    {
        if (status == 0)
            std::cout << client->getSocket() << " closed connection" << std::endl;
        else
            std::cerr << "client: " << static_cast<std::string>(strerror(errno)) << std::endl;
        removeFromPFDS(); // I need a better way to handle PFDS closing, since they might close on the first loop, before the _i had time to increase
        _clients.erase(client->getSocket());
        delete client;
    }
}


Client* Overseer::createClient(Server* server)
{
    Client* client = new Client(server);
    _clients[client->getSocket()] = client;
    addToPfds(client->getSocket(), POLLIN | POLLOUT, POLLIN); //Creates Client with POLLIN active for revents, so that it can check the client on the same POLL loop
    return (client);
}

void printHostName()
{
    char hostname[100];
    size_t size = sizeof(hostname); 


    gethostname(hostname, size);

    std::cout <<  "my domain: "<< hostname << std::endl;
}


void Overseer::mainLoop()
{
    int poll_connection;
    printHostName(); //REMOVE , IT USES INVALID FUNTIONS
    while(1) 
    {
        //std::cout << "Poll count " << _fd_count << std::endl;
        poll_connection = poll(_pfds, _fd_count, -1);
        if (poll_connection == -1) 
        {
            throw Overseer::pollException("poll: " + static_cast<std::string>(strerror(errno)));
        }
        // Run through the existing connections looking for data to read
        for(_i = 0; _i < _fd_count; _i++) 
        {
            // Check if someone's ready to read
            if (_pfds[_i].revents & POLLIN) 
            { // We got one!!

                if (_servers.find(_pfds[_i].fd) != _servers.end()) //one server got a connection
                {
                    // If listener is ready to read, handle new connection
                    Client *newClient; 
                    try
                    {
                        newClient = createClient(_servers[_pfds[_i].fd]);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                        continue;
                    }
                    //handleClientAction(newClient, POLLIN); 
                } 
                else 
                {
                    std::map<int, Client *>::iterator it = _clients.find(_pfds[_i].fd);
                    if (it != _clients.end())
                    {
                        handleClientAction(it->second, POLLIN);
                    }

                }
            }
            else if (_pfds[_i].revents & POLLOUT)
            {
                std::map<int, Client *>::iterator it = _clients.find(_pfds[_i].fd);
                if (it != _clients.end())
                {
                    handleClientAction(it->second, POLLOUT);
                }

            } 
        } 
    }
    
    return ;
}





//Private
Overseer::Overseer(const Overseer& rhs){*this = rhs;}
Overseer& Overseer::operator= (const Overseer& rhs) {(void)rhs; return *this;}

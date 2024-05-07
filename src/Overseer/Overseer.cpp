#include <Overseer.hpp>
# include <CGI.hpp>

std::size_t Overseer::_i = 0;
std::size_t Overseer::_fd_count = 0;
std::map<int, CGI *> Overseer::_CGIs;
std::map<int, Server *> Overseer::_servers;
std::map<int, Client *> Overseer::_clients;
std::map<int, BaseHandler *> Overseer::_pending_fds;

struct pollfd  Overseer::_pfds[MAX_FDS];


//Public

void Overseer::cleanOverseer()
{
    std::map<int, BaseHandler *>::iterator it = _pending_fds.begin();
    for (; it != _pending_fds.end(); it++)
    {
        delete it->second;
    }
    _pending_fds.clear();
}

Overseer::Overseer()
{
}


Overseer::~Overseer()
{
    cleanOverseer();

}


void Overseer::removeFromPFDS()
{
    Overseer::_pfds[Overseer::_i] = Overseer::_pfds[Overseer::_fd_count - 1];
    Overseer::_fd_count--;
}

void    Overseer::addToPfds(Server * server)
{
    _pending_fds[server->getFD()] = server;
    addToPfds(server->getFD(), POLLIN, 0);
}

void    Overseer::addToPfds(Client * client)
{
    _pending_fds[client->getFD()] = client;
    addToPfds(client->getFD(), POLLIN | POLLOUT, POLLIN); 
}

void    Overseer::addToPfds(CGI * cgi)
{
    _pending_fds[cgi->getFD()] = cgi;
    addToPfds(cgi->getFD(), POLLHUP , 0); 
}

BaseHandler* Overseer::getObj(int fd)
{
    std::map<int, BaseHandler *>::iterator it = _pending_fds.find(fd);
    if (it != _pending_fds.end())
    {
        return it->second;
    }
    return NULL;
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

    Server * server = new Server(confi);
    addToPfds(server);
}

void Overseer::handleAction(BaseHandler *obj, int event)
{
    int status = obj->Action(event);
        
    if (status == 0 || status == -1)
    {
        if (status == 0)
            std::cout << obj->getFD() << " closed connection" << std::endl;
        else
            std::cerr << "error: " << static_cast<std::string>(strerror(errno)) << std::endl;

        removeFromPFDS(); // I need a better way to handle PFDS closing, since they might close on the first loop, before the _i had time to increase
        _pending_fds.erase(obj->getFD());
        delete obj;
    }
}





#include <Overseer.hpp>
# include <CGI.hpp>

std::size_t Overseer::_i = 0;
std::size_t Overseer::_fd_count = 0;
bool Overseer::canContinue = true;
std::map<int, BaseHandler *> Overseer::_pending_fds;

struct pollfd  Overseer::_pfds[MAX_FDS];


//Public

void Overseer::cleanOverseer(int sig)
{
    (void)sig;
    std::map<int, BaseHandler *>::iterator it = _pending_fds.begin();
    for (; it != _pending_fds.end(); it++)
    {
        delete it->second;
    }
    _pending_fds.clear();
    canContinue = false;
}

Overseer::Overseer()
{
}


Overseer::~Overseer()
{
    cleanOverseer(0);

}


void Overseer::removeFromPFDS(BaseHandler *obj)
{

    _pfds[_i] = _pfds[_fd_count - 1];
    _fd_count--;
    _i--;
    _pending_fds.erase(obj->getFD());
    delete obj;
}

void Overseer::setListenAction(int fd, int action) // might need to change this to a map since it will take a long time to go through the _pfds if you have alot of them
{
    for (std::size_t i = 0; i < _fd_count; ++i)
    {
        if (_pfds[i].fd == fd)
        {
            _pfds[i].events = action;
        } 
    }
}

void    Overseer::addToPfds(BaseHandler * base)
{
    _pending_fds[base->getFD()] = base;
    addToPfds(base->getFD(), JUST_IN, 0);
    base->setTime();
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

Server* Overseer::saveServer(t_confi* confi)
{
    Server * server;
    try
    {
        server = new Server(confi);
    }
    catch(const std::exception& e)
    {
        server = NULL;
        std::cerr << e.what() << '\n';
        throw e;
    }
    
    addToPfds(server);
    return server;
}

bool Overseer::handleAction(BaseHandler *obj, int event)
{
    int status = obj->Action(event);
        
    if (status == 0 || status == -1)
    {
        if (status == 0)
            std::cout << obj->getFD() << " closed connection" << std::endl;
        else
            std::cerr << "error: " << static_cast<std::string>(strerror(errno)) << std::endl;
        removeFromPFDS(obj); // I need a better way to handle PFDS closing, since they might close on the first loop, before the _i had time to increase
        return false; 
    }
    obj->setTime();
    return true;

}

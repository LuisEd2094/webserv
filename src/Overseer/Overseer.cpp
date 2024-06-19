#include <Overseer.hpp>
# include <CGI.hpp>

std::size_t Overseer::_i = 0;
std::size_t Overseer::_fd_count = 0;
bool Overseer::canContinue = true;
std::map<int, BaseHandler *> Overseer::_pending_fds;
deletedFiles Overseer::files; 

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

void Overseer::removeInCGIPipe(int fd)
{
    _pfds[_i] = _pfds[_fd_count - 1];
    _fd_count--;
    _i--;
    (void)fd;
    _pending_fds.erase(fd);
}

void Overseer::removeFromPFDS(BaseHandler *obj)
{
    _pfds[_i] = _pfds[_fd_count - 1];
    _fd_count--;
    _i--;
    _pending_fds.erase(obj->getFD());
    std::cerr << "I removed someone" << std::endl;
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

bool    Overseer::addCGIInPipe(BaseHandler * base, int fd)
{
    _pending_fds[fd] = base;
    bool status = addToPfds(fd, JUST_OUT, 0);
    base->setTime();
    return status;    
}


bool    Overseer::addToPfds(BaseHandler * base)
{
    _pending_fds[base->getFD()] = base;
    bool status = addToPfds(base->getFD(), JUST_IN, 0);
    base->setTime();
    return status;    
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



bool Overseer::addToPfds(int new_fd, int events, int revents)
{
    _pfds[_fd_count].fd = new_fd;
    _pfds[_fd_count].events = events;
    _pfds[_fd_count].revents = revents;
    _fd_count++;

    return _fd_count >= MAX_FDS;
/*     if (_fd_count != MAX_FDS )
    {
        _pfds[_fd_count].fd = new_fd;
        _pfds[_fd_count].events = events;
        _pfds[_fd_count].revents = revents;
        _fd_count++;
        return true;
    }
    return false; */
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
            std::cerr << obj->getFD() << " closed connection" << std::endl;
        else
            std::cerr << "error: " << static_cast<std::string>(strerror(errno)) << std::endl;
        removeFromPFDS(obj); // I need a better way to handle PFDS closing, since they might close on the first loop, before the _i had time to increase
        return false; 
    }
    obj->setTime();
    return true;

}


void            Overseer::addToDeleted(const std::string& path)
{
    if (!files.addToDelete(path))
    {
        std::cerr << "Couldn't add to deleted files " + path + ". Path still available for use." << std::endl;
    }
}
void            Overseer::removeFromDeleted(const std::string& path)
{
    if (!files.removeFromDeleted(path))
    {
        std::cerr << "File was not removed from deleted because it was not in list: " + path << std::endl;
    }

}
bool            Overseer::checkIfDeleted(const std::string& path)
{
    return (files.checkIfDeleted(path));
}

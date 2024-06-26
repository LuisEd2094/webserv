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

void Overseer::mainLoop()
{
    int poll_connection;
    while(canContinue) 
    {
        poll_connection = poll(_pfds, _fd_count, TIME_OUT_POLL);
        if (poll_connection == -1) 
        {
            std::cerr << "poll: " + static_cast<std::string>(strerror(errno)) << std::endl;
            break ;
        }
        // Run through the existing connections loHTTP_OKing for data to read
        try
        {
            for(_i = 0; canContinue and _i < _fd_count; _i++) 
            {
                // Check if someone's ready to read
                std::map<int, BaseHandler *>::iterator it = _pending_fds.find(_pfds[_i].fd);
                if (!it->second)
                {
                    continue;
                }
                if (_pfds[_i].revents & (POLLIN | POLLOUT | POLLHUP))
                {
                    if (_pfds[_i].revents & POLLHUP)
                    {
                        if (!handleAction(it->second, POLLHUP))
                            continue;
                    }
                    if (_pfds[_i].revents & POLLIN)
                    {
                        if (!handleAction(it->second, POLLIN))
                            continue;
                    }
                    if (_pfds[_i].revents & POLLOUT)
                    {
                        if (!handleAction(it->second, POLLOUT))
                            continue;
                    }
                }
                else
                {
                    if (it->second->checkObjTimeOut())
                        removeFromPFDS(it->second);
                }
            }         
        }
        catch(const std::exception& e)
        {
            canContinue = false;
            std::cerr << e.what() << '\n';
        }
                
    }
    std::map<int, BaseHandler *>::iterator it = _pending_fds.begin();
    for (; it != _pending_fds.end(); it++)
    {
        delete it->second;
    }
    _pending_fds.clear();
    return ;
}

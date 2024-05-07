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
    int found;
    printHostName(); //REMOVE , IT USES INVALID FUNTIONS
    while(1) 
    {
        //std::cout << "Poll count " << _fd_count << std::endl;
        poll_connection = poll(_pfds, _fd_count, -1);
        found = 0;
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
                std::map<int, BaseHandler *>::iterator it3 = _pending_fds.find(_pfds[_i].fd);

                if (it3 != _pending_fds.end()) //one server got a connection
                {
                    // If listener is ready to read, handle new connection
                    handleAction(it3->second, POLLIN);
                    //handleAction(newClient, POLLIN); 
                } 
                found++;
            }
            else if (_pfds[_i].revents & POLLHUP)
            {
                std::map<int, BaseHandler *>::iterator it2 = _pending_fds.find(_pfds[_i].fd);
                if (it2 != _pending_fds.end())
                {
                    handleAction(it2->second, POLLHUP);
                }
                found++;
            }
            else if (_pfds[_i].revents & POLLOUT)
            {
                std::map<int, BaseHandler *>::iterator it = _pending_fds.find(_pfds[_i].fd);
                if (it != _pending_fds.end() && _pfds[_i].fd == it->second->getFD() )
                {
                    handleAction(it->second, POLLOUT);
                }
                found++;
            }
            if (found == poll_connection)
                break;
        } 
    }
    return ;
}

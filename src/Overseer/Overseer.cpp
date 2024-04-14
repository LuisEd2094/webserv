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
}


void Overseer::saveServer(t_confi* confi)
{

    Server *server = new Server(confi);
    _servers[server->getSocket()] = server;
    _pfds[_fd_count].fd = server->getSocket();
    _pfds[_fd_count].events = POLLIN;
    _fd_count++;
}


void Overseer::addToPfds(int new_fd)
{
    if (_fd_count != MAX_FDS )
    {
        _pfds[_fd_count].fd = new_fd;
        _pfds[_fd_count].events = POLLIN;
        _fd_count++;
    }
}


void Overseer::mainLoop()
{
    int poll_count;
    int new_fd;        // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256];    // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];
    char hostname[100];
    size_t size = sizeof(hostname); 


    gethostname(hostname, size);

    std::cout <<  "my domain: "<< hostname << std::endl;

    std::cout << "server: waiting for connections..." << std::endl;
    while(1) 
    {
        //std::cout << "Poll count " << _fd_count << std::endl;
        poll_count = poll(_pfds, _fd_count, -1);

        if (poll_count == -1) 
        {
            throw Overseer::pollException("poll: " + static_cast<std::string>(strerror(errno)));
        }

        // Run through the existing connections looking for data to read
        for(int i = 0; i < _fd_count; i++) 
        {

            // Check if someone's ready to read
            if (_pfds[i].revents & POLLIN) 
            { // We got one!!

                if (_servers.find(_pfds[i].fd) != _servers.end()) //one server got a connection
                {
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof remoteaddr;
                    new_fd = accept(_servers[_pfds[i].fd]->getSocket(),
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);

                    if (new_fd == -1) 
                    {
                        std::cerr << "accept" + static_cast<std::string>(strerror(errno)) << std::endl;
                    } 
                    else 
                    {
                        this->addToPfds(new_fd);
                        char msg[10000];
                        recv(new_fd, msg, sizeof(msg), 0);
                        std::cout << msg << std::endl;

                        std::string http = "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "Content-Length: 13\r\n"
                                    "\r\n"
                                    "Hello, world!\r\n";

                        if (send(new_fd, http.c_str(), std::strlen(http.c_str()), 0) == -1)
                            std::cerr << "send: " + static_cast<std::string>(strerror(errno)) << std::endl;
                    }
                } 
                else 
                {
                    //regular client

                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
    return ;
}





//Private
Overseer::Overseer(const Overseer& rhs){*this = rhs;}
Overseer& Overseer::operator= (const Overseer& rhs) {(void)rhs; return *this;}
#include <iostream>
#include <program.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


#include <netinet/in.h>

/*
    Classes for when we need to validate data before setting or getting. 

    Structs for when we just need to save or manipulate without the need of validation
*/

typedef struct s_confi
{
    std::string     port;
    int             socket_fd;
    int             status;
    int             backlog;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    

} t_confi;


/*
    https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    Hints struct and func info
*/

void exitError(const std::string& error)
{
    std::cerr << error << std::endl;
    std::exit(EXIT_FAILURE);

}

void setConfi(t_confi *confi)
{
    std::memset(&(confi->hints), 0, sizeof(confi->hints));
    confi->hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
    confi->hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    confi->hints.ai_flags = AI_PASSIVE;
    confi->port = "80";
    confi->backlog = 10;
    //    If the AI_PASSIVE flag is specified in hints.ai_flags, and node
    //    is NULL, then the returned socket addresses will be suitable for
    //    bind(2)ing a socket that will accept(2) connections.  The
    //    returned socket address will contain the "wildcard address"
    //    (INADDR_ANY for IPv4 addresses, IN6ADDR_ANY_INIT for IPv6
    //    address).  The wildcard address is used by applications
    //    (typically servers) that intend to accept connections on any of
    //    the host's network addresses.  If node is not NULL, then the
    //    AI_PASSIVE flag is ignored.
    // NULL to get my IP, could be domain

    if ((confi->status = getaddrinfo(NULL, confi->port.c_str(), &confi->hints, &confi->servinfo)) != 0) 
        exitError("getaddrinfo error: " + static_cast<std::string>(gai_strerror(confi->status)));
    // make a socket:
    // loop through all the results and bind to the first we can

    struct addrinfo *p;
    int yes = 1;
    // need to check what serverinfo list has
    for (p = confi->servinfo; p != NULL; p = p->ai_next)
    {
        if ((confi->socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            std::cerr << "socket error: " + static_cast<std::string>(strerror(errno)) << std::endl;
            continue ; 
            // freeaddrinfo(confi->servinfo);
            // exitError("socket error: " + static_cast<std::string>(strerror(errno)));
        };
        if (setsockopt(confi->socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            freeaddrinfo(confi->servinfo);
            exitError("socket error: " + static_cast<std::string>(strerror(errno)));
        } 
                
        // bind it to the port we passed in to getaddrinfo():
        
        if (bind(confi->socket_fd, p->ai_addr, p->ai_addrlen) != 0)
        {
            close(confi->socket_fd);
            std::cerr << "bind error: " + static_cast<std::string>(strerror(errno)) << std::endl;
            continue;
            // freeaddrinfo(confi->servinfo);
            // exitError("bind error:  "+ static_cast<std::string>(strerror(errno)));
        }
        break;
    }

    freeaddrinfo(confi->servinfo); // all done with this structure
    if (p == NULL)
        exitError("server: failed to bind");

    if (listen(confi->socket_fd, confi->backlog) == -1)
        exitError("listen error: " + static_cast<std::string>(strerror(errno)));

    socklen_t               addr_size;
    int                     new_fd;
    struct sockaddr_storage their_addr;
    char hostname[100];
    size_t size = sizeof(hostname); 

    gethostname(hostname, size);

    std::cout <<  "my domain: "<< hostname << std::endl;

    std::cout << "server: waiting for connections..." << std::endl;

    while (1)
    {
        addr_size = sizeof (their_addr);
        new_fd = accept(confi->socket_fd, (struct sockaddr *)&their_addr, &addr_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        // inet_ntop(their_addr.ss_family,
        //     get_in_addr((struct sockaddr *)&their_addr),
        //     s, sizeof s);
        // printf("server: got connection from %s\n", s);

        if (!fork()) 
        { // this is the child process
            close(confi->socket_fd); // child doesn't need the listener
            if (send(new_fd, "Hello, world!", 13, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this

    }
}
int main()
{
    t_confi confi;

    setConfi(&confi);
    //freeaddrinfo(confi.servinfo);
    test();
    std::cout << "hello \n";
    return 0;
}

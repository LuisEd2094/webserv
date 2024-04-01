#include <iostream>
#include <program.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
    Classes for when we need to validate data before setting or getting. 

    Structs for when we just need to save or manipulate without the need of validation
*/

typedef struct s_confi
{
    std::string     port;
    int             socketFd;
    int             status;
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
    //    If the AI_PASSIVE flag is specified in hints.ai_flags, and node
    //    is NULL, then the returned socket addresses will be suitable for
    //    bind(2)ing a socket that will accept(2) connections.  The
    //    returned socket address will contain the "wildcard address"
    //    (INADDR_ANY for IPv4 addresses, IN6ADDR_ANY_INIT for IPv6
    //    address).  The wildcard address is used by applications
    //    (typically servers) that intend to accept connections on any of
    //    the host's network addresses.  If node is not NULL, then the
    //    AI_PASSIVE flag is ignored.

    if ((confi->status = getaddrinfo(NULL, confi->port.c_str(), &confi->hints, &confi->servinfo)) != 0) 
        exitError("getaddrinfo error: " + static_cast<std::string>(gai_strerror(confi->status)));
    // make a socket:

    if (confi->socketFd = socket(confi->servinfo->ai_family, confi->servinfo->ai_socktype, confi->servinfo->ai_protocol) == -1)
    {
        freeaddrinfo(confi->servinfo);
        exitError("socket error: couldn't create socket");
    };
    // bind it to the port we passed in to getaddrinfo():


    if (bind(confi->socketFd, confi->servinfo->ai_addr, confi->servinfo->ai_addrlen) != 0)
    {
        freeaddrinfo(confi->servinfo);
        exitError("bind error: couldn't bind port " + confi->port + " to socket");
    }
}
int main()
{
    t_confi confi;

    setConfi(&confi);
    freeaddrinfo(confi.servinfo);
    test();
    return 0;
}

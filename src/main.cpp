#include <iostream>
#include <Aux.hpp>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/poll.h>


#include <Server.hpp>
#include <Overseer.hpp>

#include <vector>


#include <netinet/in.h>

/*
    Classes for when we need to validate data before setting or getting. 

    Structs for when we just need to save or manipulate without the need of validation
*/

int main()
{
    t_confi confi;

    // all this info should come from the confi file
    std::memset(&(confi.hints), 0, sizeof(confi.hints));
    confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
    confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    confi.hints.ai_flags = AI_PASSIVE;
    confi.port = "80";
    confi.backlog = 10;
    Overseer overseer;
    // Read from file, create server, save server to overseer;
    try
    {
        //overseer.file(argv[1]);
        // Should read from file or default info, internally call saveServer for each server
        // throws exception when server fails
        overseer.saveServer(&confi);
/*         std::memset(&(confi.hints), 0, sizeof(confi.hints));
        confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
        confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
        confi.hints.ai_flags = AI_PASSIVE;
        confi.port = "81";
        confi.backlog = 10;
        overseer.saveServer(&confi); */

        overseer.mainLoop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    //freeaddrinfo(confi.servinfo);
    std::cout << "hello \n";
    return 0;
}
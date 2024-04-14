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


void initConnection(Server& server)
{
    //sets litening socket, available at socket_fd

    socklen_t               addr_size;
    int                     new_fd;
    struct sockaddr_storage their_addr;
    char hostname[100];
    size_t size = sizeof(hostname); 



    int fd_count = 0;
    int fd_size = 5;

    //Get a stfuct for my polls, they are init to fd_size (arbitrary)
    std::vector<struct pollfd *>  pfds(fd_size);


    gethostname(hostname, size);

    std::cout <<  "my domain: "<< hostname << std::endl;

    std::cout << "server: waiting for connections..." << std::endl;

    while (1)
    {
        addr_size = sizeof (their_addr);
        new_fd = accept(server.getSocket(), (struct sockaddr *)&their_addr, &addr_size);
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
            close(server.getSocket()); // child doesn't need the listener
            /*once we have a connection, curl sends a http/1.1 request, we should parse it, check it's correct and send correct respose*/
            /*running curl -X POST sends a post request*/
            char msg[10000];
            recv(new_fd, msg, sizeof(msg), 0);
            std::cout << msg << std::endl;

            std::string http = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/plain\r\n"
                     "Content-Length: 13\r\n"
                     "\r\n"
                     "Hello, world!\r\n";

            if (send(new_fd, http.c_str(), std::strlen(http.c_str()), 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }
    close(server.getSocket());
}

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
        //Server server(&confi);
        overseer.saveServer(&confi);


        std::memset(&(confi.hints), 0, sizeof(confi.hints));
        confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
        confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
        confi.hints.ai_flags = AI_PASSIVE;
        confi.port = "81";
        confi.backlog = 10;

        //Server server1(&confi);
        overseer.saveServer(&confi);
        
        //initConnection(server);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    overseer.test();
    //freeaddrinfo(confi.servinfo);
    std::cout << "hello \n";
    return 0;
}
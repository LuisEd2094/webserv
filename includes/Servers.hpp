#ifndef SERVERS_HPP
# define SERVERS_HPP
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>


typedef struct s_confi
{
    std::string     port;
    int             socket_fd;
    int             backlog;
    struct addrinfo hints;
    struct addrinfo *servinfo;
} t_confi;


class Server
{
    public:
        Server(t_confi* confi);




    private:
        int                 _socket;
        int                 _backlog;
        struct addrinfo     _hints;
        struct addrinfo*    _servinfo;
        std::string         _port;
        
        Server();      
        Server(const Server& rhs );
        Server& operator=(const Server& rhs);
};



#endif
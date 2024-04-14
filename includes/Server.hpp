#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <cstring>
# include <Aux.hpp>
# include <cerrno>
# include <unistd.h>


/*
    https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    Hints struct and func info
*/

typedef struct s_confi
{
    std::string     port;
    int             socket;
    int             backlog;
    struct addrinfo hints;
    struct addrinfo *servinfo;
} t_confi;


class Server
{
    public:
        Server(t_confi* confi);
        ~Server();

        //Getters
        int getSocket();


    private:
        int                 _socket;
        int                 _backlog;
        std::string         _port;
        struct addrinfo     _hints;
        struct addrinfo*    _servinfo;
        
        Server();      
        Server(const Server& rhs );
        Server& operator=(const Server& rhs);

        void    initSocket();


        class   socketException;
};



#endif
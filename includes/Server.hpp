#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <cstring>
# include <Aux.hpp>
# include <cerrno>
# include <unistd.h>
# include <Confi.hpp>
# include <CGI.hpp>

class Client; 

class Server
{
    public:
        Server(t_confi* confi);
        ~Server();

        //Getters
        int getSocket();

        bool validateAction(Client& client);
        bool getResponse(Client& client);
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

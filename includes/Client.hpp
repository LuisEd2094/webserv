#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <sys/socket.h>
# include <cerrno>
# include <string.h>

class Server;

class Client
{
    public:
        Client(Server *server);
        ~Client();

        int     recvData();
        void    sendData(std::string http);

        //getters
        int getSocket();
        class clientException;


    private:
        int                     _fd;
        char                    _msg[10000];
        std::string             _out_msg;
        struct sockaddr_storage _remoteaddr; 
        socklen_t               _addrlen;
        Server *                _server;
        Client ();
        Client (const Client& rhs);
        Client& operator= (const Client& rhs);
};


#endif
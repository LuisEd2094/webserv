#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <sys/socket.h>
# include <cerrno>
# include <string>
# include <vector>
# include <fstream>



# define SEND_SIZE 5

class Server;

class Client
{
    public:
        Client(Server *server);
        ~Client();

        int     recvData();
        void    firstSendData(const std::string & http);
        void    sendBatch();
        bool    hasPending();

        //getters
        int getSocket();
        class clientException;


    private:
        int                     _fd;
        bool                    _has_msg_pending;
        std::string             _msg_pending;
        std::size_t             _bytes_sent;

        char                    _in_message[1024];
        std::size_t             _in_size;
        std::string             _in_http;
        std::vector<void *>     _in_body;
        std::string             _out_msg;
        struct sockaddr_storage _remoteaddr; 
        socklen_t               _addrlen;
        Server *                _server;
        Client ();
        Client (const Client& rhs);
        Client& operator= (const Client& rhs);
};


#endif
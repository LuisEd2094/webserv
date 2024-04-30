#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <sys/socket.h>
# include <cerrno>
# include <string>
# include <vector>
# include <fstream>
# include <poll.h>




# define SEND_SIZE 5
# define RECV_SIZE 5

enum Actions {
    WAIT,
    GET,
    POST, 
    DELETE
};

class Server;

class Client
{
    public:
        Client(Server *server);
        ~Client();
        int     clientAction(int action);
        //getters
        int getSocket();
        class clientException;


    private:

        //PLACER HOLDER UNTIL WE ADD THE PARSER
        void                    testingParse();


        void                    readFromFD();
        std::size_t             _result;

        const char *            _msg_to_send;




        int                     executeGetAction();
        int                     executePostAction();
        int                     _fd;
        bool                    _has_msg_pending;
        std::string             _msg_pending;
        std::size_t             _msg_pending_len;
        std::size_t             _bytes_sent;

        Actions                 _action;
        char                    _in_message[RECV_SIZE];
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

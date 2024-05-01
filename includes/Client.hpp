#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <iostream>
# include <sys/socket.h>
# include <cerrno>
# include <string>
# include <vector>
# include <fstream>
# include <poll.h>
# include <algorithm>


# define SEND_SIZE 8000
# define RECV_SIZE 8000

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
        int                     _fd;
        Actions                 _action;

        std::size_t             _result;


        bool                    _has_msg_pending;
        std::string             _msg_pending;
        std::size_t             _msg_pending_len;
        std::size_t             _bytes_sent;

        char                    _in_message[RECV_SIZE];
        const char *            _msg_to_send;

        bool                    _found_http;
        std::string             _in_http;
        std::vector<char>       _in_body;
        std::size_t             _content_length;

        //PLACER HOLDER UNTIL WE ADD THE PARSER
        void                    getMethodAction();
        void                    parseForHttp();






        void                    readFromFD();
        int                     executeGetAction();
        int                     executePostAction();




        struct sockaddr_storage _remoteaddr; 
        socklen_t               _addrlen;
        Server *                _server;
        Client ();
        Client (const Client& rhs);
        Client& operator= (const Client& rhs);
};


#endif

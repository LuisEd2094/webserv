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
# include <Parsing.hpp>
# include <Aux.hpp>
# include <BaseHandler.hpp>
# include <cstring>
# include <Overseer.hpp>
# include <queue>
# include <ClientHandler.hpp>
# include <DirectResponse.hpp>

class Server;

class Client : public BaseHandler
{
    public:
        Client(Server *server);
        ~Client();
        int                 Action(int event);

        // setters
        void                addObject    // checks url
(BaseHandler *);
        void                setHTTPResponse(const std::string &message, BaseHandler*);
        void                setBodyResponse(const std::string &message, BaseHandler*);
        bool                checkTimeOut();

        //getters
        const int           getFD() const;
        const std::string&  getURL();

        class clientException;


    private:
        std::queue< ClientHandler *>               _response_objects_queue;
        std::map< BaseHandler *,  ClientHandler *> _response_objects_map;
        Parsing                 _parser_http;
        Actions                 _action;
        std::size_t             _result;

        bool                    _keep_alive;

        char                    _in_message[RECV_SIZE];

        bool                    _found_http;
        std::string             _in_http;
        std::vector<char>       _in_body;
        std::size_t             _content_length;

        void                    updateMethodAction();
        void                    parseForHttp();



        void                    setSendValues(const ClientHandler &);


        void                    readFromFD();
        int                     executeGetAction();
        int                     executePostAction();


        void                    handleDirectObj(DirectResponse*, ClientHandler *);
        void                    removeFirstObj();




        struct sockaddr_storage _remoteaddr; 
        socklen_t               _addrlen;
        Server *                _server;
        Client ();
        Client (const Client& rhs);
        Client& operator= (const Client& rhs);
};



#endif

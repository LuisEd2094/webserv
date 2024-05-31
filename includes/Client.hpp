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
# include <RequestHandler.hpp>
# include <DirectResponse.hpp>

class Server;

class Client : public BaseHandler
{
    public:
        Client(Server *server);
        ~Client();
        int                 Action(int event);

        // setters
        void                addObject(BaseHandler *);
        void                setFullResponse(const fullResponse& response, BaseHandler*);
        void                setHTTPResponse(const std::string &message, BaseHandler*);
        void                setBodyResponse(const std::string &message, BaseHandler*);

        void                setResponseType(const std::string& );
        bool                checkObjTimeOut();

        //getters
        const int           getFD() const;
        const std::string&  getURL();
        const std::string&  getResponseType() const;
        const std::string&  getBody() const;

        class clientException;


    private:
        std::queue< RequestHandler *>               _response_objects_queue;
        std::map< BaseHandler *,  RequestHandler *> _response_objects_map;
        Parsing                 _parser_http;
        Actions                 _action;
        std::size_t             _result;
        std::size_t             _size_to_append;

        bool                    _keep_alive;

        char                    _in_message[RECV_SIZE];

        bool                    _pending_read;
        std::string             _in_container;
        std::string             _in_body;
        std::size_t             _content_length;

        void                    updateMethodAction();
        void                    parseForHttp();



        void                    setSendValues(const RequestHandler &);


        void                    readFromFD();
        void                    resetClient(bool);
        int                     executeGetAction();
        int                     executePostAction();


        void                    handleDirectObj(DirectResponse*, RequestHandler *);
        void                    removeFirstObj();

        std::string             _response_type;




        struct sockaddr_storage _remoteaddr; 
        socklen_t               _addrlen;
        Server *                _server;
        Client ();
        Client (const Client& rhs);
        Client& operator= (const Client& rhs);
};



#endif

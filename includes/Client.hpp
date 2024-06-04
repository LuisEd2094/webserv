#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <Parsing.hpp>
# include <Aux.hpp>
# include <BaseHandler.hpp>
# include <Overseer.hpp>
# include <RequestHandler.hpp>
# include <DirectResponse.hpp>
# include <ProgramConfigs.hpp>

# include <queue>
# include <cstring>
# include <algorithm>
# include <iostream>
# include <sys/socket.h>
# include <cerrno>
# include <string>
# include <vector>
# include <fstream>
# include <poll.h>

class Server;

class Client : public BaseHandler
{
    public:
        Client(Server *server);
        ~Client();
        int                 Action(int event);
        bool                checkObjTimeOut();
        void                addObject(BaseHandler *);

        // setters
        void                addHeader(const std::queue<std::string> &);
        void                setFullResponse(const fullResponse& response, BaseHandler*);
        void                setHTTPResponse(const std::string &message, BaseHandler*);
        void                setBodyResponse(const std::string &message, BaseHandler*);
        void                setResponseType(ObjectTypes );
        void                setPathFile(const std::string&);

        //getters
        Server *            getServer() const;
        int                 getFD() const;
        const std::string&  getURL() const;
        ObjectTypes         getResponseType() const;
        const std::string&  getBody() const;
        const std::string&  getPathFile() const;
        const std::string&  getMapValue(const std::string& ); 

        class clientException;


    private:
        std::queue< std::string>                    _http_addons;
        std::queue< RequestHandler *>               _response_objects_queue;
        std::map< BaseHandler *,  RequestHandler *> _response_objects_map;
        Parsing                 _parser_http;
        Actions                 _action;
        int                      _result;
        std::size_t             _size_to_append;

        bool                    _keep_alive;

        char                    _in_message[RECV_SIZE];

        bool                    _pending_read;
        std::string             _in_container;
        std::string             _in_body;
        std::size_t             _content_length;

        void                    updateMethodAction();
        void                    parseForHttp();


        bool                    _can_read;
        bool                    _error;
        void                    readFromFD();
        void                    resetClient(bool);
        int                     sendResponse();
        int                     saveInBodyAsFile();

        void                    checkFirstQueue(RequestHandler *);
        void                    handleDirectObj(DirectResponse*, RequestHandler *);
        void                    removeFirstObj();

        ObjectTypes             _response_type;

        std::string             _path_to_file;




        struct sockaddr_storage _remoteaddr; 
        socklen_t               _addrlen;
        Server *                _server;
        Client ();
        Client (const Client& rhs);
        Client& operator= (const Client& rhs);
};



#endif

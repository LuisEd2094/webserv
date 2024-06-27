#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <Parsing.hpp>
# include <Aux.hpp>
# include <BaseHandler.hpp>
# include <Overseer.hpp>
# include <RequestHandler.hpp>
# include <DirectResponse.hpp>
# include <ProgramConfigs.hpp>
# include <Path.hpp>

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
# include <stack>

class Server;

class Client : public BaseHandler
{
    public:
        Client(Server *server);
        ~Client();
        int                 Action(int event);
        bool                checkObjTimeOut();
        void                setTime();


                                    


        // setters
        void                addErrorFileHandlerToExistingRequest(BaseHandler* old_requester, BaseHandler* new_requester);
        void                addHeader(const std::string &new_element);
        void                addHeader(const std::queue<std::string> &);
        void                addURLRedirection(const std::string&);
        void                addObject(BaseHandler *);
        void                addClosingErrorObject(ResponseCodes);

        void                setHttpAnswerDirectory(std::string value);
        void                setdefaultResponse(const defaultResponse& response, BaseHandler*);
        void                setHTTPResponse(const std::string &message, BaseHandler*);
        void                setBodyResponse(const std::string &message, BaseHandler*);
        void                setResponseType(ObjectTypes );
        void                setPathFile(const Path&);
        void                setDefaultHttpResponse(ResponseCodes);
        void                setConfigElement(const ConfigElement *);
        void                setExecute(const Path& execute);


        //getters
        std::string                     getHttpAnswerDirectory() const;
        Server *                        getServer() const;
        int                             getFD() const;
        int                             getClientFD() const;
        const std::string&              getURL() const;
        
        const std::string&              getMethod() const;
        const std::string&              getHost() const;
        Actions                         getAction() const;
        bool                            getIsChunked() const;
        std::size_t                     getContentLength() const;
        bool                            getURLempty() const; 
        std::string                     getNextURLRedirect();
        ObjectTypes                     getResponseType() const;
        const std::string&              getBody() const;

        const Path&                     getPathFile() const;
        const std::string&              getPathFileString() const; 

        const std::string&              getMapValue(const std::string& )const;
        const std::queue<std::string>&  getHTTPAddons() const;
        const ConfigElement *           getConfigElement()  const;
        const Path&                     getExecute() const;



        class clientException;


    private:
        std::queue< std::string>                    _http_addons;
        std::stack< std::string>                    _redirection_urls;
        std::queue< RequestHandler *>               _response_objects_queue;
        std::map< BaseHandler *,  RequestHandler *> _response_objects_map;
        Parsing                 _parser_http;
        Actions                 _action;
        Path                    _execute;

        int                     _result;
        bool                    _was_zero;
        std::size_t             _size_to_append;

        bool                    _keep_alive;

        char                    _in_message[BUFFER_SIZE];

        bool                    _pending_read;
        std::string             _in_container;
        std::string             _in_body;


        std::string             _chunk;
        std::size_t             _chunk_size;

        std::size_t             _content_length;
        bool                    _is_chunked;

        void                    updateMethodAction();
        void                    parseForHttp();


        bool                    _can_read;
        bool                    _error;
        void                    readFromFD();
        void                    processChunk();
        void                    addClosingError(ResponseCodes);
        void                    resetClient(bool);
        int                     sendResponse();
        void                    handlerRecv();
        void                    makeChildrenToRespond();
        bool                    verifyIfEnoughForLocation();



        /*POST methods*/
        bool                    checkPostHeaderInfo();

        void                    checkFirstQueue(RequestHandler *);
        void                    handleDirectObj(DirectResponse*, RequestHandler *);
        void                    removeFirstObj();


        Path                    _path_to_file;
        std::string             _httpAnswerDirectory;




        struct sockaddr_storage _remoteaddr; 
        socklen_t               _addrlen;
        Server *                _server;
        ConfigVirtualServer*    _virtualServer;
        Client ();

};



#endif

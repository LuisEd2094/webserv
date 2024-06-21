#ifndef BASEHANDLER_HPP
# define BASEHANDLER_HPP

# include <iostream>
# include <ctime>
# include <ProgramConfigs.hpp>
# include <ConfigElement.hpp>


class Client;
class BaseHandler
{
    public:
        BaseHandler();
        BaseHandler(const BaseHandler& obj);
        virtual ~BaseHandler() = 0;
        virtual int                     getFD() const = 0;
        virtual int                     Action( int ) = 0;
        virtual void                    setTime();
        virtual bool                    checkObjTimeOut() = 0;
        virtual const ConfigElement*    getConfigElement() const;
        virtual BaseHandler *           getErrorResponse(ResponseCodes code); 

        virtual int                     getClientFD() const;
        virtual ResponseCodes              getErrorCode() const;
        virtual const std::string&      getPathFileString() const;
        virtual const std::string&      getDefaultHttpResponse() const;

        
        static BaseHandler*             createObject(BaseHandler&);
        static BaseHandler*             createObject(Client&);
        static BaseHandler*             createObject(const defaultResponse &);
        

    protected:
		const   ConfigElement			    			*_configElement;
        ObjectTypes                                     _response_type;
        ResponseCodes                                      _error_code;
        std::string                                     _defaultHttp;
        std::string                                     _path_to_file_str;
        int                                             _client_fd;
        int                                             _fd;

        bool    checkTimeOut();
        static const ObjectTypes valid_objs[NUM_OBJ] ;

        time_t _last_time;
};

#endif

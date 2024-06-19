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
        virtual int getFD() const = 0;
        virtual int Action( int ) = 0;
        virtual void    setTime();
        virtual bool    checkObjTimeOut() = 0;
        virtual const ConfigElement*  getConfigElement();

        static BaseHandler*     createObject(Client&);
        static BaseHandler*     createObject(const defaultResponse &);
        static BaseHandler*     getErrorResponse(ErrorCodes code);


    protected:
		const   ConfigElement			    			*_configElement;
        bool    checkTimeOut();
        static const ObjectTypes valid_objs[NUM_OBJ] ;
        int _fd;
        time_t _last_time;
};


#endif

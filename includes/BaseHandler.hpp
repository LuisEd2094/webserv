#ifndef BASEHANDLER_HPP
# define BASEHANDLER_HPP

# include <iostream>
# include <ctime>
# include <ProgramConfigs.hpp>


class Client;
class BaseHandler
{
    public:
        BaseHandler();
        virtual ~BaseHandler() = 0;
        virtual const int getFD() const = 0;
        virtual int Action( int ) = 0;
        virtual void    setTime();
        virtual bool    checkTimeOut() = 0;
        static BaseHandler*     createObject(const std::string&, Client&);
        static BaseHandler*     createObject(const std::string& , const std::string&);

    protected:
        static std::string valid_objs[NUM_OBJ] ;
        int _fd;
        time_t _last_time;
};


#endif

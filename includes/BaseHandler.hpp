#ifndef BASEHANDLER_HPP
# define BASEHANDLER_HPP

# include <iostream>
# include <ctime>
class BaseHandler
{
    public:
        BaseHandler();
        virtual ~BaseHandler() = 0;
        virtual const int getFD() const = 0;
        virtual int Action( int ) = 0;
        virtual void    setTime();
        virtual bool    checkTimeOut() = 0;
    protected:
        int _fd;
        time_t _last_time;
};


#endif

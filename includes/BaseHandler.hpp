#ifndef BASEHANDLER_HPP
# define BASEHANDLER_HPP

class BaseHandler
{
    public:
        BaseHandler();
        virtual ~BaseHandler() = 0;
        virtual const int getFD() const = 0;
        virtual int Action( int ) = 0;
    protected:
        int _fd;
};


#endif

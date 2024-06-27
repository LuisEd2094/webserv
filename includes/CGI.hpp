#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>

# include <BaseHandler.hpp>

class Client;
class CGI : public BaseHandler
{
    public:
        CGI(Client& client);
        ~CGI();
        int                 Action(int event);

        bool                checkObjTimeOut();

        static BaseHandler *    createNewCGI(Client& client);
        int                     getFD() const;
        int                     readPipe();

        class CGIException;

    private:
        int             _out_pipe[2];
        int             _in_pipe[2];

        bool            _has_error;
        pid_t           _pid;
        std::string     _buffer;
        std::string     _defaultHttp;
        std::string     _body;
        std::size_t     _len;
        std::size_t     _sent;

        void fill_arrays(std::vector<std::string>& env_strings,
            const Client& client);
        void            changeDir(const std::string& fileUrl);


};


#endif

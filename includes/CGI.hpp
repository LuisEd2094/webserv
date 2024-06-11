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

        static CGI *        createNewCGI(Client& client);
        int           getFD() const;
        int                 readPipe();

        class CGIException;

    private:
        int             _pipe[2];
        int             _client_fd;
        pid_t           _pid;
        std::string     _buffer;
        std::string     _defaultHttp;
        void            changeDir(const std::string& fileUrl);


};


#endif

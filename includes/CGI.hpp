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

        bool                checkTimeOut();

        static void         createNewCGI(Client& client);
        const int           getFD() const;
        int                 readPipe();

        class CGIException;

    private:
        int             _pipe[2];
        int             _client_fd;
        pid_t           _pid;

};


#endif

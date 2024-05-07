#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <unistd.h>
# include <signal.h>
# include <BaseHandler.hpp>

class Client;
class CGI : public BaseHandler
{
    public:
        CGI(Client& client);
        ~CGI();
        int                 Action(int event);

        static void createNewCGI(Client& client);
        static void destroyCGI(CGI *cgi);
        const int getFD() const;
        int readPipe();

        class CGIException;

    private:
        int             _pipe[2];
        Client&         _client;
        pid_t           _pid;

};


#endif

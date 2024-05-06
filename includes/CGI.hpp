#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <unistd.h>
# include <signal.h>


class Client;
class CGI
{
    public:
        CGI(Client& client);
        ~CGI();
        static void createNewCGI(Client& client);
        static void destroyCGI(CGI *cgi);
        int getSocket();
        int readPipe();

        class CGIException;

    private:
        int             _pipe[2];
        Client&         _client;

};


#endif

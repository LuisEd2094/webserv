#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <unistd.h>
# include <signal.h>


class Client;
class CGI
{
    public:
        CGI(const Client& client, std::string& message, const std::string& url);
        ~CGI();
        static void createNewCGI(const Client& client, std::string& message, const std::string& url);
        static void destroyCGI(CGI *cgi);
        int getSocket();
        int readPipe();

        class CGIException;

    private:
        int             _pipe[2];
        const Client&   _client;
        std::string&    _client_message;


};


#endif

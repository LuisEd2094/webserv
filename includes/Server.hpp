#ifndef SERVER_HPP
# define SERVER_HPP

class Server;

# include <BaseHandler.hpp>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <cstring>
# include <Aux.hpp>
# include <cerrno>
# include <unistd.h>
# include <Confi.hpp>
# include <CGI.hpp>
# include <FileReader.hpp>
# include <Parsing.hpp>
# include "ConfigVirtualServer.hpp"


# include <ProgramConfigs.hpp>
class Client; 

class Server : public BaseHandler
{
    public:
        Server(t_confi* confi);
        ~Server();

        int                 Action(int event);


        //Getters
        int getFD() const;

        bool validateAction(Client& client);
        void getResponse(Client& client);
        bool prepareClient4ResponseGeneration(Client& client);
        bool checkObjTimeOut();
        const defaultResponse& getErrorResponseObject(ErrorCodes);
        std::list<ConfigVirtualServer>         virtualServers; // TODO wolud be nice if it was private

    private:
        int                 _backlog;
        std::string         _ip;
        std::string         _port;
        struct addrinfo     _hints;
        struct addrinfo*    _servinfo;
        /*
            Guardar internamente todo para su funcionamiento desde el t_confi. 
        */
        
        Server();      
        Server(const Server& rhs );
        Server& operator=(const Server& rhs);

        void    initSocket();


        class   socketException;
};

std::ostream &operator<<(std::ostream &os,  Server &obj);



#endif

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <cstring>
# include <Aux.hpp>
# include <cerrno>
# include <unistd.h>
# include <Confi.hpp>
# include <CGI.hpp>
# include <BaseHandler.hpp>
# include <FileReader.hpp>
# include <Parsing.hpp>


# include <Overseer.hpp>
# include <ProgramConfigs.hpp>
class Client; 

class Server : public BaseHandler
{
    public:
        Server(t_confi* confi);
        ~Server();

        int                 Action(int event);


        //Getters
        const int getFD() const;

        bool validateAction(Client& client);
        void getResponse(Client& client);
        bool checkTimeOut();
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



#endif

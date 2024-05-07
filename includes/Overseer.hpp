#ifndef OVERSEER_HPP
# define OVERSEER_HPP

# include <map>
# include <iostream>
# include <Confi.hpp>
# include <cerrno>
# include <Server.hpp>
# include <Client.hpp>
# include <poll.h>


# include <BaseHandler.hpp>

# define  MAX_FDS 1000

class CGI;

class Overseer
{
    public:
        static void     cleanOverseer();

        static  void    addToPfds(Client *);
        static  void    addToPfds(Server *);
        static  void    addToPfds(CGI *);



        static  void    saveServer(t_confi* confi);
        static  void    removeFromPFDS();


        static  void    handleAction(BaseHandler * , int );
        static  void    mainLoop();
        

    private:
        Overseer();
        ~Overseer();
        
        static  void    addToPfds(int new_fd, int events, int revents);

        static  std::map<int, BaseHandler *> _pending_fds;
        static  std::map<int, CGI *> _CGIs;
        static  std::map<int, Server *> _servers;
        static  std::map<int, Client *> _clients;

        //clients map
        static  std::size_t             _i;
        static  std::size_t             _fd_count;
        static  struct pollfd           _pfds[MAX_FDS];



        class pollException;



};




#endif



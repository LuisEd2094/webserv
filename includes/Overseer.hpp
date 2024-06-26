#ifndef OVERSEER_HPP
# define OVERSEER_HPP
class Overseer;
# include <map>
# include <iostream>
# include <Confi.hpp>
# include <cerrno>
# include "Server.hpp"
# include <poll.h>
# include <DeletedFiles.hpp>


# include <BaseHandler.hpp>

# include "ConfigVirtualServer.hpp"


class Overseer
{
    public:
        static  void     cleanOverseer(int);

        static  bool    addToPfds(BaseHandler *);
        static  bool    addToPFDSJustOut(BaseHandler *, int);
        static  void    removeInCGIPipe(int fd);

        static  void            addToDeleted(const std::string&);
        static  void            removeFromDeleted(const std::string&);
        static  bool            checkIfDeleted(const std::string&);
        static  deletedFiles    files; 


        static  void    setListenAction(int, int);



        static  Server *saveServer(t_confi* confi);
        static  void    removeFromPFDS(BaseHandler *);

        static  BaseHandler* getObj(int);

        static  void    printConfig(void);
        static  bool    handleAction(BaseHandler * , int );
        static  void    mainLoop();
        static  bool    canContinue;

        static  std::list<Server *> servers;

    private:
        Overseer();
        ~Overseer();
        
        static  bool    addToPfds(int new_fd, int events, int revents);

        static  std::map<int, BaseHandler *> _pending_fds;
        //clients map
        static  std::size_t             _i;
        static  std::size_t             _fd_count;
        static  struct pollfd           _pfds[MAX_FDS];
        class pollException;
};
#endif



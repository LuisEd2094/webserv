#ifndef OVERSEER_HPP
# define OVERSEER_HPP

# include <map>
# include <iostream>
# include <Confi.hpp>
# include <cerrno>
# include <Server.hpp>
# include <poll.h>


# include <BaseHandler.hpp>

# define    MAX_FDS 1000
# define    TIME_OUT_POLL -1
# define    TIME_OUT_PROCESS 1500
# define    SEND_SIZE 8000
# define    RECV_SIZE 8000
# define    IN_AND_OUT POLLIN | POLLOUT | POLLHUP
# define    JUST_IN POLLIN | POLLHUP

class Overseer
{
    public:
        static  void     cleanOverseer(int);

        static  void    addToPfds(BaseHandler *);


        static  void    setListenAction(int, int);



        static  void    saveServer(t_confi* confi);
        static  void    removeFromPFDS(BaseHandler *);

        static  BaseHandler* getObj(int);


        static  void    handleAction(BaseHandler * , int );
        static  void    mainLoop();
        

    private:
        Overseer();
        ~Overseer();
        
        static  void    addToPfds(int new_fd, int events, int revents);

        static  std::map<int, BaseHandler *> _pending_fds;
        //clients map
        static  std::size_t             _i;
        static  std::size_t             _fd_count;
        static  struct pollfd           _pfds[MAX_FDS];



        class pollException;



};




#endif



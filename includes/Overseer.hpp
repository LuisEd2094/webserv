#ifndef OVERSEER_HPP
# define OVERSEER_HPP

# include <map>
# include <iostream>
# include <Confi.hpp>
# include <cerrno>
# include <Server.hpp>
# include <poll.h>

# define  MAX_FDS 1000

class Overseer
{
    public:
        Overseer();
        ~Overseer();
        void saveServer(t_confi* confi);
        void mainLoop();

    private:
        std::map<int, Server *> _servers;
        //clients map
        int                     _fd_count;
        struct pollfd           *_pfds;
        Overseer(const Overseer& rhs);
        Overseer& operator= (const Overseer& rhs);

        void addToPfds(int new_fd);


        class pollException;



};

#endif
#ifndef OVERSEER_HPP
# define OVERSEER_HPP

# include <map>
# include <iostream>
# include <Confi.hpp>
# include <cerrno>
# include <Server.hpp>
# include <Client.hpp>
# include <poll.h>

# define  MAX_FDS 1000

class Overseer
{
    public:
        Overseer();
        ~Overseer();
        void    saveServer(t_confi* confi);
        Client* createClient(Server * server);

        void    handleClientAction(Client * client, int action);
        void    mainLoop();

    private:
        std::map<int, Server *> _servers;
        std::map<int, Client *> _clients;

        //clients map
        int                     _i;
        int                     _fd_count;
        struct pollfd           *_pfds;
        Overseer(const Overseer& rhs);
        Overseer& operator= (const Overseer& rhs);

        void addToPfds(int new_fd, int events, int revents);
        void removeFromPFDS();


        class pollException;



};

#endif

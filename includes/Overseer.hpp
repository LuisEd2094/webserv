#ifndef OVERSEER_HPP
# define OVERSEER_HPP

# include <map>
# include <Server.hpp>


class Overseer
{
    public:
        Overseer();
        ~Overseer();
        void saveServer(t_confi* confi);

        void test();

    private:
        std::map<int, Server *> _servers;
        Overseer(const Overseer& rhs);
        Overseer& operator= (const Overseer& rhs);



};

#endif
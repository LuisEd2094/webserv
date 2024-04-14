#include <Overseer.hpp>

//Exceptions

//Public
Overseer::Overseer() {}
Overseer::~Overseer()
{
    std::map<int, Server *>::iterator it = _servers.begin();
    for (; it != _servers.end(); it++)
    {
        delete it->second;
    }
    _servers.clear();
}


void Overseer::saveServer(t_confi* confi)
{

    Server *server = new Server(confi);
    _servers[server->getSocket()] = server;
}

void Overseer::test()
{
    std::map<int, Server *>::iterator it = _servers.begin();
    for (; it != _servers.end(); it++)
    {
        std::cout << it->second->getSocket() << std::endl;
    }

}

//Private
Overseer::Overseer(const Overseer& rhs){*this = rhs;}
Overseer& Overseer::operator= (const Overseer& rhs) {(void)rhs; return *this;}
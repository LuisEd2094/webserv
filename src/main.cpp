#include <iostream>
#include <Aux.hpp>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/poll.h>

#include <Overseer.hpp>
#include <vector>
#include <netinet/in.h>
#include <signal.h>
#include <ParsingGlobal.hpp>
#include <Overseer.hpp>

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        exitError("Incorrect usage, run: ./webserv [config_file] and try again");
    }
    t_confi confi;
    FileHandler::initTypeMaps();
    Response::initDefaultMap();
    Response::initErrorsHttp();
    Response::initErrorsMap();

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, Overseer::cleanOverseer);
    (void)argv;
    try
    {        
        ParsingGlobal parser = ParsingGlobal::parseFromFile(argc == 2 ? argv[1] : "./configFile/ok/file.config");
        ConfigElement::configure(parser);
        std::list<Server* > sdaf = Overseer::servers;
        std::cout << GREEN << " ::::: CONFIGURED :::::" << Overseer::servers.size()  << END << std::endl;
        Overseer::printConfig();
        Overseer::mainLoop();
    }
    catch(const std::exception& e)
    {

        std::cerr << e.what() << '\n';
        Overseer::cleanOverseer(-1);
    }
    
    return 0;
}

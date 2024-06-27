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
// #include <Server.hpp>


/*
    Classes for when we need to validate data before setting or getting. 

    Structs for when we just need to save or manipulate without the need of validation
*/

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

    // all this info should come from the confi file
    /*
    std::memset(&(confi.hints), 0, sizeof(confi.hints));
    confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
    confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    confi.hints.ai_flags = AI_PASSIVE;
    confi.port = "8080";
    confi.ip = "";
    confi.backlog = 200;*/
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, Overseer::cleanOverseer);
    // Read https://github.com/LuisEd2094/webservfrom file, create server, save server to overseer;
    try
    {        
        ParsingGlobal parser = ParsingGlobal::parseFromFile(argc == 2 ? argv[1] : "./src/Parsing/config_file.conf");
        //parser.recursivePrint();
        ConfigElement::configure(parser);
        std::list<Server* > sdaf = Overseer::servers;
        std::cout << GREEN << " ::::: CONFIGURED :::::" << Overseer::servers.size()  << END << std::endl;
        Overseer::printConfig();
        Overseer::mainLoop();
        //overseer.file(argv[1]);
        // Should read from file or default info, internally call saveServer for each server
        // throws exception when server fails
        // Overseer::saveServer(&confi);
        
/*         std::memset(&(confi.hints), 0, sizeof(confi.hints));
        confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
        confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
        confi.hints.ai_flags = AI_PASSIVE;
        confi.port = "81";
        confi.backlog = 10;
        overseer.saveServer(&confi); */

        // Overseer::mainLoop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        Overseer::cleanOverseer(-1);
    }
    
    return 0;
}

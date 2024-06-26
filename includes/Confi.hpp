#ifndef CONFI_HPP
# define CONFI_HPP

# include <iostream>
# include <netdb.h>
/*
    https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    Hints struct and func info
*/

typedef struct s_confi
{
    std::string     port;
    std::string     ip;
    /*
        Todas las clases, listas, ect, para este servidor real. 
    */

    int             socket;
    int             backlog;
    struct addrinfo hints;
    struct addrinfo *servinfo;
} t_confi;


#endif

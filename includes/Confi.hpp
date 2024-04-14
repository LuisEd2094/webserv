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
    int             socket;
    int             backlog;
    struct addrinfo hints;
    struct addrinfo *servinfo;
} t_confi;


#endif
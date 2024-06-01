#ifndef PROGRAM_CONFIGS_HPP
# define PROGRAM_CONFIGS_HPP

# include <Response.hpp>

# define    MAX_FDS 1000
# define    TIME_OUT_POLL 500
# define    TIME_OUT_PROCESS 1500
# define    SEND_SIZE 8000
# define    RECV_SIZE 1
# define    IN_AND_OUT POLLIN | POLLOUT | POLLHUP
# define    JUST_IN POLLIN | POLLHUP
# define    JUST_OUT POLLOUT | POLLHUP
# define    KEEP_ALIVE false


# define NUM_OBJ 3
/* # define FILE_OBJ "file"
# define DIRECT_OBJ "direct"
# define CGI_OBJ "cgi" */

enum ObjectTypes
{
    NOT_SET,
    FILE_OBJ,
    DIRECT_OBJ,
    CGI_OBJ
};

#endif
/* 
GET /nolen.py HTTP/1.1
Host: localhost

GET /nolen.py HTTP/1.1
Host: localhost

GET /index.html HTTP/1.1
Host: localhost

 */

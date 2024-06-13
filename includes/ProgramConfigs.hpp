#ifndef PROGRAM_CONFIGS_HPP
# define PROGRAM_CONFIGS_HPP

# include <Response.hpp>

# define    MAX_FDS 1000
# define    TIME_OUT_POLL 500
# define    TIME_OUT_PROCESS 1500
# define    SEND_SIZE 8000
# define    RECV_SIZE 8000
# define    BUFFER_SIZE RECV_SIZE * 10
# define    IN_AND_OUT POLLIN | POLLOUT | POLLHUP
# define    JUST_IN POLLIN | POLLHUP
# define    JUST_OUT POLLOUT | POLLHUP
# define    KEEP_ALIVE false
# define    MAX_URI_OFFSET 100



# define NUM_OBJ 5
/* # define FILE_OBJ "file"
# define NO_FD_OBJ "direct"
# define CGI_OBJ "cgi" */

enum ObjectTypes
{
    NOT_SET,
    FILE_OBJ,
    NO_FD_OBJ,
    CGI_OBJ,
    DIR_OBJ
};

static std::string ObjectTypesStrings[6] = {
    "NOT_SET",
    "FILE_OBJ",
    "NO_FD_OBJ",
    "CGI_OBJ",
    "DIR_OBJ"
};

/*
std::ostream& operator<<(std::ostream &os, ObjectTypes obj)
{
    os << sdafString[obj];
    return (os);
}
*/
#endif
/* 
GET /nolen.py HTTP/1.1
Host: localhost

GET /nolen.py HTTP/1.1
Host: localhost

GET /index.html HTTP/1.1
Host: localhost

 */

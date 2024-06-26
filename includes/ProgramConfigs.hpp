#ifndef PROGRAM_CONFIGS_HPP
# define PROGRAM_CONFIGS_HPP

# include <Response.hpp>

# define    MAX_FDS 5000
# define    TIME_OUT_POLL 2000
/* Time out process has to be a bit longer if I want to be able to send a response */
# define    TIME_OUT_PROCESS 10000
# define    SEND_SIZE 8000
# define    RECV_SIZE 8000
# define    BUFFER_SIZE RECV_SIZE * 10
# define    IN_AND_OUT POLLIN | POLLOUT | POLLHUP
# define    JUST_IN POLLIN | POLLHUP
# define    JUST_OUT POLLOUT | POLLHUP
# define    KEEP_ALIVE true
# define    MAX_URI_OFFSET 100
# define    MAX_URL_SIZE 500
# define    MAX_BODY_SIZE 10000000
# define    MAX_METAVAR 100
# define    PARSING_NOT_FOUND "not found"


# define NUM_OBJ 6

enum ObjectTypes
{
    NOT_SET,
    FILE_OBJ,
    NO_FD_OBJ,
    CGI_OBJ,
    DIR_OBJ,
    REDIRECT_OBJ
};

static std::string ObjectTypesStrings[6] = {
    "NOT_SET",
    "FILE_OBJ",
    "NO_FD_OBJ",
    "CGI_OBJ",
    "DIR_OBJ",
    "REDIRECT_OBJ"
};

#endif

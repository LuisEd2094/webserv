#ifndef PROGRAMCONFIGS_HPP
# define PROGRAMCONFIGS_HPP

# define    MAX_FDS 1000
# define    TIME_OUT_POLL -1
# define    TIME_OUT_PROCESS 1500
# define    SEND_SIZE 8000
# define    RECV_SIZE 8000
# define    IN_AND_OUT POLLIN | POLLOUT | POLLHUP
# define    JUST_IN POLLIN | POLLHUP


# define NUM_OBJ 3
# define FILE_OBJ "file"
# define DIRECT_OBJ "direct"
# define CGI_OBJ "cgi"

#endif
/* 
GET / HTTP/1.1
Host: localhost

GET / HTTP/1.1
Host: localhost

GET / HTTP/1.1
Host: localhost

 */
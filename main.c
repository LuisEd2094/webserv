#include <unistd.h>

int main()
{
        char* argv[3];
        argv[0] = "/usr/bin/python3";
        argv[1] = "/workspaces/webserv/mysite/nolen.py";
        argv[2] = NULL;

    execve("/usr/bin/python3", argv, NULL);

}
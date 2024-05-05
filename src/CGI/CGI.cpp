#include <CGI.hpp>
#include <Overseer.hpp>


//Exception
class CGI::CGIException : public std::exception
{
    private:
        std::string _err;
    public:
        CGIException(const std::string& err) : _err(err) {};
        ~CGIException() throw() {};
        const char *what() const throw()
        {
            return (_err.c_str());
        }
};


void CGI::createNewCGI(const Client& client, std::string& message, const std::string& url, Overseer * overseer)
{
    CGI *new_cgi = new CGI(client, message, url);
    overseer->saveCGI(new_cgi);
    //exit(0);
}

void CGI::destroyCGI(CGI *cgi)
{
    delete cgi;
}

int CGI::getSocket()
{
    return _pipe[0];
}

int CGI::readPipe()
{
    char buff[RECV_SIZE];

    read(_pipe[0], buff, sizeof(buff));

    std::string buffer;
    buffer.append(buff);

    std::cout << buffer ;
    std::cout << std::endl;
    _client_message.append(buff);
    std::cout << std::endl;


}


CGI::CGI(const Client& client, std::string& message, const std::string& url) : _client(client), _client_message(message)
{
    if (pipe(_pipe))
    {
        throw CGIException(strerror(errno));
    }
    pid_t	pid = fork();
    if (pid == -1)
    {
        throw CGIException(strerror(errno));
    }
    if (pid == 0)
    {
/*         signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL); */
        std::string cgi_path = "/home/luis/proyects/webserv/CGI" + url;
        char* argv[3];
        argv[0] = const_cast<char*>("/usr/bin/python3");
        argv[1] = const_cast<char*>(cgi_path.c_str()); // Convert const char* to char*
        argv[2] = NULL; // Null-terminate the array
        //close(_pipe[0]);
        //dup2(_pipe[1], STDOUT_FILENO);

		close(_pipe[0]);
		dup2(_pipe[1], STDOUT_FILENO);
		close(_pipe[1]);
        if (execve("/usr/bin/python3", argv, NULL) == -1)
        {
            throw CGIException(strerror(errno));
        }
        exit(-1);
    }
    //exit(0);
  

}
CGI::~CGI()
{

}
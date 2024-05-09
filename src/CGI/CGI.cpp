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

CGI::CGI(Client& client) : _client_fd(client.getFD())
{
    if (pipe(_pipe))
    {
        throw CGIException(strerror(errno));
    }
    _pid = fork();
    if (_pid == -1)
    {
        throw CGIException(strerror(errno));
    }
    if (_pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        std::string cgi_path = "/Users/lsoto-do/core05/server/CGI" + client.getURL();
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
        std::exit(-1);
    }
	close(_pipe[1]);
}


CGI::~CGI()
{

}

void CGI::createNewCGI(Client& client)
{
    CGI *new_cgi = new CGI(client);
    Overseer::addToPfds(new_cgi);
    //exit(0);
}


int CGI::Action(int event)
{
    char buff[RECV_SIZE];
    int status = 0;

    std::string buffer;
    (void)event;


    while (read(_pipe[0], buff, sizeof(buff)) > 0)
    {
        buffer.append(buff);
        if (buffer.find("\n\n") != std::string::npos)
        {
            Client * client = dynamic_cast<Client*>(Overseer::getObj(_client_fd));
            if (client)
            {
                client->setHTTPResponse(buffer.substr(0, buffer.find("\n\n") + 2));
                client->setBodyResponse(buffer.substr(buffer.find("\n\n") + 2));
            }


        }
    }

    waitpid(_pid, &status, WNOHANG);

    //_client_message.append(buff);
    close(_pipe[0]);
    return 0;
}


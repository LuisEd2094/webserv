#include <CGI.hpp>
#include <Overseer.hpp>
#include <Client.hpp>


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
        //signal(SIGINT, SIG_DFL);
        //signal(SIGQUIT, SIG_DFL);
        std::string cgi_path = "/home/luis/proyects/webserv/CGI" + client.getURL();
        char* argv[3];
        argv[0] = const_cast<char*>("/usr/bin/python3");
        argv[1] = const_cast<char*>(cgi_path.c_str()); // Convert const char* to char*
        argv[2] = NULL; // Null-terminate the array
        //close(_pipe[0]);
        //dup2(_pipe[1], STDOUT_FILENO);


		close(_pipe[0]);
		dup2(_pipe[1], STDOUT_FILENO);
        close(_pipe[1]);

        execve("/usr/bin/python3", argv, NULL);
        std::cout << "HTTP/1.1 500 Internal Server Error\r\n" << std::endl;
        std::exit(-1);
        // try
        // {
        //     execve("/usr/bin/python3", argv, NULL);
        //     std::exit(0);
        // }
        // catch (const std::exception& e)
        // {
        //     throw CGIException(e.what() + std::string(strerror(errno)));
        //     std::exit(-1);

        // }

    }
	close(_pipe[1]);
}


CGI::~CGI()
{
    int status;

    kill(_pid, SIGTERM);
    waitpid(_pid, &status, 0);
    //_client_message.append(buff);
    close(_pipe[0]);
}

CGI* CGI::createNewCGI(Client& client)
{
    CGI *new_cgi = new CGI(client);
    Overseer::addToPfds(new_cgi);
    //exit(0);
    return new_cgi;
}


bool    CGI::checkTimeOut()
{
    time_t current_time;
    std::time(&current_time);

    double seconds;
    seconds = std::difftime(current_time, _last_time) * 1000;

    if (TIME_OUT_PROCESS > 0 && seconds >= TIME_OUT_PROCESS)
    {

        Client * client = dynamic_cast<Client*>(Overseer::getObj(_client_fd));
        if (client)
        {
            client->setHTTPResponse("HTTP/1.1 408 Request Timeout\r\n", this);
        }        
        return true;

    }
    return false;
} 


int CGI::Action(int event)
{
    char buff[RECV_SIZE];
    int  result = read(_pipe[0], buff, sizeof(buff)); 
    (void)event;
    if (result > 0)
    {
        _buffer.append(buff, result);
        return (1);
    }
    else if (result <= 0)
    {
        Client * client = dynamic_cast<Client*>(Overseer::getObj(_client_fd));
        if (client)
        {
            if (result ==  0)
            {
                std::string http_response(_buffer.substr(0, _buffer.find("\n\n") + 2));
                if (http_response.empty())
                {
                    client->setHTTPResponse("HTTP/1.1 200 OK\r\n\r\n", this);
                }
                else
                {
                    client->setHTTPResponse(http_response, this);
                    std::string body_resposne(_buffer.substr(_buffer.find("\n\n") + 2));
                    if (!body_resposne.empty())
                    {
                        client->setBodyResponse(body_resposne, this); 
                    }
                }
            }
            else
            {
                client->setHTTPResponse("HTTP/1.1 500 Internal Server Error\r\n", this);
            }
            return (0);

        }
    }
    return (1);
}
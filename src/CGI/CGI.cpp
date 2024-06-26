#include <CGI.hpp>
#include <Overseer.hpp>
#include <Client.hpp>


void CGI::changeDir(const std::string& fileUrl)
{
    std::size_t pos = fileUrl.find_last_of("/");
    std::string new_path;
    /*If pos == 0 then path is in root, so we can't do a substr*/
    if (pos != 0)
    {
        new_path = fileUrl.substr(0, pos);
    }
    else
    {
        new_path = "/";
    }
    chdir(new_path.c_str());
}

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



CGI::CGI(Client& client) :  BaseHandler(client),
                            _has_error(false),
                            _defaultHttp(client.getDefaultHttpResponse()),
                            _body(client.getBody()),
                            _len(client.getContentLength()),
                            _sent(0)
{
    if (pipe(_out_pipe))
    {
        client.setRespondeCode(BAD_GATEWAY);
        throw CGIException(strerror(errno));
    }
    if (!_body.empty() && pipe(_in_pipe))
    {
        close(_out_pipe[0]);
        close(_out_pipe[1]);
        client.setRespondeCode(BAD_GATEWAY);
        throw CGIException(strerror(errno));
    }
    _pid = fork();
    if (_pid == -1)
    {
        close(_out_pipe[0]);
        close(_out_pipe[1]);
        if (!_body.empty())
        {
            close(_in_pipe[0]);
            close(_in_pipe[1]);
        }
        client.setRespondeCode(BAD_GATEWAY);
        throw CGIException(strerror(errno));
    }
    if (_pid == 0)
    {
        std::string file = client.getPathFile().getFile();
        std::string exec = client.getExecute();
        std::string dir  = client.getPathFile().getDir();
        chdir(dir.c_str());
        char* argv[3];
        argv[0] = const_cast<char*>(exec.c_str());
        argv[1] = const_cast<char*>(file.c_str());
        argv[2] = NULL;



        std::string query = "QUERY_STRING=" + client.getMapValue("__Query__");
        char *env[3];
        env[0] = const_cast<char*>(query.c_str());
        std::string len;
        if (!_body.empty())
        {
            /*I don't understand why I can't do this in a single line*/
            len = std::string("CONTENT_LENGTH=" + toString(client.getContentLength())).c_str();
            env[1] = const_cast<char*>(len.c_str());
        }
        else
            env[1] = NULL;
        env[2] = NULL;

		close(_out_pipe[0]);
		dup2(_out_pipe[1], STDOUT_FILENO);
        if (!_body.empty())
        {
            close(_in_pipe[1]);
		    dup2(_in_pipe[0], STDIN_FILENO);
        }
        execve(exec.c_str(), argv, env);
        std::cerr << "Execve error" << std::endl;
        std::exit(-1);
    }
	close(_out_pipe[1]);
    if (!_body.empty())
    {
        close(_in_pipe[0]);
        Overseer::addToPFDSJustOut(this, _in_pipe[1]);
    }
}


CGI::~CGI()
{
    int status;

    kill(_pid, SIGTERM);
    waitpid(_pid, &status, 0);
    close(_out_pipe[0]);
}

BaseHandler* CGI::createNewCGI(Client& client)
{
    CGI * new_cgi;
    
    new_cgi = new CGI(client);
    Overseer::addToPfds(new_cgi);
    return new_cgi;
}


bool    CGI::checkObjTimeOut()
{
    if (checkTimeOut())
    {

        Client * client = dynamic_cast<Client*>(Overseer::getObj(_client_fd));
        if (client)
        {
            client->addErrorFileHandlerToExistingRequest(this, this->getErrorResponse(GATEWAY_TIMEOUT));
        }        
        return true;

    }
    return false;
} 


int CGI::Action(int event)
{
    if (_has_error)
    {
        Client * client = dynamic_cast<Client*>(Overseer::getObj(_client_fd));
        if (client)
            client->addErrorFileHandlerToExistingRequest(this, this->getErrorResponse(INTERNAL_SERVER_ERROR));
        return (-1);
    }
    if (event & POLLIN || event & POLLHUP)
    {
        char buff[RECV_SIZE];
        int  result = read(_out_pipe[0], buff, sizeof(buff)); 
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
                        http_response = _buffer.substr(0, _buffer.find("\r\n\r\n") + 4);
                    }
                    if (http_response.empty())
                    {
                        client->addErrorFileHandlerToExistingRequest(this, this->getErrorResponse(BAD_GATEWAY));
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
                    client->addErrorFileHandlerToExistingRequest(this, this->getErrorResponse(INTERNAL_SERVER_ERROR));
                }
                return (0);
            }
            else 
            {
                return (0);
            }
        }
    }
    else if (event & POLLOUT)
    {
        int chunk_to_send = std::min(SEND_SIZE,(int)(_len - _sent));
        int result = write(_in_pipe[1], _body.c_str() + _sent, chunk_to_send);
        if (result <= 0)
        {
            Overseer::removeInCGIPipe(_in_pipe[1]);
            close(_in_pipe[1]);
            if (result < 0)
            {
                _has_error =true;
                return (1);
            }
            /*Not sure what to do if the write failes*/
            return (1);
        }
        _sent += result;
    }
    
    return (1);
}

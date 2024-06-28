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


void CGI::fill_arrays(std::vector<std::string>& env_strings,
            const Client& client)
{

    const ConfigCgi * confi = dynamic_cast<const ConfigCgi *>(_configElement);
    std::map<std::string, std::string> env_map;

    if (client.getMapValue("Content-Length") != PARSING_NOT_FOUND)
        env_map["CONTENT_LENGTH="] = client.getMapValue("Content-Length");

    if (client.getMapValue("Content-Type") != PARSING_NOT_FOUND)
        env_map["CONTENT_TYPE="] = client.getMapValue("Content-Type");
    env_map["GATEWAY_INTERFACE="]  =  "CGI/1.1";
    env_map["PATH_TRANSLATED="] = confi->getRoot();
    if (client.getMapValue("__Query__") != PARSING_NOT_FOUND)
        env_map["QUERY_STRING="] = client.getMapValue("__Query__");
    else
        env_map["QUERY_STRING="] = "";
    /*this two can be set to the same value*/
    env_map["REMOTE_ADDR="] = _client_ip;
    env_map["REMOTE_HOST="] = _client_ip;
    env_map["REQUEST_METHOD="] = client.getMethod();
    env_map["SERVER_NAME="] = client.getHost();
    
    if (client.getMapValue("Port") != PARSING_NOT_FOUND)
        env_map["SERVER_PORT="] = client.getMapValue("Port");
    else
        env_map["SERVER_PORT="] = "80";
    env_map["SERVER_PROTOCOL="] = "HTTP/1.1";
    
    const std::map<std::string, std::string>& metaVars = confi->getMetaVar();
    

    /*metaVars can be set to a default value in consfig file
        if it's not sent then we can check if key/vals inside the map are equal
        If they are then that means it was not set in the config
    */
    for (std::map<std::string, std::string>::const_iterator it = metaVars.begin();
        it != metaVars.end(); it++
    )
    {
        if (client.getMapValue(it->first) != PARSING_NOT_FOUND)
        {

            if (it->first.find("HTTP_") != 0)
                env_map["HTTP_" + it->first + "="] = client.getMapValue(it->first);
            else
                env_map[it->first + "="] = client.getMapValue(it->first);
        }
        else if (it->first != it->second)
        {
            if (it->first.find("HTTP_") != 0)
                env_map["HTTP_" + it->first + "="] = it->second;
            else
                env_map[it->first + "="] = it->second; 
        }
    }
    int i = 0;
    std::map<std::string, std::string>::const_iterator it = env_map.begin();
    for (; i < MAX_METAVAR - 1 && it != env_map.end(); ++i, it++)
    {
        env_strings.push_back(it->first + it->second);
    }
}


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
    
        char *argv[3];
        std::memset(argv, 0, sizeof(argv));
        std::string file = client.getPathFile().getFile();
        std::string exec = client.getExecute();
        std::string dir  = client.getPathFile().getDir();
        chdir(dir.c_str());
        argv[0] = const_cast<char*>(exec.c_str());
        argv[1] = const_cast<char*>(file.c_str());
        argv[2] = NULL;
        std::vector<std::string> env_strings;
        fill_arrays(env_strings, client);
        char *env[MAX_METAVAR];
        int i = 0;
        std::vector<std::string>::iterator it = env_strings.begin();
        
        for (; i < MAX_METAVAR - 1 && it != env_strings.end(); ++i, it++)
        {
            env[i] = const_cast<char*>(it->c_str());
        }
        env[i] = NULL;
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
                        std::string body_response(_buffer.substr(_buffer.find("\n\n") + 2));
                        if (!body_response.empty())
                        {
                            client->setBodyResponse(body_response, this); 
                        }
                        if (!body_response.empty() && http_response.find("Content-Length") == std::string::npos)
                        {
                            http_response.insert(http_response.find("\n") + 1, CONTENTLENGTH + toString(body_response.length()) + CRNL);
                        }
                        client->setHTTPResponse(http_response, this);


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
            return (1);
        }
        _sent += result;
    }
    
    return (1);
}

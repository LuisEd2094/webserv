#include <Client.hpp>


/*They return the same because they are the same, but the FileHandler and CGI will have their own FD and Client FD to later access the client*/
int Client::getFD() const
{
    return _fd;
}


int Client::getClientFD() const
{
    return _fd;
}

std::string Client::getHttpAnswerDirectory() const
{
    return this->_httpAnswerDirectory;
}

const std::string& Client::getURL() const
{
    return  _parser_http.getRequested();
}

ObjectTypes Client::getResponseType() const
{
    return _response_type;
}

const std::string& Client::getBody() const
{
    return _in_body;
}

Server* Client::getServer() const
{
    return _server;
}


const std::string&  Client::getPathFileString() const
{
    return _path_to_file_str;
}



const std::string&  Client::getMapValue(const std::string& value) const
{
    return _parser_http.getMapValue(value);

} 


const std::queue<std::string>& Client::getHTTPAddons() const
{
    return _http_addons;
}

const std::string& Client::getMethod() const
{
    return _parser_http.getMethod();
}

const std::string& Client::getHost() const
{
    return _parser_http.getMapValue("Host");
}     

Actions  Client::getAction() const
{
    return _action;
}

std::size_t Client::getContentLength() const
{
    return _content_length;
}

bool Client::getIsChunked() const
{
    return _is_chunked;
}

const ConfigElement *           Client::getConfigElement(void) const
{
    return this->_configElement;
}


bool Client::getURLempty() const
{
    return _redirection_urls.empty();
}
std::string              Client::getNextURLRedirect()
{
    if (!_redirection_urls.empty())
    {
        std::string res(_redirection_urls.top());
        _redirection_urls.pop();
        return res;
    }
    return "";
}


const Path&  Client::getExecute() const
{
    return _execute;
}



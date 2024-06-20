#include <FileReader.hpp>
#include <Overseer.hpp>
#include <Client.hpp>

std::map<std::string, std::string> FileReader::types;

//Exception
class FileReader::FileReaderException : public std::exception
{
    private:
        std::string _err;
    public:
        FileReaderException(const std::string& err) : _err(err) {};
        ~FileReaderException() throw() {};
        const char *what() const throw()
        {
            return (_err.c_str());
        }
};

void FileReader::initTypeMaps()
{
    types["empty"] = "";
    types["html"] = "text/html";
    types["htm"] = "text/html";

}

const std::string& FileReader::getMimeType(const std::string& to_find)
{
    std::map<std::string, std::string>::iterator found = types.find(to_find);

    if (found != types.end())
        return (found->second);
    return (types["empty"]);
}


FileReader::FileReader(Client& client) : BaseHandler(client),  _client_fd(client.getFD()), _defaultHttp(client.getDefaultHttpResponse())
{
    _fd = open(client.getPathFileString().c_str(), O_RDONLY);
    std::size_t start_ext = client.getPathFileString().find_last_of(".");
    
    if (start_ext != std::string::npos)
    {
        _file_type = getMimeType(client.getPathFileString().substr(start_ext + 1, client.getPathFileString().length()));
    }
    if (_fd == -1)
    {
        throw FileReaderException(strerror(errno));
    }
}


FileReader::~FileReader()
{
    /*If open fails, then we don't close it on destruction*/
    if (_fd > -1)
        close(_fd);
}

BaseHandler* FileReader::createNewFileReader(Client& client)
{
    FileReader *new_FileReader;
    try
    {
        new_FileReader = new FileReader(client);
        Overseer::addToPfds(new_FileReader);
        return new_FileReader;
    }
    catch (...)
    {
        /*Check if this is a new File reader then update?*/
        if (client.getErrorCode() == INTERNAL_SERVER_ERROR)
        {
            /*Consume the exception?*/
            return BaseHandler::createObject(Response::getDefault(client.getErrorCode()));
        }
        else
        {
            throw FileReaderException(strerror(errno));
        }
    }
}


int FileReader::getFD() const
{
    return _fd;
}

bool    FileReader::checkObjTimeOut()
{
    return false;
}

std::string  FileReader::setContentType(std::string http)
{
    if (!_file_type.empty())
    {
        http.append("Content-Type: " + _file_type  + CRNL); // needs charset = utf-8 to be able to decode // but it's enough if it's on the html itself. Should parse maybe?
    }
    return (http);
}

int FileReader::Action(int event)
{
    char    buff[RECV_SIZE];
    int     result = read(_fd, buff, sizeof(buff)); 
    (void)event;
    if (result > 0)
    {
        _buffer.append(buff, result);
        return (1);
    }
    else if (result <= 0)
    {
        Client *client = dynamic_cast<Client*>(Overseer::getObj(_client_fd));
        if (client)
        {
            if (result ==  0)
            {
                /*I should be setting this with setdefaultResponse*/
                _defaultHttp = setContentType(_defaultHttp);
                client->setHTTPResponse(setContentLenHTTP(_defaultHttp, _buffer) , this); 
                client->setBodyResponse(_buffer, this); 
            }
            else
            {
                client->setdefaultResponse(client->getServer()->getErrorResponseObject(INTERNAL_SERVER_ERROR), this);
            }
            return (0);

        }
        return (0);
    }
    return (0);

}


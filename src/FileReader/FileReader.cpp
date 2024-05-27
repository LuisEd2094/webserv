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


FileReader::FileReader(Client& client) : _client_fd(client.getFD())
{
    _fd = open("/Users/lsoto-do/core05/webserv/html/index.html", O_RDONLY);

    std::size_t start_ext = client.getURL().find_last_of(".");
    
    if (start_ext != std::string::npos)
    {
        _file_type = getMimeType(client.getURL().substr(start_ext + 1, client.getURL().length()));
    }

    if (_fd == -1)
    {
        throw FileReaderException(strerror(errno));
    }
   
}


FileReader::~FileReader()
{
    close(_fd);
}

FileReader* FileReader::createNewFileReader(Client& client)
{
    FileReader *new_FileReader = new FileReader(client);
    Overseer::addToPfds(new_FileReader);
    //exit(0);
    return new_FileReader;
}


const int FileReader::getFD() const
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
                std::string http = setContentType(std::string(HTTP_OK));
                if (_buffer.find_last_of("\n") != _buffer.length() - 1) /*Just to make sure there is an end of line */
                    _buffer.append("\n");
                client->setHTTPResponse(setContentLenHTTP(http, _buffer) , this); 
                client->setBodyResponse(_buffer, this); 
            }
            else
            {
                client->setHTTPResponse(setContentLenHTTP(INTERNAL_ERROR_HTTP, ""), this);
            }
            return (0);

        }
        return (0);
    }
        return (0);

}


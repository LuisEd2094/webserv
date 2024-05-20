#include <FileReader.hpp>
#include <Overseer.hpp>
#include <Client.hpp>


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

FileReader::FileReader(Client& client) : _client_fd(client.getFD())
{
    // checks url

    _fd = open("/Users/lsoto-do/core05/webserv/html/index.html", O_RDONLY);
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

bool    FileReader::checkTimeOut()
{
    return false;
    
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
                client->setHTTPResponse("HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n", this);
                client->setBodyResponse(_buffer, this); 
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


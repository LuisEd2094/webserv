#include <FileHandler.hpp>
#include <Overseer.hpp>
#include <Client.hpp>

std::map<std::string, std::string> FileHandler::types;

//Exception
class FileHandler::FileHandlerException : public std::exception
{
    private:
        std::string _err;
    public:
        FileHandlerException(const std::string& err) : _err(err) {};
        ~FileHandlerException() throw() {};
        const char *what() const throw()
        {
            return (_err.c_str());
        }
};

void FileHandler::initTypeMaps()
{
    types["empty"] = "";
    types["html"] = "text/html";
    types["htm"] = "text/html";
    types["md"] = "text/markdown";
    types["jpg"] = "image/jpeg";
    types["jpeg"] = "image/jpeg";
    types["mp3"] = "audio/mpeg";    
    types["js"] = "text/javascript";

}

const std::string& FileHandler::getMimeType(const std::string& to_find)
{
    std::map<std::string, std::string>::iterator found = types.find(to_find);

    if (found != types.end())
        return (found->second);
    return (types["empty"]);
}

void    FileHandler::openFile(const std::string& file_name, int mode, mode_t permissions)
{
    if (mode == O_RDONLY)
        _fd = open(file_name.c_str(), mode);
    else
        _fd = open(file_name.c_str(), mode, permissions);
    std::size_t start_ext = file_name.find_last_of(".");
    
    if (start_ext != std::string::npos)
    {
        _file_type = getMimeType(file_name.substr(start_ext + 1, file_name.length()));
    }
}

bool FileHandler::getIsWritting() const
{
    return _is_writting;
}

static bool fileExists(const std::string& path) 
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

FileHandler::FileHandler(BaseHandler& obj) : BaseHandler(obj),
        _refered("not found"),
        _body(""),
        _file_name(""),
        _boundary(""),
        _bytes_written(0),
        _file_len(0),
        _is_writting(false),
        _file_exists(false)
{
    try
    {
        const Client& client = dynamic_cast<const Client&>(obj);
        if (client.getAction() == GET)
        {
            openFile(client.getPathFileString(), O_RDONLY, 0);
            _file_name = client.getPathFile();
        }
        else if (client.getAction() == POST)
        {
            std::string content_type = client.getMapValue("Content-Type");
            _body = client.getBody();
            if (content_type != "not found")
            {
                std::size_t boundary_start = content_type.find("boundary=");
                if (boundary_start != std::string::npos)
                {
                    _boundary = content_type.substr(boundary_start + std::strlen("boundary="));
                }
            }
            _refered = client.getMapValue("Referer");            

            std::size_t file_name_start = _body.find("filename=\"");
            if (file_name_start != std::string::npos)
            {
                std::size_t file_name_len = std::strlen("filename=\"");
                std::size_t file_name_end = _body.find_first_of('"', file_name_len + file_name_start);


                _file_name = client.getPathFile().toStr() + _body.substr(file_name_start + file_name_len, file_name_end - (file_name_start + file_name_len));
            }
            else
            {
                /* some sort of default?*/
                _file_name = client.getPathFileString(); 
            }
            if (!_boundary.empty())
            {
                std::size_t end_boundary = _body.rfind(_boundary);
                std::size_t start_boundary = _body.find("\r\n\r\n");
                if (end_boundary != std::string::npos && start_boundary != std::string::npos)
                {
                    start_boundary += std::strlen("\r\n\r\n");
                    end_boundary -= 4;
                    _body = _body.substr(start_boundary, end_boundary - start_boundary);
                }
            }
            _file_len = _body.length();
            _file_exists = fileExists(_file_name.toStr());
            openFile(_file_name.toStr(), O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
            _is_writting = true;
        }
    }
    catch (const std::bad_cast & err)
    {
        _file_name = obj.getPathFile();
        openFile(obj.getPathFileString(), O_RDONLY, 0);
    }
    if (_fd == -1)
    {
        throw FileHandlerException(strerror(errno));
    }
}

FileHandler::~FileHandler()
{
    /*If open fails, then we don't close it on destruction*/
    if (_fd > -1)
        close(_fd);
}

BaseHandler* FileHandler::createNewFileHandler(BaseHandler& obj)
{
    FileHandler *new_FileHandler;
    try
    {
        new_FileHandler = new FileHandler(obj);
        if (new_FileHandler->getIsWritting())
            Overseer::addToPFDSJustOut(new_FileHandler, new_FileHandler->getFD());
        else
            Overseer::addToPfds(new_FileHandler);

        return new_FileHandler;
    }
    catch (...)
    {
        /*Check if this is a new File reader then update?*/
        if (obj.getErrorCode() == INTERNAL_SERVER_ERROR)
        {
            /*Consume the exception?*/
            return BaseHandler::createObject(Response::getDefault(obj.getErrorCode()));
        }
        else
        {
            obj.setRespondeCode(INTERNAL_SERVER_ERROR);
            return obj.getErrorResponse(obj.getErrorCode());
        }
    }
}

int FileHandler::getFD() const
{
    return _fd;
}

bool    FileHandler::checkObjTimeOut()
{
    return false;
}

std::string  FileHandler::setContentType(std::string http)
{
    if (!_file_type.empty())
    {
        http.append("Content-Type: " + _file_type  + CRNL); // needs charset = utf-8 to be able to decode // but it's enough if it's on the html itself. Should parse maybe?
    }
    return (http);
}

int FileHandler::readFromFile()
{
    char    buff[RECV_SIZE];
    int     result = read(_fd, buff, sizeof(buff)); //-1;//
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
                const ConfigLocation * confi = dynamic_cast<const ConfigLocation *>(_configElement);
                (void )confi;
                //std::string file_name = ->
                _defaultHttp = setContentType(_defaultHttp);
                client->setHTTPResponse(setContentLenHTTP(_defaultHttp, _buffer) , this); 
                client->setBodyResponse(_buffer, this);
                return (0);
            }
            else
            {
                if (_error_code != INTERNAL_SERVER_ERROR)
                    client->addErrorFileHandlerToExistingRequest(this, this->getErrorResponse(INTERNAL_SERVER_ERROR));
                else
                    client->addErrorFileHandlerToExistingRequest(this, BaseHandler::createObject(Response::getDefault(_error_code)));
                return (-1);
            }
        }
        return (0);
    }
    return (0);
}

int FileHandler::writeToFIle()
{
        int     result = write(_fd, _body.c_str() + _bytes_written, std::min(SEND_SIZE, (int)_file_len - (int)_bytes_written));

        if (result <= 0)
        {
            Client *client = dynamic_cast<Client*>(Overseer::getObj(_client_fd));
            if (result < 0)
            {
                if (client)
                    client->addErrorFileHandlerToExistingRequest(this, this->getErrorResponse(INTERNAL_SERVER_ERROR));
                return (-1);
            }
            else
            {
                if (Overseer::checkIfDeleted(_file_name.toStr()))
                {
                    Overseer::removeFromDeleted(_file_name.toStr());
                }
                if (client)
                {
                    const ConfigLocation *confi = dynamic_cast<const ConfigLocation*>(_configElement);
                    (void) confi;
                    if (_file_exists)
                        _error_code = OK;
                    else
                        _error_code = CREATED;
                    
                    std::string file_location = confi->get__elemArg__() + _file_name.getFile().toStr().erase(0,1);


                    std::string		html("<!DOCTYPE html>\n");
                    html = html + "<html><head></head><body>";	
                    html = html + "<h1>You can see you file clicking here</h1><hr>";	
                    html = html + "<ul>";
                    html = html + "<li><a href=\"" + file_location + "\">" + file_location + "</a></li>";
                    html += "</ul><hr></body></html>";

                    std::string http = Response::getHttpFirtsLine(_error_code) + "Location: " + file_location + CRNL;
                    _file_type = getMimeType("html");
                    http = setContentType(http);
                    http = setContentLenHTTP(http, html);
                    client->setHTTPResponse(http, this); 
                    client->setBodyResponse(html, this);
                    
                }
                return (0);

            }
        }
        _bytes_written += result;
        return (1);
    return (0);
}

int FileHandler::Action(int event)
{
    if (event & POLLIN)
    {
        return readFromFile();
    }
    else if (event & POLLOUT)
    {
        return writeToFIle();
    }
    return (-1);
}


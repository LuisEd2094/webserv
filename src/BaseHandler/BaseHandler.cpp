#include <BaseHandler.hpp>
#include <CGI.hpp>
#include <FileHandler.hpp>
#include <DirectResponse.hpp>
#include <Client.hpp>

const ObjectTypes BaseHandler::valid_objs[NUM_OBJ] = {
    FILE_OBJ,
    NO_FD_OBJ,
    CGI_OBJ,
    DIR_OBJ,
    REDIRECT_OBJ,
 };


void    BaseHandler::setTime()
{
    std::time(&_last_time);
    // should return error if clock fails
}

void    BaseHandler::setRespondeCode(ResponseCodes code)
{
    _defaultHttp = Response::getHttpFirtsLine(code);
    _error_code = code;
}

const std::string dirList(const std::string& dir_str, const std::string& url)
{
	DIR				*dir;
	struct dirent	*file;
	std::string		html("<!DOCTYPE html>\n");
    Path            new_url(url + "/");
	//chdir(directory.c_str()); //TODO try catch
	dir = opendir(dir_str.c_str()); //TODO try catch
    if (!dir)
        throw std::exception();
    /* this throws seg fault if we limit the fd*/
    errno = 0;
	file = readdir(dir);
    if (!file && errno == EBADF)
    {
    	closedir(dir);
        throw std::exception();
    }
	html = html + "<html><head></head><body>";	
	html = html + "<h1>Directory listing for " + dir_str + "</h1><hr>";	
	html = html + "<ul>";
    while (file)
	{
		if (!Overseer::checkIfDeleted(dir_str + (file->d_name)) && std::string(file->d_name) != "." && std::string(file->d_name) != "..")
			html = html + "<li><a href=\"" + new_url.toStr() + file->d_name + "\">" + file->d_name + "</a></li>";
        errno = 0;
        file = readdir(dir);
        if (!file && errno == EBADF)
        {
        	closedir(dir);
            throw std::exception();
        }
	}
	html += "</ul><hr></body></html>";
	closedir(dir);
	return html;
}

BaseHandler* BaseHandler::createObject(BaseHandler& obj)
{
    /*If you are base then you are either a FileHandler or a CGI, so you got here through error creation*/
    return FileHandler::createNewFileHandler(obj);
}


BaseHandler* BaseHandler::createObject(Client& client)
{
    //Factory creator, requires string to identify it and client that'd be assigned to that Object
    for (std::size_t i = 0; i < NUM_OBJ; ++i)
    {
        if (client.getResponseType() == valid_objs[i])
        {
            if (valid_objs[i] == FILE_OBJ)
                return FileHandler::createNewFileHandler(client);
            else if (valid_objs[i] == CGI_OBJ)
                return CGI::createNewCGI(client);
            else if (valid_objs[i] == DIR_OBJ)
            {
                std::string listing = dirList(client.getPathFileString(), client.getURL());
                return DirectResponse::createNewDirect(setContentLenHTTP(std::string(HTTP_OK) + "Content-Type: text/html\r\n", listing), listing);
            }
            else if (valid_objs[i] == REDIRECT_OBJ)
            {
                std::string body(REDIRECT_TEMPLATE);

                std::size_t body_pos(body.find("<body>"));

                const ConfigLocation * config = dynamic_cast<const ConfigLocation *>(client.getConfigElement());
                if (!config)
                {
                    throw ConfigElement::ParamError("No config set");
                }
                const std::list<Uri>&  redirections = config->getRedirections();
                std::string http = Response::getHttpFirtsLine(config->getCodeRedirections());

                for (std::list<Uri>::const_iterator it = redirections.begin();it != redirections.end(); it++)
                {
                    std::string new_location = (std::string)*it;
                    std::string a = "<p><a href=\"";

                    a.append(new_location + "\">" + new_location + "</a></p>\n");


                    http.append("Location :" + new_location + CRNL);
                    body.insert(body_pos + std::strlen("<body>"),a );

                }
                return DirectResponse::createNewDirect(setContentLenHTTP(http, body), body);
            }
            else if (valid_objs[i] == NO_FD_OBJ)
            {
                return DirectResponse::createNewDirect(setContentLenHTTP(client.getDefaultHttpResponse(), client.getBody() ), client.getBody());
            }
            else
                return BaseHandler::createObject(Response::getDefault(INTERNAL_SERVER_ERROR));
        }
    }
    return BaseHandler::createObject(Response::getDefault(INTERNAL_SERVER_ERROR));
}
BaseHandler* BaseHandler::createObject(const defaultResponse & obj)
{
    // This is for when you want to generate a DirectResponse Object, 
    // it needs a HTTP and Body string to create it. 
    return DirectResponse::createNewDirect(obj._http, obj._body);

}

int BaseHandler::getClientFD() const
{
    return _client_fd;
}

ResponseCodes BaseHandler::getErrorCode() const
{
    return _error_code;
}
const std::string& BaseHandler::getPathFileString() const
{
    return _path_to_file_str;
}

const std::string&  BaseHandler::getDefaultHttpResponse() const
{
    return _defaultHttp;
}


const ConfigElement* BaseHandler::getConfigElement() const
{
    return _configElement;
}

bool    BaseHandler::checkTimeOut()
{
    time_t current_time;
    std::time(&current_time);

    double seconds;
    seconds = std::difftime(current_time, _last_time) * 1000;
    return (TIME_OUT_POLL > 0 && seconds >= TIME_OUT_PROCESS);
}

BaseHandler* BaseHandler::getErrorResponse(ResponseCodes code)
    {
    /*Every Error we send should pass through here, if anyone that's NOT the client calls this function
    they should set _configElement to their respective configElement*/
    const ConfigLocation* location = dynamic_cast<const ConfigLocation*>(_configElement);

    if (location == NULL)
    {
        /*If location == NULL it means we didn't go through server to get the location,
            for example, if HTTP fails, we can't use it to get the server info, so we just send a generic response
            Location is reset to NULL after sending an answer
        */
        const ConfigCgi* cgi = dynamic_cast<const ConfigCgi *>(_configElement);
        
        /*Everything up to here is working, but cgi->location has nothing. Setting CGI to null so it goes through default to test other logics*/


        if (cgi != NULL)
        {
            const ConfigLocation& location  = cgi->getLocation();
            const std::string& errr = location.getErrorPage(code);
            if (!errr.empty())
            {
                _response_type = FILE_OBJ;
                _error_code = code;
                _defaultHttp = Response::getHttpFirtsLine(code);
                _path_to_file_str = errr;
                return BaseHandler::createObject(*this);
            }  
        }

        return BaseHandler::createObject(Response::getDefault(code));
    }
    const std::string& errr = location->getErrorPage(code);
    if (!errr.empty())
    {
        _response_type = FILE_OBJ;
        _error_code = code;
        _defaultHttp = Response::getHttpFirtsLine(code);
        _path_to_file_str = errr;
        return BaseHandler::createObject(*this);
    }
    else
    {
        return BaseHandler::createObject(Response::getDefault(code));
    }  
}

const Path&  BaseHandler::getPathFile() const
{
    return _path_to_file;
}

BaseHandler::BaseHandler():
    _configElement(NULL),
    _response_type(NOT_SET),
    _error_code(INVALID_CODE),
    _defaultHttp(),
    _client_ip(""),
    _path_to_file_str(),
    _client_fd(0),
    _fd(0)
{
    setTime();
}


BaseHandler::BaseHandler(const BaseHandler& obj) : 
    _configElement(obj._configElement),
    _response_type(obj._response_type),
    _error_code(obj._error_code),
    _defaultHttp(obj._defaultHttp),
    _client_ip(obj._client_ip),
    _path_to_file_str(obj._path_to_file_str),
    _client_fd(obj.getClientFD()),
    _fd(0)
{
    setTime();
}
BaseHandler::~BaseHandler() {}

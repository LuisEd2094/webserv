#include <BaseHandler.hpp>
#include <CGI.hpp>
#include <FileReader.hpp>
#include <DirectResponse.hpp>
#include <Client.hpp>

const ObjectTypes BaseHandler::valid_objs[NUM_OBJ] = {
    FILE_OBJ,
    NO_FD_OBJ,
    CGI_OBJ,
    DIR_OBJ,
 };


void    BaseHandler::setTime()
{
    std::time(&_last_time);
    // should return error if clock fails

}




const std::string dirList(const std::string& dir_str)
{
	DIR				*dir;
	struct dirent	*file;
	std::string		html("<!DOCTYPE html>\n");
	//chdir(directory.c_str()); //TODO try catch
	dir = opendir(dir_str.c_str()); //TODO try catch
	file = readdir(dir);

	html = html + "<html><head></head><body>";	
	html = html + "<h1>Directory listing for " + dir_str + "</h1><hr>";	
	html = html + "<ul>";	
	while (file)
	{
		if (!Overseer::checkIfDeleted(dir_str + (file->d_name)) && std::string(file->d_name) != "." && std::string(file->d_name) != "..")
			html = html + "<li><a href=" + dir_str + file->d_name + ">" + file->d_name + "</a></li>";
		file = readdir(dir);
	}
	html += "</ul><hr></body></html>";

	closedir(dir);
	return html;
}

BaseHandler* BaseHandler::createObject(BaseHandler& obj)
{
    /*If you are base then you are either a FileReader or a CGI, so you got here through error creation*/
    return FileReader::createNewFileReader(obj);
}


BaseHandler* BaseHandler::createObject(Client& client)
{
    //Factory creator, requires string to identify it and client that'd be assigned to that Object
    for (std::size_t i = 0; i < NUM_OBJ; ++i)
    {
        if (client.getResponseType() == valid_objs[i])
        {
            if (valid_objs[i] == FILE_OBJ)
                return FileReader::createNewFileReader(client);
            else if (valid_objs[i] == CGI_OBJ)
                return CGI::createNewCGI(client);
            else if (valid_objs[i] == DIR_OBJ)
            {
                std::string listing = dirList(client.getPathFileString());
                return DirectResponse::createNewDirect(setContentLenHTTP(std::string(HTTP_OK) + "Content-Type: text/html\r\n", listing), listing);
            }
            else if (valid_objs[i] == NO_FD_OBJ)
            {
                if (client.getErrorCode() >= MULTIPLE_REDIRECTS && client.getErrorCode() <= MULTIPLE_REDIRECTS)
                {
                    std::string body(REDIRECT_TEMPLATE);

                    std::size_t body_pos(body.find("<body>"));

                    while(!client.getURLempty())
                    {
                        std::string newUrl = client.getNextURLRedirect();
                        std::string a = "<p><a href=\"";

                        a.append(newUrl + "\">" + newUrl + "</a></p>\n");
                        
                        
                        body.insert(body_pos + std::strlen("<body>"),a );
                    }
                    return DirectResponse::createNewDirect(setContentLenHTTP(client.getDefaultHttpResponse(), body), body);

                }
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

ErrorCodes BaseHandler::getErrorCode() const
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


const ConfigElement* BaseHandler::getConfigElement()
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

BaseHandler* BaseHandler::getErrorResponse(ErrorCodes code)
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

BaseHandler::BaseHandler():
    _configElement(NULL),
    _response_type(NOT_SET),
    _error_code(INVALID_CODE),
    _defaultHttp(),
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
    _path_to_file_str(obj._path_to_file_str),
    _client_fd(obj.getClientFD()),
    _fd(0)
{
    setTime();
}
BaseHandler::~BaseHandler() {}

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
                    std::string listing = "THIS IS LISTING";
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

bool    BaseHandler::checkTimeOut()
{
    time_t current_time;
    std::time(&current_time);

    double seconds;
    seconds = std::difftime(current_time, _last_time) * 1000;
    return (TIME_OUT_POLL > 0 && seconds >= TIME_OUT_PROCESS);
}

BaseHandler::BaseHandler(){ setTime(); }
BaseHandler::~BaseHandler() {}

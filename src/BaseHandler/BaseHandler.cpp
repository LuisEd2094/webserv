#include <BaseHandler.hpp>
#include <CGI.hpp>
#include <FileReader.hpp>
#include <DirectResponse.hpp>
#include <Client.hpp>

const ObjectTypes BaseHandler::valid_objs[NUM_OBJ] = {
    FILE_OBJ,
    NO_FD_OBJ,
    CGI_OBJ,
 };


void    BaseHandler::setTime()
{
    std::time(&_last_time);
    // should return error if clock fails

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
                std::string listing = "THIS IS LISTING";
                return DirectResponse::createNewDirect(setContentLenHTTP(HTTP_OK, listing), listing);
            }
            else
                return NULL;
        }
    }
    return NULL;
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

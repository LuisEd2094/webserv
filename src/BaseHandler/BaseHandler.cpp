#include <BaseHandler.hpp>
#include <CGI.hpp>
#include <FileReader.hpp>
#include <DirectResponse.hpp>

 std::string BaseHandler::valid_objs[NUM_OBJ] = {
    std::string(FILE_OBJ),
    std::string(DIRECT_OBJ),
    std::string(CGI_OBJ),
 };


void    BaseHandler::setTime()
{
    std::time(&_last_time);
    // should return error if clock fails

}

BaseHandler* BaseHandler::createObject(const std::string& obj, Client& client)
{
    //Factory creator, requires string to identify it and client that'd be assigned to that Object
    for (std::size_t i = 0; i < NUM_OBJ; ++i)
    {
        if (obj == valid_objs[i])
        {
            if (valid_objs[i] == FILE_OBJ)
                return FileReader::createNewFileReader(client);
            else if (valid_objs[i] == CGI_OBJ)
                return CGI::createNewCGI(client);
            else
                return NULL;
        }
    }
    return NULL;
}
BaseHandler* BaseHandler::createObject(const std::string& http, const std::string& body)
{
    // This is for when you want to generate a DirectResponse Object, 
    // it needs a HTTP and Body string to create it. 
    return DirectResponse::createNewDirect(generateHTTP(http, body), body);

}

bool    BaseHandler::checkTimeOut()
{
    time_t current_time;
    std::time(&current_time);

    double seconds;
    seconds = std::difftime(current_time, _last_time) * 1000;
    return (TIME_OUT_POLL > 0 && seconds >= TIME_OUT_PROCESS);
}

BaseHandler::BaseHandler(){}
BaseHandler::~BaseHandler() {}

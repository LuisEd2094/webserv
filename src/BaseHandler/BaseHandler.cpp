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
    return DirectResponse::createNewDirect(http, body);
    // for (std::size_t i = 0; i < NUM_OBJ; ++i)
    // {
    //     if (obj == valid_objs[i])
    //     {
    //         if (valid_objs[i] == DIRECT_OBJ)
    //             return DirectResponse::createNewDirect(http, body);
    //         else
    //             return NULL;
    //     }
    // }
    // return NULL;

}

BaseHandler::BaseHandler(){}
BaseHandler::~BaseHandler() {}

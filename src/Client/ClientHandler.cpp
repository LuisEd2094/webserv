#include <ClientHandler.hpp>


void ClientHandler::setHTTPResponse(const std::string &message)
{
    _HTTP_response.append(message);
    _C_type_HTTP = _HTTP_response.c_str();
    _HTTP_response_len = std::strlen(_C_type_HTTP);
}
void ClientHandler::setBodyResponse(const std::string &message)
{
    _out_body.append(message);
    _C_type_body = _out_body.c_str();
    _body_response_len = std::strlen(_C_type_body);
    _body_bytes_sent = 0;
}

bool    ClientHandler::has_http()
{
    return !_HTTP_response.empty();
}
bool    ClientHandler::has_body()
{
    return !_out_body.empty();
}

const std::string&      ClientHandler::getHTTP()
{
    return _HTTP_response;
}
const std::string&      ClientHandler::getBody()
{
    return _out_body;
}

ClientHandler::ClientHandler()
{

}

ClientHandler::~ClientHandler()
{
    
}

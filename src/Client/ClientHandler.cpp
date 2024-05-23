#include <ClientHandler.hpp>


void ClientHandler::setHTTPResponse(const std::string &message)
{
    _HTTP_response.append(message);
    _C_type_HTTP = _HTTP_response.c_str();
    _HTTP_response_len = std::strlen(_C_type_HTTP);
    _HTTP_bytes_sent = 0;
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

size_t ClientHandler::getChunkSize()
{
    if (_HTTP_response_len > _HTTP_bytes_sent)
    {
        return ((_HTTP_response_len - _HTTP_bytes_sent) > \
            SEND_SIZE ? SEND_SIZE : _HTTP_response_len - _HTTP_bytes_sent);
    }
    else
    {
        return ((_body_response_len - _body_bytes_sent) > \
            SEND_SIZE ? SEND_SIZE : _body_response_len - _body_bytes_sent);
    }

}

const char * ClientHandler::getC_HTTP()
{
    return _C_type_HTTP + _HTTP_bytes_sent;
}

const char * ClientHandler::getC_Body()
{
    return _C_type_body;
}


bool ClientHandler::pendingSend()
{
    return (_HTTP_response_len > _HTTP_bytes_sent || _body_response_len > _body_bytes_sent);
}


const char * ClientHandler::getToSend()
{
    if (_HTTP_response_len > _HTTP_bytes_sent)
        return _C_type_HTTP + _HTTP_bytes_sent;
    else
        return _C_type_body + _body_bytes_sent;

}

void ClientHandler::updateBytesSent(std::size_t result)
{
    if (_HTTP_response_len > _HTTP_bytes_sent)
        _HTTP_bytes_sent += result;
    else
        _body_bytes_sent += result;   
}

bool    ClientHandler::isFinished()
{
    if (_HTTP_bytes_sent >= _HTTP_response_len && _out_body.empty())
        return true;
    else if (!_out_body.empty() && _body_bytes_sent >= _body_response_len)
        return true;
    else
        return false;
}

ClientHandler::ClientHandler()
{

}

ClientHandler::~ClientHandler()
{
    
}

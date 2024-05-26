#include <RequestHandler.hpp>


void RequestHandler::setHTTPResponse(const std::string &message)
{
    _HTTP_response.append(message);
    _C_type_HTTP = _HTTP_response.c_str();
    _HTTP_response_len = std::strlen(_C_type_HTTP);
    _HTTP_bytes_sent = 0;
}
void RequestHandler::setBodyResponse(const std::string &message)
{
    _out_body.append(message);
    _C_type_body = _out_body.c_str();
    _body_response_len = std::strlen(_C_type_body);
    _body_bytes_sent = 0;
}

bool    RequestHandler::has_http()
{
    return !_HTTP_response.empty();
}
bool    RequestHandler::has_body()
{
    return !_out_body.empty();
}

const std::string&      RequestHandler::getHTTP()
{
    return _HTTP_response;
}
const std::string&      RequestHandler::getBody()
{
    return _out_body;
}

size_t RequestHandler::getChunkSize()
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

const char * RequestHandler::getC_HTTP()
{
    return _C_type_HTTP + _HTTP_bytes_sent;
}

const char * RequestHandler::getC_Body()
{
    return _C_type_body;
}


bool RequestHandler::pendingSend()
{
    return (_HTTP_response_len > _HTTP_bytes_sent || _body_response_len > _body_bytes_sent);
}


const char * RequestHandler::getToSend()
{
    if (_HTTP_response_len > _HTTP_bytes_sent)
        return _C_type_HTTP + _HTTP_bytes_sent;
    else
        return _C_type_body + _body_bytes_sent;

}

void RequestHandler::updateBytesSent(std::size_t result)
{
    if (_HTTP_response_len > _HTTP_bytes_sent)
        _HTTP_bytes_sent += result;
    else
        _body_bytes_sent += result;   
}

bool    RequestHandler::isFinished()
{
    if (_HTTP_bytes_sent >= _HTTP_response_len && _out_body.empty())
        return true;
    else if (!_out_body.empty() && _body_bytes_sent >= _body_response_len)
        return true;
    else
        return false;
}

RequestHandler::RequestHandler()
{

}

RequestHandler::~RequestHandler()
{
    
}

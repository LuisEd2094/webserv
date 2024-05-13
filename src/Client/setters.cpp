#include <Client.hpp>

void   Client::setHTTPResponse(const std::string &message)
{
    _HTTP_response.append(message);
    _C_type_HTTP = _HTTP_response.c_str();
    _HTTP_response_len = std::strlen(_C_type_HTTP);
    Overseer::setListenAction(_fd, POLLIN  | POLLOUT);
}

void Client::setBodyResponse(const std::string &message)
{
    _out_body.append(message);
    _C_type_body = _out_body.c_str();
    _body_response_len = std::strlen(_C_type_body);
    _body_bytes_sent = 0;
    Overseer::setListenAction(_fd, POLLIN  | POLLOUT);

}

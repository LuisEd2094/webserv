#include <Client.hpp>

void   Client::setMessage(const std::string &message)
{
    _msg_pending.append(message);
    _has_msg_pending = true;
    _msg_to_send = _msg_pending.c_str();
    _msg_pending_len = std::strlen(_msg_to_send);
}
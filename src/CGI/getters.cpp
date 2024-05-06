#include <CGI.hpp>


int CGI::getSocket()
{
    return _pipe[0];
}

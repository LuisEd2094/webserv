#include <CGI.hpp>


int CGI::getFD() const
{
    return _pipe[0];
}

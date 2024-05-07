#include <CGI.hpp>


const int CGI::getFD() const
{
    return _pipe[0];
}

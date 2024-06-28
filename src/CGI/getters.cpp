#include <CGI.hpp>


int CGI::getFD() const
{
    return _out_pipe[0];
}

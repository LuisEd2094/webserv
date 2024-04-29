#include <Aux.hpp>


void exitError(const std::string& error)
{
    std::cerr << error << std::endl;
    std::exit(EXIT_FAILURE);

}

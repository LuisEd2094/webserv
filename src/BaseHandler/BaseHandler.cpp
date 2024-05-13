#include <BaseHandler.hpp>

void    BaseHandler::setTime()
{
    std::time(&_last_time);
    // should return error if clock fails

}

BaseHandler::BaseHandler(){}
BaseHandler::~BaseHandler() {}

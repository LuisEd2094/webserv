#include <DirectResponse.hpp>
#include <Client.hpp>

DirectResponse* DirectResponse::createNewDirect(const std::string& http, const std::string & body)
{
    DirectResponse* newDirect = new DirectResponse(http, body);
    return newDirect;
}

DirectResponse::DirectResponse(const std::string& http, const std::string & body) :
    _http_response(http),
    _body_response(body)
{

}

DirectResponse::~DirectResponse()
{

}

std::string& DirectResponse::get_http()
{
    return _http_response;
}

std::string& DirectResponse::get_body()
{
    return _body_response;
}

bool DirectResponse::has_http()
{
    return !_http_response.empty();

}
bool DirectResponse::has_body()
{
    return !_body_response.empty();

}

int DirectResponse::Action(int event){(void)event; return 0;}
bool DirectResponse::checkTimeOut(){return false;}
const int DirectResponse::getFD() const {return 0;}
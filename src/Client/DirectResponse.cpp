#include <DirectResponse.hpp>
#include <sstream>


DirectResponse* DirectResponse::createNewDirect(const std::string& http, const std::string & body)
{
    return new DirectResponse(http, body);
}

DirectResponse::DirectResponse(const std::string& http, const std::string & body) :
    _http_response(http),
    _body_response(body)
{
    if (!_body_response.empty())
    {
        _http_response.append(CONTENTLENGTH + toString(_body_response.length()) + CRNL);
        std::cout << toString(_body_response.length()) << std::endl;
    }
    _http_response.append("\r\n");
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

#ifndef DIRECTRESPONSE_HPP
# define DIRECTRESPONSE_HPP

# include <BaseHandler.hpp>
# include <iostream>

class Client;

class DirectResponse : public BaseHandler
{
    public:
        static DirectResponse * createNewDirect(const std::string&, const std::string &);
        DirectResponse(const std::string&, const std::string &);
        ~DirectResponse();

        std::string& get_http();
        std::string& get_body();

        bool has_http();
        bool has_body();

    private:
        int Action(int event);
        bool checkObjTimeOut();
        int getFD() const;

        std::string _http_response;
        std::string _body_response;
};

#endif

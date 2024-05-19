#ifndef DIRECTRESPONSE_HPP
# define DIRECTRESPONSE_HPP

# include <BaseHandler.hpp>
# include <iostream>

class Client;

class DirectResponse : public BaseHandler
{
    public:

        DirectResponse(Client *, const std::string&, const std::string &);
        ~DirectResponse();

    private:
        int Action(int event);
        bool checkTimeOut();
        const int getFD() const;
};

#endif
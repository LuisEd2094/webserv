#ifndef CLIENTHANDLER_HPP
# define CLIENTHANDLER_HPP
# include <iostream>
# include  <cstring>

class ClientHandler
{
    public:
        ClientHandler();
        ~ClientHandler();
        void                setHTTPResponse(const std::string &message);
        void                setBodyResponse(const std::string &message);

        bool                has_http();
        bool                has_body();

        const std::string&      getHTTP();
        const std::string&      getBody();

    private:
        std::string              _HTTP_response;
        std::size_t             _HTTP_response_len;
        std::size_t             _HTTP_bytes_sent;
        const char *            _C_type_HTTP;

        bool                    _requested_response;
        std::string             _out_body;
        std::size_t             _body_response_len;
        std::size_t             _body_bytes_sent;
        const char *            _C_type_body;

};

#endif
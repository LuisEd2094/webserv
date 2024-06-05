#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP
# include <iostream>
# include <cstring>
# include <ProgramConfigs.hpp>
# include <queue>

class Client;

class RequestHandler
{
    public:
        RequestHandler(const Client&);
        ~RequestHandler();
        void                setHTTPResponse(const std::string &message);
        void                setBodyResponse(const std::string &message);
        void                setHeaderAddons(const std::queue<std::string>& addons);

        bool                has_http();
        bool                has_body();
        bool                pendingSend();
        bool                isFinished();

        void                    updateBytesSent(std::size_t);

        const char *            getToSend();
        std::size_t             getChunkSize();
        const std::string&      getHTTP();
        const std::string&      getBody();
        const char *            getC_HTTP();
        const char *            getC_Body();

    private:
        std::queue<std::string> _header_addons;
        std::string             _HTTP_response;
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

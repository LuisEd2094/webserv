#ifndef FILEREADER_HPP
# define FILEREADER_HPP

# include <iostream>
# include <BaseHandler.hpp>
# include <map>

class Client;

class FileReader : public BaseHandler
{
    public:
        FileReader(Client& client);
        ~FileReader();
        int                 Action(int event);

        bool                checkObjTimeOut();

        std::string         setContentType(std::string);
        int           getFD() const;

        static FileReader* createNewFileReader(Client& client);
        static const std::string& getContentType(const std::string&);
        static void initTypeMaps();
        static const std::string& getMimeType(const std::string&);
        class FileReaderException;

    private:
        static std::map<std::string, std::string> types;
        int             _client_fd;
        std::string     _buffer;
        std::string     _file_type;
        std::string     _defaultHttp;

};


#endif

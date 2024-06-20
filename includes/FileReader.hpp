#ifndef FILEREADER_HPP
# define FILEREADER_HPP

# include <iostream>
# include <BaseHandler.hpp>
# include <map>

class Client;

class FileReader : public BaseHandler
{
    public:
        FileReader(BaseHandler& client);
        ~FileReader();
        int                 Action(int event);

        bool                checkObjTimeOut();

        std::string         setContentType(std::string);
        int           getFD() const;

        static BaseHandler*                     createNewFileReader(BaseHandler& client);
        static const std::string& getContentType(const std::string&);
        static void initTypeMaps();
        static const std::string& getMimeType(const std::string&);
        class FileReaderException;

    private:
        static std::map<std::string, std::string> types;
        std::string     _buffer;
        std::string     _file_type;
        std::string     _defaultHttp;

};


#endif

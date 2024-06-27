#ifndef FileHandler_HPP
# define FileHandler_HPP

# include <iostream>
# include <BaseHandler.hpp>
# include <map>
# include <Path.hpp>
# include <typeinfo>
# include <algorithm>
class Client;

class FileHandler : public BaseHandler
{
    public:
        FileHandler(BaseHandler&);
        ~FileHandler();
        int                 Action(int event);

        bool                checkObjTimeOut();

        std::string         setContentType(std::string);
        int                 getFD() const;
        bool                getIsWritting() const; 

        static BaseHandler*                     createNewFileHandler(BaseHandler& client);
        static const std::string& getContentType(const std::string&);
        static void initTypeMaps();
        static const std::string& getMimeType(const std::string&);
        class FileHandlerException;

    private:
        static std::map<std::string, std::string> types;
        std::string     _buffer;
        std::string     _file_type;
        std::string     _refered;
        std::string     _body;
        Path            _file_name;
        std::string     _boundary;
        std::size_t     _bytes_written;
        std::size_t     _file_len;
        bool            _is_writting;
        bool            _file_exists;


        void            openFile(const std::string&, int, mode_t);
        int             readFromFile();
        int             writeToFIle();

};



#endif

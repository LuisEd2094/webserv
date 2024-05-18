#ifndef FILEREADER_HPP
# define FILEREADER_HPP

# include <iostream>
# include <BaseHandler.hpp>

class Client;

class FileReader : public BaseHandler
{
    public:
        FileReader(Client& client);
        ~FileReader();
        int                 Action(int event);

        bool                checkTimeOut();

        static FileReader * createNewFileReader(Client& client);
        const int           getFD() const;
        class FileReaderException;

    private:
        int             _client_fd;
        std::string     _buffer;
};


#endif

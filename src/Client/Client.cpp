#include <Server.hpp>
#include <DirectResponse.hpp>
#include <Client.hpp>

//Exception
class Client::clientException : public std::exception
{
    private:
        std::string _err;
    public:
        clientException(const std::string& err) : _err(err) {};
        ~clientException() throw() {};
        const char *what() const throw()
        {
            return (_err.c_str());
        }
};

//public:
Client::Client(Server *server) : BaseHandler()
{
    if (!server)
        return;
    _addrlen = sizeof(_remoteaddr);
    _fd = accept(server->getFD(), (struct sockaddr *)&_remoteaddr, &_addrlen);
    if (_fd == -1) 
    {
        throw Client::clientException("accept" + static_cast<std::string>(strerror(errno)));
    }
    struct sockaddr_in *sin = (struct sockaddr_in *)&_remoteaddr;
    unsigned long ip = ntohl(sin->sin_addr.s_addr);
    unsigned char bytes[4];

    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;

    std::ostringstream ip_stream;
    ip_stream << static_cast<int>(bytes[3]) << "."
              << static_cast<int>(bytes[2]) << "."
              << static_cast<int>(bytes[1]) << "."
              << static_cast<int>(bytes[0]);

    _client_ip = ip_stream.str();
    // << "Connected IP: " << ip_str << std::endl;

    _server = server;
    _response_type = NOT_SET;
    _action = WAIT;
    _pending_read = false;
    _keep_alive = KEEP_ALIVE;
    _size_to_append = 0;
    _can_read = true;
    _error = false;
    _is_chunked = false;
    _chunk_size = 0;
    _virtualServer = NULL;
    _path_to_file = Path("");
    _defaultHttp = "";
    _content_length = 0;
    _was_zero = false;
}

Client::~Client() 
{
    while (!_response_objects_queue.empty())
    {
        delete _response_objects_queue.front();
        _response_objects_queue.pop();
    }
    close(_fd);
}


void  Client::setTime()
{
    if (!_was_zero)
        BaseHandler::setTime();
}

int Client::Action(int event)
{
    if (event & POLLIN)
    {
        if (_can_read)
        {
            readFromFD();
            if (_result < 0)
                return (-1);
        }

    }
    if (event & POLLOUT)
    {
        return sendResponse();
    }
    if (event & POLLHUP)
    {
        return 0;
    }
    return _result;
}

bool    Client::verifyIfEnoughForLocation()
{
    std::string method, url, host;

    method = _parser_http.getMethod();
    url = _parser_http.getRequested();
    host = _parser_http.getMapValue("Host");

    return (!method.empty() && !url.empty() && host != "not found");
}

void    Client::addClosingError(ResponseCodes error)
{
    /*This functions adds the error object to the queue and sets the client to a state
        where it'd close the connection after finishing ending the queue */
    if (!_configElement && verifyIfEnoughForLocation())
    {
        _server->prepareClient4ResponseGeneration(*this);
    }
    addObject(getErrorResponse(error));
    _action = GET;
    _error = true;
    _can_read = false;
    _pending_read = false; 
    _keep_alive = false;
    Overseer::setListenAction(_fd, JUST_OUT);
}

void    Client::addClosingErrorObject(ResponseCodes error)
{
    addClosingError(error);
}

void Client::handlerRecv()
{
    if (_action == POST)
    {
        std::size_t size_to_read;
        if (_is_chunked)
        {
            /*
                we read + 2 because _chunk_size doesn't include the new lines
                new lines can be CRNL
            */
            if (_chunk_size == 0)
            {
                /*
                    If chunk_size == 0 then we haven't read the first line of the chunk
                    I don't Want to read 80k chars here, just in case there are a bunch of 
                    HTTP requests after this body, that way this client doesn't take forever
                */
                size_to_read = RECV_SIZE;
            }
            else
            {
                size_to_read = std::min(_chunk_size - _chunk.length() + 2, (std::size_t)BUFFER_SIZE);
            }
        }
        else
        {
            size_to_read = std::min(_content_length - _in_body.length(), (std::size_t)BUFFER_SIZE);
        }
        _result = recv(_fd, _in_message, size_to_read, 0);
    }
    else
    {
        _result = recv(_fd, _in_message, RECV_SIZE, 0);
    }
}

void Client::readFromFD()
{
    handlerRecv();
    if (_result > 0)
    {
        // post > in_body, parseeer URl, METHod, Client dame el body
        if (!_parser_http.getEndRead()) // and nott MAX HEADER SIZE?
        {
            _in_container.append((const char *)_in_message, _result);
            // << _in_container << std::endl;
            while (_keep_alive && _parser_http.getPos() != _in_container.length())
            {
                if (!_pending_read)
                    _pending_read = true;
                if (_action == WAIT)
                {
                    int parser_method = _parser_http.checkMethod(_in_container);
                    if (!parser_method) //check method returns 0 on success
                    {
                        updateMethodAction();
                    }
                    else
                    {
                        /*
                            if parseer method wrong (not GET, POST or DELETE) then its a 501
                            Error fortmat == Bad Request
                            ERROR_HEADER == BAD_REQUEST
                            if ERROR_VERSION == HTTP Version Not Supported if 1.1+
                        */
                        if (parser_method != WARNING)
                        {
                            addClosingError(ResponseCodes(parser_method));
                        }
                        break;
                    }
                }
                if (_action != WAIT) 
                {
                    int parsingMessage = _parser_http.parsingHeader(_in_container);

                    if (parsingMessage == WARNING) // parsingHeader return 1 on failure
                        break;
                    else if (parsingMessage) // parsingHeader returns something different to warning when error
                    {
                        addClosingError(ResponseCodes(parsingMessage));
                        break ;
                    } 
                    parseForHttp();
                }
            }
        }
        else if (_action == POST)
        {
            /*if it's chunked I want to handle everything directly on _in_container
                as I might have a few chunks ready on the first read, and after I finish with the chunk
                I might have a new request after the last chunk, so at any giving point I might have to
                work directly on _in container
            */
            if (_is_chunked)
            {
                _in_container.append((const char *)_in_message, _result);
                processChunk();
            }
            else
            {
                _size_to_append = std::min(_result, static_cast<int>(_content_length - _in_body.length()));
                _in_body.append((const char *)_in_message, _size_to_append);
                if (_in_body.length() == _content_length ) // we done with body || if chunked, if read size is 0
                {  
                    resetClient(true);
                }
            }


        }
    }
    else
    {
         if (_result == 0)
         {
            _was_zero = true;
            _result = !checkTimeOut();
            /*Returns 0 when NOT time out returns 1 when timeout*/
         }
    }
}


void Client::checkFirstQueue(RequestHandler *obj)
{
    if (_response_objects_queue.front() == obj)
    {
        Overseer::setListenAction(_fd, IN_AND_OUT);
    }
}

void Client::handleDirectObj(DirectResponse* NO_FD_OBJ, RequestHandler *new_handler)
{
    if (NO_FD_OBJ->has_http())
    {
        new_handler->setHTTPResponse(NO_FD_OBJ->get_http());
    }
    if (NO_FD_OBJ->has_body())
    {
        new_handler->setBodyResponse(NO_FD_OBJ->get_body());
    }
    checkFirstQueue(new_handler);
    delete NO_FD_OBJ; // I delete this since it wont be going to the FD POLL
}

bool Client::checkPostHeaderInfo()
{
    /*Get post data from Header so we can check it on server to validate it.*/
    const std::string& content_length = _parser_http.getMapValue("Content-Length");
    const std::string& transfer_encoding = _parser_http.getMapValue("Transfer-Encoding");
    if ((content_length != "not found" && transfer_encoding != "not found") || \
        (content_length == "not found" && transfer_encoding == "not found" ))
    {
        /*Close connection since we can't have both content len and transfer enconding 
        and we wont accept a connection without at least one of them */
        addClosingError(LENGTH_REQUIRED); 
        return false;
    }
    if (content_length != "not found")
    {
        _content_length = std::atoi(content_length.c_str());
    }
    else
    {
        _is_chunked = true;
    }
    return true;
}

void Client::processChunk()
{
    /*it'd allway have the chunk in _in_container
        we need the chunck size (we can find the first r or n) in hexa
        read the chunk size from in_container(it does not include the new line size)
    */
   if (!_chunk_size)
   {
    /*If not chunk_size then we haven't process this chunk yet.
        get chunk size and remove the first line from in_container.
    */
        std::size_t chunk_end_line = _in_container.find_first_of("\n\r");
        if (chunk_end_line != std::string::npos)
        {
            _chunk_size = hexStringToSizeT(_in_container.substr(0, chunk_end_line + 1));
            if (_chunk_size == 0)
            {
                resetClient(true);
                return;
            }
            _in_container.erase(0, _in_container.find_first_of('\n') + 1);
        }
   }
   if (_chunk_size) /*this is not an else since I want it to run on once it gets chunk size from the last line*/
   {
        _size_to_append = std::min(_in_container.length(), _chunk_size - _chunk.length());
        _chunk.append(_in_container, 0, _size_to_append);
        if (_chunk.length() == _chunk_size)
        {
            /*if we get the full chunk size, we have to remove the last end, since 
                the endl is not included in the size of the chunk
            */
            _in_body += _chunk;
            _chunk.clear();
            _chunk_size = 0;
            
            /*Removes what ever we got from this chunked read + 2 for the CRNL*/
            _in_container.erase(0, _size_to_append + 2);
            _size_to_append = 0; 
        }
        else
        {
            _in_container.clear();

        }
   }



}

void Client::parseForHttp()
{
    if (_parser_http.getEndRead())
    {

        if (!_server->validateAction(*this))
        {            
            _in_container.erase(0, _parser_http.getPos() + _parser_http.getEndSize());
            resetClient(false);
            return ;
        }
        if (_action == POST)
        {
            if (!checkPostHeaderInfo())
                return;
        }
        if (_action == POST)
        {
            if (!_is_chunked)
            {
                if (_content_length > std::size_t(MAX_BODY_SIZE))
                {
                    addClosingErrorObject(PAYLOAD);
                    return;
                }
            }
        }

        if (_parser_http.getMapValue("Expect") == "100-continue")
        {
            addObject(BaseHandler::createObject(Response::getDefault(CONTINUE)));
        }
        _in_container.erase(0, _parser_http.getPos() + _parser_http.getEndSize());
        if (_parser_http.getMapValue("Connection") == "keep-alive")
        {
            if (_keep_alive)
                _keep_alive = true;
        }
        else if (_parser_http.getMapValue("Connection") == "close")
        {
            _keep_alive = false;
        }
        if(_action == POST)
        {
            /* this should check what type of post they are doing, could be a contentlen or a chunked sent */

            if (_is_chunked)
            {
                processChunk();
            }
            else
            {
                _in_body.append(_in_container, 0, std::min(_content_length, _in_container.length()));
                if (_in_body.size() == _content_length ) // we done with body || if chunked, if read size is 0
                {
                    resetClient(true);
                }
                else
                {
                    /*  
                        Clear _in_container if _in_body doesn't == content_lengthght, since that means we are missing data
                        and we will be getting it next read.
                        If we get the full body in a single read, reset will remove body size from in container
                    */
                    _in_container.clear();
                }
            }
            
        }
        else if (_action == DELETE)
        {
            Overseer::addToDeleted(getPathFileString());
            _response_type = NO_FD_OBJ;
            _error_code = OK;
            resetClient(false);
        }
        else 
        {
            resetClient(false);
        }
    }
}

void Client::updateMethodAction()
{
    const std::string & method = _parser_http.getMethod();
      
    if (method == "GET")
        _action = GET;
    else if (method == "POST")
        _action = POST;
    else if (method == "DELETE")
        _action = DELETE;
}


void Client::removeFirstObj()
{
    delete _response_objects_queue.front();
    _response_objects_queue.pop();
    if (!_response_objects_queue.empty())
    {
        if(!(_response_objects_queue.front()->has_http()))
        {
            Overseer::setListenAction(_fd, JUST_IN);
        }
    }
    else
    {
        if (_error)
        {
            Overseer::setListenAction(_fd, 0);
        }
        else
        {
            Overseer::setListenAction(_fd, JUST_IN);
        }
    }
}
 
int Client::sendResponse()
{
    RequestHandler * request = _response_objects_queue.front();
    if (request && request->pendingSend())
    {
        if ((_result = send(_fd, request->getToSend(), request->getChunkSize(), 0) ) == -1)
            return (-1);
        request->updateBytesSent(_result);
        if (request->isFinished())
        {
            removeFirstObj();
            return (!_response_objects_queue.empty() || _keep_alive);
        }
        else
        {
            return (1);
        }
    }
    return (1);
}




void Client::makeChildrenToRespond()
{
    BaseHandler *response;
    try
    {
        /*prepareClient4ResponseGeneration doesn't set if no file is found*/
        if (IS_ERROR_CODE(_error_code))
        {
            response = getErrorResponse(_error_code);
        }
        else if (_response_type == NOT_SET)
        {

            response = getErrorResponse(NOT_FOUND);
        }
        else if (_action != DELETE && Overseer::checkIfDeleted(getPathFileString()))
        {
            response = getErrorResponse(GONE);
        }
        else
            response = BaseHandler::createObject(*this);
    }
    catch (const Path::InvalidPathException & err)
    {
        std::cerr << err.what() << std::endl;
        _error_code = BAD_REQUEST;
        response = getErrorResponse(_error_code);
    }
    catch(const std::exception& e)
    {
        /*Leave this one as the the default answer, if file reader tries to open a file to give an answer
            (INTERNAL_SERVER_ERROR) and it fails, then it should just go to the default
            FD_READER throws exception in case OPEN fails
        */
       _error_code = INTERNAL_SERVER_ERROR;
        response = getErrorResponse(_error_code);
    }
    std::queue<std::string> queue;

    addHeader(queue);
    if (_keep_alive)   
        addHeader(std::string("Connection: keep-alive") + CRNL); 
    else
        addHeader(std::string("Connection: close") + CRNL); 

    addObject(response);
}

void Client::resetClient(bool has_body)
{
    if (has_body)
    {
        if (_response_type != CGI_OBJ)
        {
            _response_type = FILE_OBJ;
            setDefaultHttpResponse(OK);
        }

        /* gives seg fault*/
        /*If _size_to_append == 0 it means we got the full body in the first read with HTTP*/
        if (_size_to_append == 0)
        {
            /* If it's chunked we will remove all chuncks inside _in_container before getting here
                so if if it gets here (because we got all chuncks in the first read of the request)
                _in_container will NOT have any chuncked data in it. 
                So we only remove _in_body.length from _in_container when we get here in the first read 
                and we are NOT chuncked
            */
            if (!_is_chunked)
            {
                _in_container.erase(0, _in_body.length());
            }

        }
        else if (static_cast<int>(_size_to_append) != _result)
        {
            /*If size to append is not == 0 it means we went through at least a second read
                if that append size doesnt equal _result, it means we appended less than the characters we read
                so we can assume that there might be a new http request after the body of the first one.
            */
            _in_container = ((const char *)&(_in_message[_size_to_append + 1]));         
        }
    }
    makeChildrenToRespond();
    /* HTTP redirections por ejemplo, Location ...*/
    while (!_http_addons.empty())
        _http_addons.pop();
    _in_body.clear();
    _pending_read = false;
    _action = WAIT;
    _content_length = 0;
    _response_type = NOT_SET;
    _size_to_append = 0;
    _is_chunked = false;
    _parser_http.resetParsing();
    _chunk_size = 0;
    _virtualServer = NULL;
    _configElement = NULL;
    _path_to_file = Path("");
    _path_to_file_str = "";
    _defaultHttp = "";
    _error_code = OK;
    _was_zero = false;
}

bool Client::checkObjTimeOut()
{
    /* Should have a different time out setting for open connections that are not being used*/
/*     char *error_buffer[1];*/ 
    // return false;
    if (!_response_objects_queue.empty())
        return false;
    if((_pending_read || _error) && checkTimeOut())
    {
        //When time out, client will stop listening for incoming messages, will send everything it has
        // in queue and thenn close connection.
        if (_pending_read)
        {
            addClosingError(REQUEST_TIMEOUT);
            Overseer::setListenAction(_fd, JUST_OUT);
        }
        else
        {
            /*This returns true because if _error then we are in the middle of sending an error and we dont want to close connection*/
            return true;
        }
        _keep_alive = false;
        return false;
    }
    return checkTimeOut();
}

//private:
Client::Client() {}






#ifndef URI_HPP
#define URI_HPP

#include <string>
#include <algorithm>
#include <iostream>

struct Uri
{
	public:
		std::string QueryString;
		std::string Path;
		std::string Protocol;
		std::string Host;
		std::string Port;
	static Uri Parse(const std::string &uri)
	{
	    Uri result;

	    typedef std::string::const_iterator iterator_t;
	    if (uri.length() == 0)
	        return result;

	    iterator_t uriEnd = uri.end();
	    iterator_t queryStart = std::find(uri.begin(), uriEnd, '?');
	    iterator_t protocolStart = uri.begin();
	    iterator_t protocolEnd = std::find(protocolStart, uriEnd, ':');

	    if (protocolEnd != uriEnd)
	    {
	        std::string prot = &*(protocolEnd);
	        if ((prot.length() > 3) && (prot.substr(0, 3) == "://"))
	        {
	            result.Protocol = std::string(protocolStart, protocolEnd);
	            protocolEnd += 3;
	        }
	        else
	            protocolEnd = uri.begin();
	    }
	    else
	        protocolEnd = uri.begin();

	    iterator_t hostStart = protocolEnd;
	    iterator_t pathStart = std::find(hostStart, uriEnd, '/');
	    iterator_t hostEnd = std::find(protocolEnd,
	        (pathStart != uriEnd) ? pathStart : queryStart,
	        ':');

	    result.Host = std::string(hostStart, hostEnd);
	    if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == ':'))
	    {
	        hostEnd++;
	        iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
	        result.Port = std::string(hostEnd, portEnd);
	    }
	    if (pathStart != uriEnd)
	        result.Path = std::string(pathStart, queryStart);
	    if (queryStart != uriEnd)
	        result.QueryString = std::string(queryStart, uri.end());
	    return (result);
		}
	};

// std::ostream &operator<<(std::ostream &os, const Uri &obj)
// {
// 	os  << "QueryString: " << obj.QueryString << std::endl;
// 	os  <<  "Path: " << obj.Path << std::endl;
// 	os  <<  "Protocol: " << obj.Protocol << std::endl;
// 	os  <<  "Host: " << obj.Host << std::endl;
// 	os  <<  "Port: " << obj.Port << std::endl;
// 	return (os);
// }
#endif

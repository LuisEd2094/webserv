#include "ConfigElement.hpp"
#include "ConfigVirtualServer.hpp"

Overseer ConfigElement::parse(ParsingGlobal parsedData)
{
	t_confi confi;
	Server *server;
	std::map<std::string, Server>::iterator serverIterator;
	std::map<std::string, Server> servers;

    std::memset(&(confi.hints), 0, sizeof(confi.hints));
    confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
    confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    confi.hints.ai_flags = AI_PASSIVE;
    confi.backlog = 200;
	 
	for (std::list<ParsingServer>::iterator i = parsedData.servers.begin(); i != parsedData.servers.end(); i++)
	{
		confi.ip = (*i)["host"];		//TODO checkear un poco
		confi.port = (*i)["port"];		//TODO checkear un poco 
		serverIterator = servers.find(confi.ip + confi.port);
		if (serverIterator == servers.end())
			server = Overseer::saveServer(&confi);
		else
			server = &(serverIterator->second);
		server->virtualServers.push_back(ConfigVirtualServer(*i));
		
		

	}
}


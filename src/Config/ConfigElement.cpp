#include "ConfigElement.hpp"
#include "ConfigVirtualServer.hpp"
#include "Overseer.hpp"
# include <map> //map

void ConfigElement::configure(ParsingGlobal parsedData)
{
	t_confi confi;
	Server *server;
	std::map<std::string, Server*>::iterator serverIterator;
	std::map<std::string, Server*> servers;

    std::memset(&(confi.hints), 0, sizeof(confi.hints));
    confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
    confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    confi.hints.ai_flags = AI_PASSIVE;
    confi.backlog = 200;
	std::cout << "Num of servers: " <<  parsedData.servers.size() << std::endl;
 
	for (std::list<ParsingServer>::iterator i = parsedData.servers.begin(); i != parsedData.servers.end(); i++)
	{
	    std::memset(&(confi.hints), 0, sizeof(confi.hints));
		confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
		confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
		confi.hints.ai_flags = AI_PASSIVE;
		confi.backlog = 200;
		confi.ip = (*i)["host"];		//TODO checkear un poco
		// std::cout << "host " << (*i)["host"] << " " << confi.ip << std::endl;
		confi.port = (*i)["port"];		//TODO checkear un poco
		std::string phisicServerId = confi.ip + confi.port;
		serverIterator = servers.find(phisicServerId);
		if (serverIterator == servers.end())
		{
			server = Overseer::saveServer(&confi);
			std::cout << "*******-*-*-*--*************" << std::endl;
			servers.insert(std::pair<std::string,Server*>(phisicServerId, server));
			std::cout << "*******-*-*-*--*************" << std::endl;
		}
		else
			server = serverIterator->second;
		server->virtualServers.push_back(ConfigVirtualServer(*i));
	}
}


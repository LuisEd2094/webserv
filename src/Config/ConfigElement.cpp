#include "ConfigElement.hpp"
#include "ConfigVirtualServer.hpp"
#include "Overseer.hpp"
# include <map>

void fileElement4Response();

std::string ConfigElement::genSpace(int ammount)
{
	ammount *= 4;
	std::string	result;
	while (ammount--)
		result += " ";
	return result;
}
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
    confi.backlog = MAX_FDS;
	// << BLUE << "Num of servers: " <<  END << parsedData.servers.size() << std::endl;
	if (parsedData.servers.empty())
		throw ParamError("Error: File empty");
	for (std::list<ParsingServer>::iterator i = parsedData.servers.begin(); i != parsedData.servers.end(); i++)
	{
	    std::memset(&(confi.hints), 0, sizeof(confi.hints));
		confi.hints.ai_family = AF_UNSPEC; //takes ipv4 and ipv6
		confi.hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
		confi.hints.ai_flags = AI_PASSIVE;
		confi.backlog = MAX_FDS;
		confi.ip = (*i)["host"];		//TODO checkear un poco
		confi.port = (*i)["port"];		//TODO checkear un poco
		std::string phisicServerId = confi.ip + confi.port;
		serverIterator = servers.find(phisicServerId);
		if (serverIterator == servers.end())
		{
			server = Overseer::saveServer(&confi);
			servers.insert(std::pair<std::string,Server*>(phisicServerId, server));
		}
		else
			server = serverIterator->second;
		server->virtualServers.push_back(ConfigVirtualServer(*i));
	}
}


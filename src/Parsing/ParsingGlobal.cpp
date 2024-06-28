#include "../../includes/ParsingGlobal.hpp"
#include "../../includes/Parsing.hpp"
#include <fstream>

ParsingGlobal	ParsingGlobal::parseFromFile(std::string fileName)
{
	std::fstream	file(fileName.c_str());
	std::string		fileStr;										  
	std::string		tmp;
	std::string::iterator begin;
	std::string::iterator end;
	std::string::iterator aux;

	if (!file.is_open())
	{
		throw std::exception();
	}
	while (true)
	{
		file >> tmp;
		if (file.eof())
			break;
		fileStr += tmp + " ";
	}
	begin = fileStr.begin();
	end = fileStr.end();
	aux = fileStr.begin();
	ParsingGlobal 	parser(begin, end, aux);

	parser.parse();
	return (parser);
}

ParsingGlobal::ParsingGlobal(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ParsingElement(begin, eof, statementEnd)
{
}

void	ParsingGlobal::createNestedElement(std::string param0, std::string param1)
{
	(void)param1;
	if (param0 == "server")
	{
		ParsingServer element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		this->servers.push_back(element);
	}
	else
	{
		std::cout << "Invalid configElement: " << param0 << std::endl;
		throw std::exception();
	}
}

void	ParsingGlobal::recursivePrint(void)
{
	std::cout << TUR << "Call ParsingGlobal " << servers.size() << END << std::endl;
	std::list<ParsingServer>::iterator it;
	for (it = this->servers.begin(); it != this->servers.end(); ++it)
		it->recursivePrint();
}

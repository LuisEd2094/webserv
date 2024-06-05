#include "../../includes/ParsingGlobal.hpp"
#include "../../includes/Parsing.hpp"
#include <fstream>

ParsingGlobal	ParsingGlobal::parseFromFile(std::string fileName)
{
	std::fstream	file(fileName.c_str()); //TODO"config_file.conf"
	std::string		fileStr;										  
	std::string		tmp;
	std::string::iterator begin;
	std::string::iterator end;
	std::string::iterator aux;

	std::cout << fileName <<std::endl;
	if (!file.is_open())
	{
		std::cout << "file not found" << std::endl;
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
	try
	{
		parser.parse();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (parser);
}

ParsingGlobal::ParsingGlobal(std::string::iterator &begin,
				std::string::iterator &eof,
				std::string::iterator &statementEnd) : ParsingElement(begin, eof, statementEnd)
{
	std::cout << "::::: ParsingGlobal constructor called :::::" << ">" << std::endl;
}

void	ParsingGlobal::createNestedElement(std::string param0, std::string param1)
{

	std::cout << "global nested: " << param0 << "  " << param1 << std::endl;
	if (param0 == "server")
	{
		ParsingServer element(this->statementBegin, this->eof, this->statementEnd);	
		element.parse();
		//element.insert(std::make_pair("__elemType__", param0));
		//element.insert(std::make_pair("__elemArgument__", param1));
		this->servers.push_back(element);
	}
	else
	{
		std::cout << "user is stupid" << std::endl;
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

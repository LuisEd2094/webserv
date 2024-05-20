/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:56:11 by dacortes          #+#    #+#             */
/*   Updated: 2024/05/20 16:00:39 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>

//#include "../../includes/ParsingElement.hpp"
//#include "../../includes/ParsingServer.hpp"
//#include "../../includes/ParsingLocation.hpp"
#include "../../includes/ParsingGlobal.hpp"
#include "../../includes/Parsing.hpp"
//#include "../../includes/ParsingCgi.hpp"
#include <iostream>

int fileConfig(void)
{
	std::fstream	file("config_file.conf"); //TODO
	std::string		fileStr;										  
	std::string		tmp;
	std::string::iterator begin;
	std::string::iterator end;
	std::string::iterator aux;

	if (!file.is_open())
	{
		std::cout << "no toy" << std::endl;
		return (1);
	}
	while (true)
	{
		file >> tmp;
		if (file.eof())
			break;
		fileStr += tmp + " ";
	}
	std::cout << "**" << fileStr << "**" <<std::endl;
	begin = fileStr.begin();
	end = fileStr.end();
	aux = fileStr.begin();
	// std::cout << "Should be equal : " << (begin == aux) << std::endl;
	// std::cout << "fkkk" << std::string(begin, end) << std::endl;
	// std::cout << "Should be equal : " << (begin == aux) << std::endl;

	ParsingGlobal 	parser(begin, end, aux);
	try
	{
		parser.parse();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	std::cout << "-------------------------------------" << std::endl;
	parser.recursivePrint();
	std::cout << "-------------------------------------" << std::endl;
	return 0;
	
}

int	testParseHtpp(void)	
{
	Parsing test;
	std::cout << TUR << "test: " << ((int)test.getTypeLine("hola\n")[0]) << END << std::endl;
	return (0);
}

int  main(void)
{
	fileConfig();
	// testParseHtpp();
	return (0);
}

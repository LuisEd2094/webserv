/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:56:11 by dacortes          #+#    #+#             */
/*   Updated: 2024/05/08 17:12:30 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>

//#include "../../includes/ConfigParse.hpp"
//#include "../../includes/ConfigServer.hpp"
//#include "../../includes/ConfigLocation.hpp"
#include "../../includes/ConfigGlobal.hpp"
//#include "../../includes/ConfigCgi.hpp"
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

	ConfigGlobal 	parser(begin, end, aux);
	try
	{
		parser.parse();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
	
}
	
int  main(void)
{
	fileConfig();
	return (0);
}

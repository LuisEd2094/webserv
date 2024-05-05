/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:56:11 by dacortes          #+#    #+#             */
/*   Updated: 2024/05/05 17:03:36 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include "../../includes/ConfigParse.hpp"
#include <iostream>

int fileConfig(void)
{
	std::fstream	file("config_file.conf"); //TODO
	std::string		fileStr;										  
	std::string		tmp;

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
	ConfigParse 	parser(fileStr);
	parser.parse();

	return 0;
	
}
	
int  main(void)
{
	fileConfig();
	return (0);
}

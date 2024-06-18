/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:56:11 by dacortes          #+#    #+#             */
/*   Updated: 2024/06/14 16:28:49 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include "../../includes/ParsingGlobal.hpp"
#include <iostream>
#include <string>







#include <Path.hpp>

// std::list<std::string> split(const std::string& str, char delimiter)
// {
//     std::list<std::string> list;
//     std::istringstream input(str);
//     std::string word;
    
//     while (std::getline(input, word, delimiter))
//         list.push_back(word);
//     return (list);
// }

// int fileConfig(void)
// {
// 	std::fstream	file("config_file.conf"); //TODO
// 	std::string		fileStr;										  
// 	std::string		tmp;
// 	std::string::iterator begin;
// 	std::string::iterator end;
// 	std::string::iterator aux;

// 	if (!file.is_open())
// 	{
// 		std::cout << "no toy" << std::endl;
// 		return (1);
// 	}
// 	while (true)
// 	{
// 		file >> tmp;
// 		if (file.eof())
// 			break;
// 		fileStr += tmp + " ";
// 	}
// 	std::cout << "**" << fileStr << "**" <<std::endl;
// 	begin = fileStr.begin();
// 	end = fileStr.end();
// 	aux = fileStr.begin();
// 	// std::cout << "Should be equal : " << (begin == aux) << std::endl;
// 	// std::cout << "fkkk" << std::string(begin, end) << std::endl;
// 	// std::cout << "Should be equal : " << (begin == aux) << std::endl;

// 	ParsingGlobal 	parser(begin, end, aux);
// 	try
// 	{
// 		parser.parse();
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << e.what() << '\n';
// 	}
// 	std::cout << "-------------------------------------" << std::endl;
// 	parser.recursivePrint();
// 	std::cout << "-------------------------------------" << std::endl;
// 	return 0;
	
// }

// int	testParseHtpp(void)	
// {
// 	Parsing test;
// 	std::cout << TUR << "test: " << ((int)test.getTypeLine("hola\n")[0]) << END << std::endl;
// 	return (0);
// }

void testPath(std::string test)
{
	std::cout << "Testing:" << test << ":" << std::endl;
	try {
		Path path(test);
		std::cout << GREEN << "Path: " << path << END << std::endl << std::endl;

	} catch (Path::InvalidPathException &e) {
		std::cout << e.what() << std::endl;
	}
}

void testPath(std::string str1, std::string str2)
{
	std::cout << "Testing:" << str1 << " + "  << str2 << std::endl;
	try 
	{
		Path path1(str1);
		Path path2(str2);
		path1.append(path2);
		path1.normalize();
		std::cout << GREEN << "Path: " << END << path1 << std::endl << std::endl;

	} catch (Path::InvalidPathException &e) {
		std::cout << e.what() << std::endl;
	}
}

void	testUri(std::string test)
{
	Uri::Parse(test);
}

void pathTests(void)
{
	testPath("/");
	testPath("./");
	testPath("path0/path1");
	testPath("/path0/path1");
	testPath("path0/path1", "path2/");
	testPath("/path0/path1/", "path2/");
	testPath("path0/path1/", "/path2/");
	testPath("/path0/path1/", "/path2/");
	testPath("/", "/");
	testPath("/fkd/..//////////////////////////////sdaf/.//////////a/./");
	testPath("/path0", "sdaf/");
}

void testExtension(std::string inp)
{
	Path path = inp;
	std::cout << "testing: " << path << std::endl;

	std::cout << "extension: " << path.getExtension() << std::endl;
	std::cout << std::endl;
}

void extensionTests(void)
{
	testExtension("hello/empty");
	testExtension("hello/.empty");
	testExtension("hello/.wololo.ext");
	testExtension("hello/wololo.ext");
	testExtension("hello/wololo.ext");

	testExtension("asloEmpty/.wololo.ext.");
	testExtension("asloEmpty/wololo.ext..");
	testExtension("asloNOTEmpty/wololo..ext");
}

int  main(void)
{
	// fileConfig();
	// testParseHtpp();
//	testPath("/queso/./Dirhola/../fkkkk");
//	testPath("////////");
//	testPath("././././.");
//	testPath("./");
//	testPath("");
//	testPath("  ");
//	testPath("runaway/../..");

	//std::list<std::string> res = ft_split("GET POST DELEETE", ' ');
	//for (std::list<std::string>::iterator iter = res.begin(); iter != res.end(); iter++)
		//std::cout << YELLOW << "word: " << END << *iter << std::endl;
	// testUri("http://somehost.com/cgi-bin/somescript/this?%2eis%2epath%3binfo");
	/*
		path0/path1
	*/
 
	extensionTests();
	return 0;
/*	
	Path sdaf = Path("/sdaf");
	sdaf.append(Path("/"));
	std::cout << "Is file: " << sdaf.getIsFile();
	std::cout << "  sdaf: " << sdaf  << std::endl;
	return (0);
	*/
}

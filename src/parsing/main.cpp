/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:56:11 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/25 09:56:17 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parsing.hpp"

	// DIR *directory = opendir("./");
	// struct dirent *foo = readdir(directory);
	// while(foo != NULL)
	// {
	// 	std::cout << foo->d_name << std::endl;
	// 	foo = readdir(directory);
	// }
	
int  main(void)
{
	// int fd = open("testHttp/hola.txt", O_RDONLY);
	int fd = open("testHttp/defaultOK.txt", O_RDONLY);
	if (fd == -1)
	{
		std::cout << "Error: open file" << std::endl;
		return (EXIT_FAILURE);
	}
	Parsing http;
	http.parsingHeader(http.readSocket(fd));
	close(fd);
	return ( EXIT_SUCCESS );
}

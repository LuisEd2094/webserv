/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:56:11 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/21 16:25:38 by dacortes         ###   ########.fr       */
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
	int fd = open("test.txt", O_RDONLY);
	Parsing http;
	http.parsingHeader(http.readSocket(fd));
	close(fd);
	return ( EXIT_SUCCESS );
}

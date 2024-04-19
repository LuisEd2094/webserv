/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:56:11 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/18 19:31:29 by dacortes         ###   ########.fr       */
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
	(Parsing().checkMethod(Parsing().readSocket(fd)));
	close(fd);
	if (Parsing().isMethods("GET") == false)
		std::cout << "OK" << std::endl;
	else
		std::cout << "KO" << std::endl;
	return ( EXIT_SUCCESS );
}

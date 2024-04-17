/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dacortes <dacortes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 10:56:11 by dacortes          #+#    #+#             */
/*   Updated: 2024/04/17 16:57:46 by dacortes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Parsing.hpp"

int  main(void)
{
	int fd = open("test.txt", O_RDONLY);
	(Parsing(fd));
	close(fd);
	if (Parsing().isMethods("GET") == false)
		std::cout << "OK" << std::endl;
	else
		std::cout << "KO" << std::endl;
	return ( EXIT_SUCCESS );
}

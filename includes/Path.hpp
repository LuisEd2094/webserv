#include <string>
#include <algorithm>
#include <iostream>
#include <list>
#include "Parsing.hpp"

struct Path
{
	public:
		std::list<std::string> directories;
		static Path Parse(std::string pathStr)
		{
			Path result;

			result.directories = ft_split(pathStr, '//');
			
			return (result);
		}
};

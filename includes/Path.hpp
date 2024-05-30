#include <string>
#include <algorithm>
#include <iostream>
#include <list>
#include "Parsing.hpp"


class Path
{
	private:
		void normalize1dot(std::list<std::string>::iterator &curFile);
		void normalize2dot(std::list<std::string>::iterator &curFile);

		std::list<std::string> directories;

	public:
	 	Path();
		Path(std::string pathStr);
		Path(Path &orig);
		Path &operator=(Path &orig);
		operator std::string();
		~Path();	
		void normalize(void);
};



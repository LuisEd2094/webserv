#include <string>
#include <algorithm>
#include <iostream>
#include <list>
#include <exception>
#include "Parsing.hpp"


class Path
{
	private:
		void deleteAndBack(std::list<std::string>::iterator &curFile);
//		void normalize2dot(std::list<std::string>::iterator &curFile);

		std::list<std::string> directories;

	public:
	 	Path();
		Path(std::string pathStr);
		Path(Path &orig);
		Path &operator=(Path &orig);
		operator std::string();
		~Path();	
		int normalize(void);

	class InvalidPathException: public std::exception
	{
		 public: 
			//virtual const char* what() const throw() {
			virtual const char* what() const throw() {
				return "ERROR on file path"; 
			}
	} ;
};



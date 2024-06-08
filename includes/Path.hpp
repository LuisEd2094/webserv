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
		bool isRelative;
		void popBegin(int ammount);
//		void normalize2dot(std::list<std::string>::iterator &curFile);

		std::list<std::string> directories;

	public:
	 	Path();
		Path(std::string pathStr);
		Path(const Path &orig);
		Path &operator=(const Path &orig);
		operator std::string();
		~Path();	

		int		size() { return directories.size(); };	
		void	setIsRelative(bool v) { isRelative = v; };
		bool	getIsRelative(void) const { return isRelative; };

		void	append(Path tail);
		int		normalize(void);
		bool	includes(const Path &compPath) const;
		bool	included(const Path &compPath) const;

	class InvalidPathException: public std::exception
	{
		 public: 
			//virtual const char* what() const throw() {
			virtual const char* what() const throw() {
				return "ERROR on file path"; 
			}
	} ;
};

std::ostream &operator<<(std::ostream &os, const Path &obj);


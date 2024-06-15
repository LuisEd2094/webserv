#ifndef PATH_HPP
# define PATH_HPP
# include <string>
# include <algorithm>
# include <iostream>
# include <list>
# include <exception>
# include "Parsing.hpp"
#include <sys/stat.h>

# define PATH_TO_C_STR(path) ((std::string) (const_cast<Path &>((path)))).c_str()

class Path
{
	private:
		void deleteAndBack(std::list<std::string>::iterator &curFile);
		bool isRelative;
		bool isFile;
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
		void	setIsFile(bool v) { isFile = v; };
		bool	getIsRelative(void) const { return isRelative; };
		bool	getIsFile(void) const { return isFile; };

		void	popBegin(int ammount);
		void	append(Path tail);
		int		normalize(void);
		bool	includes(const Path &compPath) const;
		bool	included(const Path &compPath) const;

		bool	assertFileExists(void) const;
		bool	assertDirExists(void) const;

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

#endif


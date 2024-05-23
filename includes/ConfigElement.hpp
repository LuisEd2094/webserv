#ifndef  CONFIG_ELEMENT_HPP
# define CONFIG_ELEMENT_HPP

# include "ParsingGlobal.hpp"

class ConfigElement
{
	private:

	public:

		class ParamError: public std::exception
		{
			private:
				std::string _msgError;
			public:
				ParamError(const std::string& msg): _msgError(msg){}
				virtual const char *what() const throw()
				{
					return (_msgError.c_str());
				}
				~ParamError(void) throw(){}
		};
};
#endif

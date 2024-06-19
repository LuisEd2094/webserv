#ifndef  CONFIG_ELEMENT_HPP
# define CONFIG_ELEMENT_HPP
class ConfigElement;
# include "ParsingGlobal.hpp"


class ConfigElement
{
	private:

	public:
		int			nestedPrint;
		static void configure(ParsingGlobal parsedData);
		static std::string genSpace(int ammount);
		virtual ~ConfigElement(void){};

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

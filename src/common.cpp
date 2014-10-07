#include "common.h"
#include "Logger.hpp"
uint8_t parseDigit(char& digit){
if (digit >= '0' && digit <= '9')
	return  digit - 48;
if (digit >= 'a' && digit <= 'f')
	return digit - 87; //start of lowercase a is 97, hex value of a starts at 10d, so -97 + 10 = 87
else
	return 0;

}

void strToLowerCase(std::string& in)
{
	std::transform(in.begin(),in.end(), in.begin(), convlower);
}
void strToUpperCase(std::string& in)
{
	std::transform(in.begin(),in.end(), in.begin(), convupper);
}

void trim(std::string& in, char toTrim){
		 
   		 std::remove( in.begin(), in.end(), toTrim );
}
char convlower(char in)
{
	if ( in >= 'A'&& in <='Z')
		return in-('A' -'a');

	return in;
}
char convupper(char in)
{
	if ( in >= 'a'&& in <='z')
		return in + ('A' - 'a');

	return in;
}




namespace detail {
	static unsigned int ios_indent = std::ios_base::xalloc();
};


ostream& setIndent(ostream& ostreamInstance, unsigned int indentlevel)
{
	ostreamInstance.iword(detail::ios_indent) = indentlevel;
	return ostreamInstance;
}

ostream& IncreaseIndent(ostream& ostreamInstance)
{
	ostreamInstance.iword(detail::ios_indent)++;
	return ostreamInstance;
}
ostream& DecreaseIndent(ostream& ostreamInstance)
{
	if (ostreamInstance.iword(detail::ios_indent) > 0)
	{
		ostreamInstance.iword(detail::ios_indent)--;
	}
	return ostreamInstance;

}

ostream& Indent(ostream& streamInstance)
{
	for(unsigned int i = 0; i < streamInstance.iword(detail::ios_indent); i++)
	{
		streamInstance << '\t';
	}
	
	return streamInstance;
}

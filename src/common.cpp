#include "common.h"

uint8_t parseDigit(char& digit){
if (digit >= '0' && digit <= '9')
	return  digit - 48;
if (digit >= 'a' && digit <= 'f')
	return digit - 87; //start of lowercase a is 97, hex value of a starts at 10d, so -97 + 10 = 87


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
		 
		in.erase(
   		 remove( in.begin(), in.end(), toTrim ),
   		 in.end()
   		 );
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

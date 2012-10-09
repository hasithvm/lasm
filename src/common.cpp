#include "common.h"

uint8_t parseDigit(char& digit){
if (digit >= '0' && digit <= '9')
	return  digit - 48;
if (digit >= 'a' && digit <= 'f')
	return digit - 87; //start of lowercase a is 97, hex value of a starts at 10d, so -97 + 10 = 87


}

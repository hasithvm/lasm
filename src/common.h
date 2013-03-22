#ifndef _COMMON_H
#define _COMMON_H
#include <stdint.h>
#include <string>
#include <algorithm>
#include "data.h"
//common utility functions for data manipulation.



uint8_t parseDigit(char& digit);
void strToLowerCase(std::string& str);
void strToUpperCase(std::string& str);
char convlower(char in);
char convupper(char in);
void trim(std::string& in, char toTrim);


#endif

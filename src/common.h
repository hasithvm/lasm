#ifndef _COMMON_H
#define _COMMON_H
#include <string>
#include <algorithm>
#include <cstdint>

//common utility functions for data manipulation.



uint8_t parseDigit(char& digit);
void strToLowerCase(std::string& str);
void strToUpperCase(std::string& str);
char convlower(char in);
char convupper(char in);
void trim(std::string& in, char toTrim);


#endif

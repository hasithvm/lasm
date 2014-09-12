#ifndef _COMMON_H
#define _COMMON_H
#include <string>
#include <algorithm>

#ifndef _MSC_VER
#include <stdint.h>
#else
#include <cstdint>
#endif

#include <iostream>
using namespace std;
//common utility functions for data manipulation.



uint8_t parseDigit(char& digit);
void strToLowerCase(std::string& str);
void strToUpperCase(std::string& str);
char convlower(char in);
char convupper(char in);
void trim(std::string& in, char toTrim);


ostream& Indent(ostream& streamInstance);
ostream& DecreaseIndent(ostream& ostreamInstance);
ostream& IncreaseIndent(ostream& ostreamInstance);
ostream& setIndent(ostream& ostreamInstance, unsigned int indentlevel);

#endif

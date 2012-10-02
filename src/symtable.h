#ifndef _SYMTABLE_H
#define _SYMTABLE_H
#include <stdlib.h>
#include <map>
#include <stdint.h>
#include <iostream>
#include "data.h"
using namespace std;
#define OPCODE(op,bin)	ret[std::string(op)] = bin




class SymTable{
		public:
						SymTable();
						uint8_t get(std::string opcode);
						void repr();
		private:
					typedef map<std::string, uint8_t> SymMap;
						static SymMap m_opmap;
						static SymMap generate();
};

#endif

#ifndef _SYMTABLE_H
#define _SYMTABLE_H
#include <stdlib.h>
#include <map>
#include <stdint.h>
#include <iostream>
#include "data.h"
#include <initializer_list>
using namespace std;

#define OPCODE(op,bin)	ret[std::string(op)] = bin


//Constants for operand types.
#define REG8	2
#define REG16	4
#define MEM8	8
#define MEM16	16
#define IMM8	32
#define IMM16	64

//Defines a bitmask for the MOD/RM Extension for one-byte opcodes. See Intel refMan.
#define MOD_RM_EXT 0xFF

//Bitmask for the bit values.
#define MOD_RM_EXTMASK 0x07

#define SINGLE_OP 0x00



#define OP_DIR 0

#define OP_SIGN 1

#define OP_WORD 2
//bitmask for operand count {0,1,2,3?}
#define OP_OPERAND_COUNT 12
//mod is fixed
#define OP_MOD_FIXED 16
//reg is fixed
#define OP_REG_FIXED 32
//rm is fixed
#define OP_RM_FIXED 64
//next byte has the prefixes necessary.
#define OP_PREFIX_BYTE 128


typedef std::vector<uint8_t> 	OpType;
typedef std::vector< OpType > 	OpVars;



//single byte opcodes supported only!
//OpType format: [future implementation, 0x00] opcode modRM (optional) operand1 operand2


class SymTable{
		public:
						SymTable();
						uint8_t get(std::string opcode);
						void repr();
		private:
					typedef map<std::string, OpVars > SymMap;
						static SymMap m_opmap;
						static SymMap generate();
};

#endif

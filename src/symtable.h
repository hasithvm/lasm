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
#define OP_8BIT		0
#define	OP_16BIT	1
#define REG			2

#define MEM			4
#define IMM			8

#define REG8		REG | OP_8BIT
#define REG16		REG | OP_16BIT

#define MEM8		MEM | OP_8BIT
#define MEM16		MEM | OP_16_BIT

#define IMM8	IMM | OP_8BIT
#define IMM16	IMM | OP_16BIT


#define REG_PRESET	16


#define REG_AX	0x00 << 5
#define REG_CX	0x01 << 5
#define REG_DX	0x02 << 5
#define REG_BX	0x03 << 5

#define REG_SP	0x04 << 5
#define REG_BP	0x05 << 5
#define REG_SI	0x06 << 5
#define REG_DI	0x07 << 5

#define REG_AL	0x00 << 5
#define REG_CL	0x01 << 5
#define REG_DL	0x02 << 5
#define REG_BL	0x03 << 5
#define REG_AH	0x04 << 5
#define REG_CH	0x05 << 5
#define REG_DH	0x06 << 5
#define REG_BH	0x07 << 5

#define isset(o, m) (m & o == m)

//Defines a bitmask for the MOD/RM Extension for one-byte opcodes. See Intel refMan.
#define MOD_RM_EXT 0xFF

//single byte opcode

//bitmask for operand count {0,1,2,3?}
#define OP_OPERANDS 3

#define OP_NO_OPERANDS 0
#define OP_SINGLE_OPERAND 1
#define OP_TWO_OPERANDS 2
#define OP_THREE_OPERANDS 3

//there is no modRM byte
#define OP_NO_MODRM 4
//jump-type operation, uses displacement (16-bit?)
#define OP_IS_JUMP 8



//next byte after has the REG field fixed.
#define OP_REG_EXT 32






typedef std::vector<uint8_t> 	OpType;
typedef std::vector< OpType > 	OpVars;



//single byte opcodes supported only!
//OpType format: [future implementation, 0x00] opcode modRM (optional) operand1 operand2


class SymTable{
		public:
						SymTable();
						uint8_t get(std::string opcode);
						void repr();
						bool exists(std::string opcode);
						OpVars& at(std::string search); 
		private:
					typedef map<std::string, OpVars > SymMap;
						static SymMap m_opmap;
						static SymMap generate();
};

#endif

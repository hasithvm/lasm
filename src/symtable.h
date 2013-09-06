#ifndef _SYMTABLE_H
#define _SYMTABLE_H
#include <stdlib.h>
#include <map>
#include <stdint.h>
#include <iostream>
#include "data.h"
#include <initializer_list>
#include "symtable-generated.hpp"
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
#define MEM16		MEM | OP_16BIT

#define IMM8	IMM | OP_8BIT
#define IMM16	IMM | OP_16BIT


#define REG_PRESET	16
#define ENC_REG_AX	0x00
#define ENC_REG_CX	0x01
#define ENC_REG_DX	0x02
#define ENC_REG_BX	0x03

#define ENC_REG_SP	0x04
#define ENC_REG_BP	0x05
#define ENC_REG_SI	0x06
#define ENC_REG_DI	0x07

#define ENC_REG_AL	0x00
#define ENC_REG_CL	0x01
#define ENC_REG_DL	0x02
#define ENC_REG_BL	0x03
#define ENC_REG_AH	0x04
#define ENC_REG_CH	0x05
#define ENC_REG_DH	0x06
#define ENC_REG_BH	0x07

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
#define OP_ONE_OPERAND 1
#define OP_TWO_OPERANDS 2
#define OP_THREE_OPERANDS 3

//there is no modRM byte
#define OP_NO_MODRM 4
//jump-type operation, uses displacement (16-bit?)
#define OP_IS_JMP 8

//used only in the PUSH. OR the dst regis 
#define OP_REG_ADD 16


//next byte after has base value of the mod/rm byte. all mod/rms are ORed with it.
#define OP_MODRM_EXT 32






typedef std::vector<uint8_t> 	OpType;
typedef std::vector< OpType > 	OpVars;



//single byte opcodes supported only!
//OpType format: [future implementation, 0x00] opcode modRM (optional) operand1 operand2
/*class OpType{
public:
		OpType* instance();
		unsigned char& operator[](int index);
		int size();
		void realign(inst_variant* ptr);
private:
		OpType();
		OpType(OpType const&){};			// copy constructor is private
		OpType& operator=(OpType const&){}; // assignment operator is private
		inst_variant* m_ptr;
		static OpType* m_pInstance;
};	

class OpVars{
	public:		
				OpVars* instance();
				OpType& operator[](int index);
				int size();
				void realign(int index);
	private:
			OpVars();
			OpVars(OpVars const&){};			// copy constructor is private
			OpVars& operator=(OpVars const&){}; // assignment operator is private
			int m_index;
			static OpVars* m_pInstance;

};
*/



class SymTable{
		public:
						SymTable();
						uint8_t get(std::string opcode);
						void repr();
						bool exists(std::string opcode);
						OpVars& at(std::string search); 
						OpVars& at(int index); 
		private:
					typedef map<std::string, OpVars > SymMap;
						static SymMap m_opmap;
						static SymMap generate();
};




#endif

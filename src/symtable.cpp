#include "symtable.h"

SymTable::SymMap SymTable::m_opmap(SymTable::generate());

SymTable::SymMap SymTable::generate(){
	SymTable::SymMap ret ={
	{"add",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x04 ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x05 ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x00, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x01, REG16|IMM16, REG16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0x81, REG16|MEM16, IMM16}
			} 
	},
	{"and",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x24 ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x25 ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x20, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x21, REG16|IMM16, REG16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x04 << 3,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x03 << 3,0x81, REG16|MEM16, IMM16},

			}	
			
	},
	{"call",
	 		{
	 		{OP_ONE_OPERAND | OP_NO_MODRM,0xE8,IMM16}
	 		}
	 
	},
	{"cmp",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x3C, REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x3D ,REG16 | REG_PRESET | REG_AX, IMM16},
			}
	
	},
	{"dec",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x01<<3,0xFE, REG8|MEM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x01<<3,0xFF, REG16|MEM16}
			}
	
	},
	{"div",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x06 << 3,0xF6, REG8|MEM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x06 << 3,0xF7, REG16|MEM16}
			}
	
	
	},
	{"hlt",
	{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xF4}
	}
	
	
	},
	{"in",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xEE, REG8 |REG_PRESET | REG_AL, REG16 | REG_PRESET | REG_DX}
			}
	
	
	},
	{"inc",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x00,0xFE, REG8|MEM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x00,0xFF, REG16|MEM16}
			}
	
	
	},
	{"int", {
			{OP_ONE_OPERAND | OP_NO_MODRM,0xCD, IMM8}
			}
	
	},
	{"iret",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xCF}
			}
	
	
	},
	{"ja",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x77, IMM8}
			}
	
	
	},
	{"jae",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x73, IMM8}
			}
	
	
	},
	{"jb",
			{	
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x72, IMM8}
			}
	
	
	},
	{"jbe",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x76, IMM8}
			}
	
	
	},
	{"jc",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x72, IMM8}
			}
	
	
	},
	{"je",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x74, IMM8}
			}
	
	
	},
	{"jg",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7f, IMM8}
			}
	
	
	},
	{"jge",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7d, IMM8}
			}
	
	
	},
	{"jl",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7C, IMM8}
			}
	
	
	},
	{"jle",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7E, IMM8}
			}
	
	
	},
	{"jnc",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x73, IMM8}
			}
	
	
	},
	{"jne",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x75, IMM8}
			}
	
	
	},
	{"jno",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x71, IMM8}
			}
	
	
	},
	{"jns",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x79, IMM8}
			}
	
	
	},
	{"jnz",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x75, IMM8}
			}
	
	
	},
	{"jo",	{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x70, IMM8}
			}
	
	
	},
	{"js",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x78, IMM8}
			}
	
	
	},
	{"jz",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x74, IMM8}
			}
	
	
	},
	{"jmp",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP, 0xE9, IMM16}
			}
	
	
	},
	{"mov",
			{
			{OP_TWO_OPERANDS, 0x88, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x89, REG16|IMM16, REG16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0xC6, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0xC7, REG16|MEM16, IMM16},
			{OP_TWO_OPERANDS ,0x8A, REG8, MEM8},
			{OP_TWO_OPERANDS ,0x8B, REG8, MEM16},
			}
	
	
	},
	{"mul",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x04 << 3,0xF6, REG8|MEM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x04 << 3,0xF7, REG16|MEM16}
			}
	
	
	},
	{"neg",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x03 << 3, 0xF6, REG8|IMM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x03 << 3, 0xF7, REG16|IMM16},
			}
	
	
	},
	{"nop",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x90}
			}
	
	
	},
	{"not",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x02 << 3, 0xF6, REG8|IMM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x02 << 3, 0xF7, REG16|IMM16},
			}
	
	
	},
	{"or",
			{

			}
	
	
	},
	{"out",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xEE, REG16 |REG_PRESET | REG_DX, REG8 | REG_PRESET | REG_AL}
			}
	
	
	},
	{"pop",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_REG_ADD, 0x58, REG16}
			}
	
	
	},
	{"push",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_REG_ADD, 0x50, REG16}
			}
	
	
	},
	{"ret",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM,0xC3}
			}
	
	
	},
	{"shl",
			{

			}
	
	
	},
	{"shr",
			{

			}
	
	
	},
	{"sub",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x2C ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x2D ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x28, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x29, REG16|IMM16, REG16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x05 << 3,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x03 << 3,0x81, REG16|MEM16, IMM16}
			} 
	
	
	}
	};
	return ret;



};


void SymTable::repr(){

//clog << "p86asm supported opcodes:" << endl;
for (SymMap::iterator it=m_opmap.begin(); it != m_opmap.end();it++)
{
//clog << "\t opcode: " <<it->first <<" encoding:  " << hex2str(&it->second,1)<<  endl;
}
clog << endl;

}

bool SymTable::exists(std::string opcode){
	//clog << "SymTable: matching " << opcode << endl;
	strToLowerCase(opcode);
	if (m_opmap.count(opcode) > 0){
	//	clog << "SymTable: match found!" << endl;
		return true;
	}
	//clog << "SymTable: not an opcode" << endl;
	return false;
}
OpVars& SymTable::at(std::string search){

	return m_opmap.at(search);
}
SymTable::SymTable(){
}



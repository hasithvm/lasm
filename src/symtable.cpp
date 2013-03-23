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

			}
	
	
	},
	{"inc",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x00,0xFE, REG8|MEM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x00,0xFF, REG16|MEM16}
			}
	
	
	},
	{"int",
			{
			}
	
	
	},
	{"iret",
			{

			}
	
	
	},
	{"ja",
			{

			}
	
	
	},
	{"jae",
			{

			}
	
	
	},
	{"jb",
			{

			}
	
	
	},
	{"jbe",
			{

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

			}
	
	
	},
	{"jge",
			{

			}
	
	
	},
	{"jl",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7C, IMM8}
			}
	
	
	},
	{"jle",
			{

			}
	
	
	},
	{"jnc",
			{

			}
	
	
	},
	{"jne",
			{

			}
	
	
	},
	{"jno",
			{

			}
	
	
	},
	{"jns",
			{

			}
	
	
	},
	{"jnz",
			{

			}
	
	
	},
	{"jo",
			{

			}
	
	
	},
	{"js",
			{

			}
	
	
	},
	{"jz",
			{

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

			}
	
	
	},
	{"nop",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x90}
			}
	
	
	},
	{"not",
			{

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
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_REG_ADD, 0x90, REG16}
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

			}
	
	
	}
	};
	//	OPCODE("ADD", )
//	OPCODE("ADD",0x00);
//	OPCODE("AND",0xF1);
//	OPCODE("CALL",0x00);
//	OPCODE("CMP",0x00);
//	OPCODE("DEC",0x00);
//	OPCODE("DIV", 0x00);
//	OPCODE("HLT",0x00);
//	OPCODE("IN",0x00);
//	OPCODE("INC",0x00);
//	OPCODE("INT",0x00);
//	OPCODE("IRET",0x00);
//	OPCODE("JA",0x00);
//	OPCODE("JAE",0x00);
//	OPCODE("JB",0x00);
//	OPCODE("JBE",0x00);
//	OPCODE("JC",0x00);
//	OPCODE("JE",0x00);
//	OPCODE("JG",0x00);
//	OPCODE("JGE",0x00);
//	OPCODE("JL",0x00);
//	OPCODE("JLE",0x00);
//	OPCODE("JNC",0x00);
//	OPCODE("JNE",0x00);
//	OPCODE("JNO",0x00);
//	OPCODE("JNS",0x00);
//	OPCODE("JNZ",0x00);
//	OPCODE("JO",0x00);
//	OPCODE("JS",0x00);
//	OPCODE("JZ",0x00);
//	OPCODE("JMP",0x00);
//	OPCODE("MOV",0x00);
//	OPCODE("MUL",0x00);
//	OPCODE("NEG",0x00);
//	OPCODE("NOP",0x00);
//	OPCODE("NOT",0x00);
//	OPCODE("OR",0x00);
//	OPCODE("OUT",0x00);
//	OPCODE("POP",0x00);
//	OPCODE("PUSH",0x00);
//	OPCODE("RET",0x00);
//	OPCODE("SHL",0x00);
//	OPCODE("SHR",0x00);
//	OPCODE("SUB",0x00);
//	OPCODE("TST",0x00);
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



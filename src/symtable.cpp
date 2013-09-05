#include "symtable.h"

SymTable::SymMap SymTable::m_opmap(SymTable::generate());

SymTable::SymMap SymTable::generate(){
	SymTable::SymMap ret =
	{"aaa",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x37}
			}
	},
	{"aad",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xD5, 0x0A},
			{OP_ONE_OPERAND | OP_NO_MODRM, 0xD5, IMM8}
			}
	},
	{"aam",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xD4, 0x0A},
			{OP_ONE_OPERAND | OP_NO_MODRM, 0xD4, IMM8}
			}
	},
	{"aas",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x3F}
			}
	},
	{"adc",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x14 ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x15 ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x10, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x11, REG16|IMM16, REG16},
			{OP_TWO_OPERANDS, 0x13, REG16, REG16|MEM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x02,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x02,0x81, REG16|MEM16, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x02,0x83, REG16|MEM16, IMM8}
			} 
	},
	{"add",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x04 ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x05 ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x00, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x01, REG16|IMM16, REG16},
			{OP_TWO_OPERANDS, 0x03, REG16, REG16|IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0x81, REG16|MEM16, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0x83, REG16|MEM16, IMM8}
			} 
	},
	{"and",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x24 ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x25 ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x20, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x21, REG16|IMM16, REG16},
			{OP_TWO_OPERANDS, 0x23, REG16, REG16|MEM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x04 << 3, 0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x04 << 3, 0x81, REG16|MEM16, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x04 << 3, 0x83, REG16|MEM16, IMM8},
			}	
	},
	{"call",
	 		{
	 		{OP_ONE_OPERAND | OP_NO_MODRM,0xE8,IMM16},
	 		{OP_ONE_OPERAND | OP_MODRM_EXT, 0x02 << 3, 0xFF, REG16|MEM16}
	 		}
	},
	{"cbw",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x98}
			}
	},
	{"clc",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xF8}
			}
	},
	{"cld",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xFC}
			}
	},
	{"cli",
		{
		{OP_NO_OPERANDS | OP_NO_MODRM, 0xFA}
		}
	},
	{"cmc",
		{
		{OP_NO_OPERANDS | OP_NO_MODRM, 0xF5}
		}
	},
	{"cmp",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x3C, REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x3D ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x38, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x39, REG16|MEM16, REG16},
			{OP_TWO_OPERANDS, 0x3A, REG8, REG8|MEM8},
			{OP_TWO_OPERANDS, 0x3B, REG16, REG16|MEM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x07 << 3,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x07 << 3,0x81, REG16|MEM16, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x07 << 3,0x83, REG16|MEM16, IMM8},
			}
	},
	{"cmpsb",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xA6},
			}
	},
	{"cmpsw",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xA7},
			}
	},
	{"cwd",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x99},
			}
	},
	{"daa",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x27},
			}
	},
	{"das",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x2F},
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
	{"idiv",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x07 << 3, 0xF6, REG8|MEM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x07 << 3, 0xF7, REG16|MEM16},
			}
	},
	{"in",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xE4, REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xE5, REG16 | REG_PRESET | REG_AX, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xEC, REG8 |REG_PRESET | REG_AL, REG16 | REG_PRESET | REG_DX},
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xED, REG16 |REG_PRESET | REG_AX, REG16 | REG_PRESET | REG_DX}
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
	{"into",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xCE}
			}
	},
	{"iret",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xCF}
			}
	
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
	{"jcxz",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0xE3, IMM8}
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
	{"jna",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x76, IMM8}
			}
	},
	{"jnae",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x72, IMM8}
			}
	},
	{"jnb",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x73, IMM8}
			}
	},
	{"jnbe",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x77, IMM8}
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
	{"jng",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7E, IMM8}
			}
	},
	{"jnge",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7C, IMM8}
			}
	},
	{"jnl",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7D, IMM8}
			}
	},
	{"jnle",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7F, IMM8}
			}
	},
	{"jno",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x71, IMM8}
			}
	},
	{"jnp",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7B, IMM8}
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
	{"jp",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7A, IMM8}
			}
	},
	{"jpe",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7A, IMM8}
			}
	},
	{"jpo",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP,0x7B, IMM8}
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
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP, 0xE9, IMM16},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x04 << 3, 0xFF, REG16|MEM16}
			}
	},
	{"lahf",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x9F},
			}
	},
	{"lds",
			{
			{OP_TWO_OPERANDS, 0xC5, REG16, MEM16},
			}
	},
	{"lea",
			{
			{OP_TWO_OPERANDS, 0x8D, REG16, MEM16},
			}
	},
	{"les",
			{
			{OP_TWO_OPERANDS, 0xC4, REG16, MEM16},
			}
	},
	{"lodsb",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xAC},
			}
	},
	{"lodsw",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xAD},
			}
	},
	{"loop",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP, 0xE2, IMM8},
			}
	},
	{"loope",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP, 0xE1, IMM8},
			}
	},
	{"loopne",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP, 0xE0, IMM8},
			}
	},
	{"mov",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM | OP_REG_ADD, 0xB0, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM | OP_REG_ADD, 0xB8, IMM16},
			{OP_TWO_OPERANDS, 0x88, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x89, REG16|MEM16, REG16},
			{OP_TWO_OPERANDS ,0x8A, REG8, MEM8},
			{OP_TWO_OPERANDS ,0x8B, REG8, MEM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0xC6, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00,0xC7, REG16|MEM16, IMM16},
			}
	},
	{"movsb",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xA4},
			}
	},
	{"movsw",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xA5},
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
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x03 << 3, 0xF6, REG8|MEM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x03 << 3, 0xF7, REG16|MEM16},
			}
	},
	{"nop",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x90}
			}
	},
	{"not",
			{
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x02 << 3, 0xF6, REG8|MEM8},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x02 << 3, 0xF7, REG16|MEM16},
			}
	},
	{"or",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x0C ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x0D ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x08, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x09, REG16|MEM16, REG16},
			{OP_TWO_OPERANDS, 0x0A, REG8, REG8|MEM8},
			{OP_TWO_OPERANDS, 0x0B, REG16, REG16|MEM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x01 << 3,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x01 << 3,0x81, REG16|MEM16, IMM16}
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x01 << 3,0x83, REG16|MEM16, IMM8}
			}
	},
	{"out",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xEE, REG16 |REG_PRESET | REG_DX, REG8 | REG_PRESET | REG_AL}
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xEF, REG16 |REG_PRESET | REG_DX, REG16 | REG_PRESET | REG_AX}
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xE6, IMM8, REG8 | REG_PRESET | REG_AL}
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xE7, IMM8, REG8 | REG_PRESET | REG_AX}
			}
	},
	{"pop",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_REG_ADD, 0x58, REG16},
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x00, 0x8F, REG16|MEM16},
			}
	},
	{"popa",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x61},
			}
	},
	{"popf",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x9D},
			}
	},
	{"push",
			{
			{OP_ONE_OPERAND | OP_NO_MODRM | OP_REG_ADD, 0x50, REG16}
			{OP_ONE_OPERAND | OP_MODRM_EXT, 0x06 << 3, 0xFF, REG16|MEM16},
			{OP_ONE_OPERAND | OP_NO_MODRM, 0x6A, IMM8},
			{OP_ONE_OPERAND | OP_NO_MODRM, 0x68, IMM16},
			}
	},
	{"pusha",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x60},
			}
	},
	{"pushf",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x9C},
			}
	},
	{"rcl",
			{
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x02 << 3, 0xD2, REG8 | MEM8, REG8 | REG_PRESET | REG_CL}, 
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x02 << 3, 0xD3, REG16 | MEM16, REG8 | REG_PRESET | REG_CL},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x02 << 3, 0xC0, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x02 << 3, 0xC1, REG16 | MEM16, IMM16},
			}
	},
	{"rcr",
			{
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x03 << 3, 0xD2, REG8 | MEM8, REG8 | REG_PRESET | REG_CL}, 
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x03 << 3, 0xD3, REG16 | MEM16, REG8 | REG_PRESET | REG_CL},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x03 << 3, 0xC0, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x03 << 3, 0xC1, REG16 | MEM16, IMM16},
			}
	},
	{"ret",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM,0xC3}
			}
	},
	{"rol",
			{
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x00, 0xD2, REG8 | MEM8, REG8 | REG_PRESET | REG_CL}, 
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x00, 0xD3, REG16 | MEM16, REG8 | REG_PRESET | REG_CL} 
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x00, 0xC0, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x00, 0xC1, REG16 | MEM16, IMM16},
			}
	},
	{"ror",
			{
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x01 << 3, 0xD2, REG8 | MEM8, REG8 | REG_PRESET | REG_CL},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x01 << 3, 0xD3, REG8 | MEM8, REG8 | REG_PRESET | REG_CL}  
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x01 << 3, 0xC0, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x01 << 3, 0xC1, REG16 | MEM16, IMM16},
			}
	},
	{"sahf",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0x9E},
			}
	},
	{"sal",
			{
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x04 << 3, 0xD2, REG8 | MEM8, REG8 | REG_PRESET | REG_CL},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x04 << 3, 0xD3, REG8 | MEM8, REG8 | REG_PRESET | REG_CL}  
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x04 << 3, 0xC0, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x04 << 3, 0xC1, REG16 | MEM16, IMM16},
			}
	},
	{"sar",
			{
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x07 << 3, 0xD2, REG8 | MEM8, REG8 | REG_PRESET | REG_CL}, 
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x07 << 3, 0xD3, REG16 | MEM16, REG8 | REG_PRESET | REG_CL} 
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x07 << 3, 0xC0, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x07 << 3, 0xC1, REG16 | MEM16, IMM16},
			}
	},
	{"sbb",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0x1C, REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0x1D, REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x03 << 3, 0x80, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x03 << 3, 0x81, REG16 | MEM16, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x03 << 3, 0x83, REG16 | MEM16, IMM8},
			{OP_TWO_OPERANDS, 0x18, REG8 | MEM8, REG8},
			{OP_TWO_OPERANDS, 0x19, REG16 | MEM16, REG16},
			{OP_TWO_OPERANDS, 0x1A, REG8, MEM8 | REG8},
			{OP_TWO_OPERANDS, 0x1B, REG16, MEM16 | REG16},
			}
	},
	{"scasb",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xAE},
			}
	},
	{"scasw",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xAF},
			}
	},
	{"shl",
			{
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x04 << 3, 0xD2, REG8 | MEM8, REG8 | REG_PRESET | REG_CL},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x04 << 3, 0xD3, REG8 | MEM8, REG8 | REG_PRESET | REG_CL}  
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x04 << 3, 0xC0, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x04 << 3, 0xC1, REG16 | MEM16, IMM16},
			}
	},
	{"shr",
			{
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x05 << 3, 0xD2, REG8 | MEM8, REG8 | REG_PRESET | REG_CL}, 
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x05 << 3, 0xD3, REG16 | MEM16, REG8 | REG_PRESET | REG_CL} 
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x05 << 3, 0xC0, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT ,0x05 << 3, 0xC1, REG16 | MEM16, IMM16},
			}
	},
	{"stc",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xF9},
			}
	},
	{"std",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xFD},
			}
	},
	{"sti",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xFB}
			}
	},
	{"stosb",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xAA},
			}
	},
	{"stosw",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xAB},
			}
	},
	{"sub",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x2C ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x2D ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x28, REG8|MEM8, REG8},
			{OP_TWO_OPERANDS, 0x29, REG16|MEM16, REG16},
			{OP_TWO_OPERANDS, 0x2A, REG8, MEM8 | REG8},
			{OP_TWO_OPERANDS, 0x2B, REG16, MEM16 | REG16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x05 << 3,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x05 << 3,0x81, REG16|MEM16, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x05 << 3,0x83, REG16|MEM16, IMM8}
			} 
	},
	{"test",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xA8, REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM, 0xA9, REG16 | REG_PRESET | REG_Ax, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00, 0xF6, REG8 | MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x00, 0xF7, REG16 | MEM16, IMM16},
			{OP_TWO_OPERANDS, 0x85, REG8 | MEM8, REG8},
			{OP_TWO_OPERANDS, 0x85, REG16 | MEM16, REG16},
			}
	},
	{"xchg",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM | OP_REG_ADD, 0x90, REG16, REG16 | REG_PRESET | REG_AX},
			{OP_TWO_OPERANDS, 0x86, REG8 | MEM8, REG8},
			{OP_TWO_OPERANDS, 0x87, REG16 | MEM16, REG16},
			}
	},
	{"xlatb",
			{
			{OP_NO_OPERANDS | OP_NO_MODRM, 0xD7},
			}
	},
	{"xor",
			{
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x34 ,REG8 | REG_PRESET | REG_AL, IMM8},
			{OP_TWO_OPERANDS | OP_NO_MODRM ,0x35 ,REG16 | REG_PRESET | REG_AX, IMM16},
			{OP_TWO_OPERANDS, 0x30, REG8 | MEM8, REG8},
			{OP_TWO_OPERANDS, 0x31, REG16 | MEM16, REG16},
			{OP_TWO_OPERANDS, 0x32, REG8, MEM8 | REG8},
			{OP_TWO_OPERANDS, 0x33, REG16, MEM16 | REG16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x06 << 3,0x80, REG8|MEM8, IMM8},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x06 << 3,0x81, REG16|MEM16, IMM16},
			{OP_TWO_OPERANDS | OP_MODRM_EXT, 0x06 << 3,0x83, REG16|MEM16, IMM8}
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



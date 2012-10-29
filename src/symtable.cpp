#include "symtable.h"

SymTable::SymMap SymTable::m_opmap(SymTable::generate());

SymTable::SymMap SymTable::generate(){
	SymTable::SymMap ret ={
	{"add",
			{
			{OP_DIR | OP_WORD | OP_TWO_OPERANDS ,0xC0,REG8|REG16, REG8|REG16},
			{OP_SIGN | OP_WORD 	,0x00}
			} 
	},
	{"and",
	 		{{0x00,0x00,REG8|REG16, IMM8|IMM16}}
	 
	},
	{"call",
	 		{
	 		{0x00,0xE8,IMM16}
	 		}
	 
	},
	{"cmp",
			{
			{0x00}
			}
	
	
	},
	{"dec",
			{
			{0x00}
			}
	
	
	},
	{"div",
			{
			{0x00}
			}
	
	
	},
	{"hlt",
	{
		{0x00}
	}
	
	
	},
	{"in",
			{
			{0x00}
			}
	
	
	},
	{"inc",
			{
			{0x00}
			}
	
	
	},
	{"int",
			{
			{0x00}
			}
	
	
	},
	{"iret",
			{
			{0x00}
			}
	
	
	},
	{"ja",
			{
			{0x00}
			}
	
	
	},
	{"jae",
			{
			{0x00}
			}
	
	
	},
	{"jb",
			{
			{0x00}
			}
	
	
	},
	{"jbe",
			{
			{0x00}
			}
	
	
	},
	{"jc",
			{
			{0x00}
			}
	
	
	},
	{"je",
			{
			{0x00}
			}
	
	
	},
	{"jg",
			{
			{0x00}
			}
	
	
	},
	{"jge",
			{
			{0x00}
			}
	
	
	},
	{"jl",
			{
			{0x00}
			}
	
	
	},
	{"jle",
			{
			{0x00}
			}
	
	
	},
	{"jnc",
			{
			{0x00}
			}
	
	
	},
	{"jne",
			{
			{0x00}
			}
	
	
	},
	{"jno",
			{
			{0x00}
			}
	
	
	},
	{"jns",
			{
			{0x00}
			}
	
	
	},
	{"jnz",
			{
			{0x00}
			}
	
	
	},
	{"jo",
			{
			{0x00}
			}
	
	
	},
	{"js",
			{
			{0x00}
			}
	
	
	},
	{"jz",
			{
			{0x00}
			}
	
	
	},
	{"jmp",
			{
			{0x00}
			}
	
	
	},
	{"mov",
			{
			{0x00}
			}
	
	
	},
	{"mul",
			{
			{0x00}
			}
	
	
	},
	{"neg",
			{
			{0x00}
			}
	
	
	},
	{"nop",
			{
			{0x00}
			}
	
	
	},
	{"not",
			{
			{0x00}
			}
	
	
	},
	{"or",
			{
			{0x00}
			}
	
	
	},
	{"out",
			{
			{0x00}
			}
	
	
	},
	{"pop",
			{
			{0x00}
			}
	
	
	},
	{"push",
			{
			{0x00}
			}
	
	
	},
	{"ret",
			{
			{0x00}
			}
	
	
	},
	{"shl",
			{
			{0x00}
			}
	
	
	},
	{"shr",
			{
			{0x00}
			}
	
	
	},
	{"sub",
			{
			{0x00}
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

clog << "p86asm supported opcodes:" << endl;
for (SymMap::iterator it=m_opmap.begin(); it != m_opmap.end();it++)
{
//clog << "\t opcode: " <<it->first <<" encoding:  " << hex2str(&it->second,1)<<  endl;
}
clog << endl;

}

bool SymTable::exists(std::string opcode){
	clog << "SymTable: matching " << opcode << endl;
	strToLowerCase(opcode);
	if (m_opmap.count(opcode) > 0){
		clog << "SymTable: match found!" << endl;
		return true;
	}
	clog << "SymTable: not an opcode" << endl;
	return false;
}

SymTable::SymTable(){
}



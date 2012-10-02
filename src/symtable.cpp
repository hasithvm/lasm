#include "symtable.h"

SymTable::SymMap SymTable::m_opmap(SymTable::generate());

SymTable::SymMap SymTable::generate(){
	SymTable::SymMap ret;
	
	OPCODE("ADD",0x00);
	OPCODE("AND",0xF1);
	OPCODE("CALL",0x00);
	OPCODE("CMP",0x00);
	OPCODE("DEC",0x00);
	OPCODE("DIV", 0x00);
	OPCODE("HLT",0x00);
	OPCODE("IN",0x00);
	OPCODE("INC",0x00);
	OPCODE("INT",0x00);
	OPCODE("IRET",0x00);
	OPCODE("JA",0x00);
	OPCODE("JAE",0x00);
	OPCODE("JB",0x00);
	OPCODE("JBE",0x00);
	OPCODE("JC",0x00);
	OPCODE("JE",0x00);
	OPCODE("JG",0x00);
	OPCODE("JGE",0x00);
	OPCODE("JL",0x00);
	OPCODE("JLE",0x00);
	OPCODE("JNC",0x00);
	OPCODE("JNE",0x00);
	OPCODE("JNO",0x00);
	OPCODE("JNS",0x00);
	OPCODE("JNZ",0x00);
	OPCODE("JO",0x00);
	OPCODE("JS",0x00);
	OPCODE("JZ",0x00);
	OPCODE("JMP",0x00);
	OPCODE("MOV",0x00);
	OPCODE("MUL",0x00);
	OPCODE("NEG",0x00);
	OPCODE("NOP",0x00);
	OPCODE("NOT",0x00);
	OPCODE("OR",0x00);
	OPCODE("OUT",0x00);
	OPCODE("POP",0x00);
	OPCODE("PUSH",0x00);
	OPCODE("RET",0x00);
	OPCODE("SHL",0x00);
	OPCODE("SHR",0x00);
	OPCODE("SUB",0x00);
	return ret;



};


void SymTable::repr(){

clog << "p86asm supported opcodes:" << endl;
for (map<std::string, uint8_t>::iterator it=m_opmap.begin(); it != m_opmap.end();it++)
{
clog << "\t opcode: " <<it->first <<" encoding:  " << hex2str(&it->second,1)<<  endl;
}
clog << endl;

}


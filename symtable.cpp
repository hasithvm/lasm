#include "symtable.h"

SymTable::SymTable(): m_opmap()
{
	OPCODE("mov",0x00);
	OPCODE("jmp",0xF1);











};


void SymTable::repr(){

clog << "p86asm supported opcodes:" << endl;
for (map<std::string, uint8_t>::iterator it=m_opmap.begin(); it != m_opmap.end();it++)
{
clog << "\t opcode: " <<it->first <<" encoding:  " << hex2str(&it->second,1)<<  endl;
}
clog << endl;

}


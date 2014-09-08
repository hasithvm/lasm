#include "symtable.h"
#include <iostream>
#include <iomanip>
#include "assert.h"
#include "symtable-generated.hpp"


namespace SymTable{
void repr(){

clog << "p86asm supported opcodes:" << endl;
/*for (SymMap::iterator it=m_opmap.begin(); it != m_opmap.end();it++)
{
	clog << "\t opcode: " <<it->first << endl;
		for (OpVars::iterator it2 = it->second.begin(); it2 != it->second.end();it2++){
			for (OpType::iterator it3 = it2->begin(); it3 != it2->end(); it3++){
				std::string a[4];
				clog << (int)*it3;
				clog << ",";
			}
			clog << endl;
		}

	clog << "\t end" << endl;
	}
*/
}

auto_ptr<OpVars> getVariants(int index){
	auto_ptr<OpVars> pOp(new OpVars());
	pOp->realign(index);
	return pOp;
}

const char * translateIDToName(unsigned int nID){
	return lookup_opcodes[nID];

}

};


OpVars::OpVars() {
	m_index = 0;

}


void OpVars::realign(int index){
	m_index = index;


}

int OpVars::size(){
	return instr[m_index].varCount;
}

auto_ptr<OpType> OpVars::get(int index){
	auto_ptr<OpType> opvar(new OpType());
	opvar->realign((instr[m_index].ptr + index));
	return opvar;
}

OpType::OpType(){

	m_ptr = NULL;

}


int OpType::size(){
	if (m_ptr)
		return m_ptr->szVariant;
	else 
		return -1;
}

void OpType::realign(inst_variant* ptr){

	m_ptr = ptr;

}


unsigned char& OpType::operator[](int index){
	assert(m_ptr);
	return m_ptr->pStart[index];

}

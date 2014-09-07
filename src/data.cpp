#include "data.h"

#ifndef _MSC_VER
#define toupper std::toupper
#else
#define snprintf(a, b, c, ...) _snprintf_s(a, b, b, c, __VA_ARGS__)
#endif

//string lookup table for register types.
const char *regtypeLUT[4] = {"uninitialized","general purpose", "special"};
const char *accessmodeLUT[4] = {"uninitialized", "direct","address","indexed"};

Operand::Operand():m_am(UNINITIALIZED){

}

AccessMode& Operand::getAccessMode(){
	return m_am;
}

void Operand::setAccessMode(AccessMode am){
	m_am = am;
}

Register::RegLookupMap Register::m_regmap(Register::_populate());
//default constructor.

Register::Register(): reg(0xFF),m_regtype(0), m_regname(), m_isIndexBase(false),m_ptrOffset(NULL)
{
}



//extended constructor. Consumes a regname and an accessmode.
Register::Register(char* pRegName, AccessMode accessmode): 
reg(0xFF), m_regtype(0),m_regname(), m_isIndexBase(false),m_ptrOffset(NULL)

{
//create the LUT for regtypes. Really should make this static.
string tmp = std::string((const char *)pRegName,2);
//call the parser function.
reg = Register::parseRegString(tmp);
m_regname = tmp.substr(0,2);
Operand::setAccessMode(accessmode);

}

//sets the register name, ie "AX". 
void Register::setName(std::string& regname){
reg = Register::parseRegString(regname);
}

//returns the binary representation of the register.
uint8_t Register::getBinEncoding(){

return reg;

}

std::string& Register::getRegName(){

	return m_regname;

}
//prints a representation of the register to clog.
ostream& Register::repr(ostream& stream)
{
	stream << Indent << "<Register>" << endl;
	stream << IncreaseIndent;
	stream << Indent << "<address>"<< std::hex << reg << "</address>" << endl;
	stream << Indent << "<name>" << m_regname << "</name>" <<  endl;

	if (m_am == REG_OFFSET)
	{
		stream << Indent << "<offset>" << endl;
		stream << IncreaseIndent;

		Operands* ops =  m_ptrOffset;
		for (unsigned int i =0;i < ops->size();i++)
		{
			/*switch(ops->at(i)->getAccessMode())
			{
				
				stream << *((Register*)ops->at(i));
				break;

				case IMMEDIATE:
				case IMMEDIATE_ADDR:
				stream << *((Immediate*)ops->at(i));
				break;

				case CONST:
				case CONST_ADDR:
				stream << *((Constant*)ops->at(i));
				break;
			}*/
				stream << *(ops->at(i));
		}
		stream << DecreaseIndent;
		stream << Indent << "</offset>" << endl;
	}
	stream << Indent << "<type>" << regtypeLUT[m_regtype] <<  "</type>"<< endl;
	stream << DecreaseIndent;
	stream << Indent << "</Register>" << endl;
	return stream;
}


//returns a binary representation of the register.
uint8_t Register::parseRegString(std::string& str){
//convert string to uppercase.
	strToUpperCase(str);
//if the register is not in memory,i.e invalid combination such as "AI" or "SX"
if (m_regmap.find(str) == m_regmap.end())
{
	cerr << "Error! invalid register identifier specified!" << endl;
	return 0xFF;
}
//set the register type
if ((str.at(1) != 'H') && 
		RANGE(m_regmap[str],0x04, 0x07))
	m_regtype = REG_SP;
else
	m_regtype = REG_GP;

if (str.at(1) == 'H' || (str.at(1) == 'L' ))
	m_aw = AW_8BIT;
else
	m_aw = AW_16BIT;
if (((m_regtype == REG_SP) && ( m_regmap[str] > 0x04)) || (str.compare("BX") == 0))
	m_isIndexBase = true;

return m_regmap[str];
}

Register::RegLookupMap Register::_populate(){

	RegLookupMap ret;
	
	REGMAP("AX",0x00);
	REGMAP("CX",0x01);
	REGMAP("DX",0x02);
	REGMAP("BX",0x03);

	REGMAP("SP",0x04);
	REGMAP("BP",0x05);
	REGMAP("SI",0x06);
	REGMAP("DI",0x07);

	REGMAP("AL",0x00);
	REGMAP("CL",0x01);
	REGMAP("DL",0x02);
	REGMAP("BL",0x03);
	REGMAP("AH",0x04);
	REGMAP("CH",0x05);
	REGMAP("DH",0x06);
	REGMAP("BH",0x07);
	
	
	return ret;
}
bool Register::exists(std::string reg){
	strToUpperCase(reg);
	if (m_regmap.count(reg))
		return true;
	return false;
}

void Register::setOffsetPtr(Operands* ptr){
	m_ptrOffset = ptr;
}

Operands* Register::getOffsetPtr(){
	return m_ptrOffset;
}

AccessWidth& Register::getAccessWidth(){
	return m_aw;
}

bool& Register::isIndexable(){
	return m_isIndexBase;

}
//==============================================

Constant::Constant(char* pName){
	m_name = std::string(pName);
};


ostream& Constant::repr(ostream& stream)
{
	stream << Indent << "<Constant>" << endl;
	stream << IncreaseIndent;
	stream << Indent << "<name>"  << m_name << "</name>" <<  endl;
	stream << DecreaseIndent;
	stream << Indent <<"</Constant>" << endl;
	return stream;
}

std::string& Constant::getName(){
	return m_name;
}










void catOperands(Operands* ptr1, Operands* ptr2)
{
	ptr1->insert(ptr1->end(), ptr2->begin(), ptr2->end());
}



std::string hex2str(uint8_t* bytes, int count)
{
std::string temp;
temp.reserve(count*2);
char* str = (char*)malloc(sizeof(char)*4);
for (int i=(count-1);i >= 0;i--)
{
snprintf(str,3, "%02X", bytes[i]);
temp.append((const char *)str,2);
}
free(str);
	return temp;

}

vector<Operand*>* sortOperands(Operand* op1, Operand* op2){
	
	vector<Operand*>* pList = new vector<Operand*>();
	pList->push_back(op1);
	pList->push_back(op2);
	_helperOperandSort(pList);
	return pList;
}

vector<Operand*>* sortOperands(Operand* op1, Operand* op2, Operand* op3){
	
	vector<Operand*>* pList = new vector<Operand*>();
	pList->push_back(op1);
	pList->push_back(op2);
	pList->push_back(op3);
	_helperOperandSort(pList);
	return pList;
}

bool sortOps(Operand* op1, Operand* op2){
	if (op1->getAccessMode() == REG_DIRECT){
	 if(op2->getAccessMode() == IMMEDIATE)
		return true;
	else if (op2->getAccessMode() == REG_DIRECT){
		return (((Register*) op1)->getBinEncoding() < ((Register*)op2)->getBinEncoding());
	}
	} else if (op1->getAccessMode() == IMMEDIATE) {
		if (op2->getAccessMode() == REG_DIRECT) {
			return false;
		} else if (op2->getAccessMode() == IMMEDIATE) {
			return (((Immediate*)op1)->getBinEncoding() < ((Immediate*)op2)->getBinEncoding());
		}
	}
	return false;
}


std::vector<Operand*>* _helperOperandSort(vector<Operand*>* pList){

	sort(pList->begin(), pList->end(), sortOps);
	return pList;

}



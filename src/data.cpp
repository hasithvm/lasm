#include "data.h"

#ifndef VS2010
#define toupper std::toupper
#else
#define toupper(a) ((a >= 'a' && a <= 'z') ? (a - 'a' + 'A') : a)
#define snprintf(a, b, c, ...) _snprintf_s(a, b, b, c, __VA_ARGS__)
#endif

//string lookup table for register types.
char *regtypeLUT[4] = {"uninitialized","general purpose", "special"};
char *accessmodeLUT[4] = {"uninitialized", "direct","address","indexed"};

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

Register::Register(): reg(0xFF),m_regtype(0), m_regname(),m_ptrOffset(NULL), m_isIndexBase(false)
{
}



//extended constructor. Consumes a regname and an accessmode.
Register::Register(char* pRegName, AccessMode accessmode): 
reg(0xFF), m_regtype(0),m_regname(),m_ptrOffset(NULL), m_isIndexBase(false)

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
void Register::repr(int indentlevel){
	std::string indenter(indentlevel, '\t');
	clog << indenter << "<Register>" << endl;
	clog << indenter << "\t<address>"<< hex2str(&reg,1) << "</address>" << endl;
	clog << indenter << "\t<name>" << m_regname << "</name>" <<  endl;
	//clog << indenter << "\t<accessmode>" << accessmodeLUT[(uint8_t)Operand::getAccessMode()] << "</accessmode>" << endl;
	if (Operand::getAccessMode() == REG_OFFSET)
	{
		clog << indenter << "\t<offset>" << endl;
		Operands* ops =  getOffsetPtr();
		for (int i =0;i < ops->size();i++){
			ops->at(i)->repr(indentlevel + 1);
		}
		clog << indenter << "\t</offset>" << endl;
	}
	clog << indenter << "\t<type>" << regtypeLUT[m_regtype] <<  "</type>"<< endl;
	clog << indenter << "</Register>" << endl;
}


//returns a binary representation of the register.
uint8_t Register::parseRegString(std::string& str){
//convert string to uppercase.
for (int i=0;i<str.size();i++)
		str[i] = toupper(str[i]);
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
if ((m_regtype == REG_SP) &&( m_regmap[str] > 0x04) || (str.compare("BX") == 0))
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


void Constant::repr(int indentlevel){
	std::string indenter(indentlevel, '\t');
	clog << indenter << "<Constant>" << endl;
	clog << indenter << "\t<name>" << m_name << "</name>" <<  endl;
//	clog << indenter << "\t<accessmode>" << accessmodeLUT[((uint8_t)Operand::getAccessMode()) >> 4] << "</accessmode>" << endl;

	clog << indenter << "</Constant>" << endl;
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

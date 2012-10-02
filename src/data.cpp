#include "data.h"
//string lookup table for register types.
char *regtypeLUT[3] = {"uninitialized","general purpose", "special"};
char *accessmodeLUT[3] = {"uninitialized", "direct","address"};
Register::RegLookupMap Register::m_regmap(Register::_populate());
//default constructor.

Register::Register(): reg(0xFF),m_am(UNINITIALIZED),m_regtype(0), m_regname()
{
}



//extended constructor. Consumes a regname and an accessmode.
Register::Register(char* pRegName, AccessMode accessmode): 
reg(0xFF), m_am(accessmode),m_regtype(0),m_regname()
{
//create the LUT for regtypes. Really should make this static.
string tmp = std::string((const char *)pRegName,2);
//call the parser function.
reg = Register::parseRegString(tmp);
m_regname = tmp.substr(0,2);
}

//sets the register name, ie "AX". 
void Register::setName(std::string& regname){
reg = Register::parseRegString(regname);
}

//returns the binary representation of the register.
uint8_t Register::getBinEncoding(){

return reg;

}

//returns the register accessmode.
AccessMode Register::getAccessMode(){
return m_am;
}

//prints a representation of the register to clog.
void Register::repr(){
clog << "<Register>" << endl;
clog <<"\taddress:\t"<< hex2str(&reg,1) << endl;
clog <<"\tname:\t" << m_regname <<  endl;
clog << "\taccessmode:\t" << accessmodeLUT[m_am] << endl;
clog << "\ttype:\t" << regtypeLUT[m_regtype] << endl;
clog << "</Register>" << endl;
}


//returns a binary representation of the register.
uint8_t Register::parseRegString(std::string& str){
//convert string to uppercase.
for (int i=0;i<2;i++)
		str[i] = std::toupper(str[i]);
std::string tmp_regname = str.substr(0,2);
//if the register is not in memory,i.e invalid combination such as "AI" or "SX"
if (m_regmap.find(tmp_regname) == m_regmap.end())
{
	cerr << "Error! invalid register identifier specified!" << endl;
	return 0xFF;
}
//set the register type
if ((str.at(1) != 'H') && 
		RANGE(m_regmap[tmp_regname],0x04, 0x07))
	m_regtype = REG_SP;
else
	m_regtype = REG_GP;
		 
return m_regmap[tmp_regname];
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

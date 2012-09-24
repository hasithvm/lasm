#include "data.h"

char chHexLUT[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

Register::Register(): reg(0),am(UNINITIALIZED),regtype(0), m_regmap(), regname()
{
_populate();
}

Register::Register(char* pRegName, AccessMode	 accessmode): 
reg(0), am(accessmode),regtype(0),m_regmap(),regname()
{
_populate();
string tmp = std::string((const char *)pRegName,2);
reg = Register::parseRegString(tmp);
}

void Register::setName(std::string& regname){
reg = Register::parseRegString(regname);
}
uint8_t Register::getBinEncoding(){
//TODO
}
AccessMode Register::getAccessMode(){
return this,am;
}

void Register::repr(){
clog << "<Register>" << endl;
clog <<"\taddress:\t"<< hex2str(&reg,1) << endl;
clog <<"\tname:\t" << regname <<  endl;
clog << "</Register>" << endl;
}

uint8_t Register::parseRegString(std::string& str){

for (int i=0;i<2;i++)
		str[i] = std::toupper(str[i]);
regname = str.substr(0,2);
reg = m_regmap[regname];
printf("%02x", reg);

}

void Register::_populate(){
REGMAP("AX", 0x00);
REGMAP("CX",0x01);
REGMAP("DX",0x02);
REGMAP("BX", 0x03);

REGMAP("SP", 0x04);
REGMAP("BP",0x05);
REGMAP("SI", 0x06);
REGMAP("DI",0x07);

REGMAP("AL",0x00);
REGMAP("CL",0x01);
REGMAP("DL",0x02);
REGMAP("BL", 0x03);
REGMAP("AH",0x04);
REGMAP("CH",0x05);
REGMAP("DH",0x06);
REGMAP("BH",0x07);
}


std::string hex2str(uint8_t* bytes, int count)
{
std::string temp;
temp.reserve(count*2);
char* str = (char*)malloc(sizeof(char)*3);
for (int i=(count-1);i >= 0;i--)
{
snprintf(str,3, "%02X", bytes[i]);
temp.append((const char *)str,2);
}

	return temp;

}

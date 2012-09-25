#ifndef _DATA_H_
#define _DATA_H_
//defines for assembler information
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#define REGMAP(a,b)	m_regmap[a] = b

using namespace std;
enum AccessMode{
	UNINITIALIZED=0,
	REG_DIRECT=1,			//use register as direct access.
	REG_ADDR=2,				//register contains address of operand.
};
enum RegType{
	REG_GP=1,					//general purpose register
	REG_IP=2,					//special purpose register (SP, BP, SI, BI, IP)
	};
extern char chHexLUT[16];
extern char *accessmodeLUT[3];
std::string hex2str(uint8_t* bytes, int count);
class Register{
		public:
					 Register();
					 Register(char * pRegName, AccessMode accessmode);
					 void setName(std::string& regname);
					 uint8_t getBinEncoding();
					 AccessMode getAccessMode();
					 void repr();
					 
		private:
						//uint8_t mask[4] = {0x00,0x03,0x01,0x02};
						uint8_t reg;
						int m_regtype;
						std::string m_regname;
						AccessMode am;
						uint8_t parseRegString(std::string& str);
						std::map<std::string, uint8_t> m_regmap;
						void _populate();
};

#endif

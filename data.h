#ifndef _DATA_H_
#define _DATA_H_
//defines for assembler information
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#define REGMAP(a,b)	ret[a] = b
#define RANGE(x,a,b)	((a <= x) && (x <= b))
using namespace std;
enum AccessMode{
	UNINITIALIZED=0,
	REG_DIRECT=1,				//use register as direct access.
	REG_ADDR=2,					//register contains address of operand.
	IMMEDIATE=4,				//operand is an immediate
	IMMEDIATE_ADDR=8,		//immediate contains address to actual data.
};

enum RegType{
	REG_GP=1,					//general purpose register
	REG_SP=2,					//special purpose register (SP, BP, SI, BI, IP)
	};

extern char *accessmodeLUT[3];
static 	std::string hex2str(uint8_t* bytes, int count);

class Operand{
	public:
			virtual AccessMode getAccessMode()=0;
 			virtual std::vector<uint8_t> getByteArray() = 0;
			virtual void repr(){};
};

class Immediate : public Operand{
	public:
			Immediate():m_data(){}	;
			Immediate(uint8_t* ptr, int len, AccessMode am);
			AccessMode getAccessMode();
			std::vector<uint8_t> getByteArray();
			void repr();
	private:

			std::vector<uint8_t> m_data;
			uint8_t parse(std::string val);
			
};

class Register : public Operand{
		public:
					Register();
					Register(char * pRegName, AccessMode accessmode);
					void setName(std::string& regname);
					std::vector<uint8_t> getByteArray();
					AccessMode getAccessMode();
					void repr();
					
		private:
						typedef std::map<string, uint8_t>  RegLookupMap;
						uint8_t reg;
						int m_regtype;
						std::string m_regname;
						AccessMode am;
						uint8_t parseRegString(std::string& str);
 						static RegLookupMap m_regmap;
						static RegLookupMap _populate();
};

#endif

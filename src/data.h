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
#include <algorithm>
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

enum ImmediateEncoding{
	UNDEFINED=0,
	BASE_BIN=2,
	BASE_HEX=16,
	BASE_ASC=32,
};
extern char *accessmodeLUT[3];
std::string hex2str(uint8_t* bytes, int count);
char convlower(char in);
class Operand{
	public:
			virtual AccessMode getAccessMode()=0;
			
 			//virtual std::vector<uint8_t> getByteArray() = 0;
			virtual void repr(){};
};

class Immediate : public Operand{
	public:
			Immediate();
			Immediate(std::string a,ImmediateEncoding base, AccessMode am);
			AccessMode getAccessMode();
			vector<uint8_t> getBinEncoding();
			void repr();
	private:
			AccessMode m_am;
			vector<uint8_t> m_data;
			static vector<uint8_t> parse(std::string val, ImmediateEncoding base);
			void init();
			
};

class Register : public Operand{
		public:
					Register();
					Register(char * pRegName, AccessMode accessmode);
					void setName(std::string& regname);
					uint8_t getBinEncoding();
					AccessMode getAccessMode();
					void repr();
					
		private:
						typedef std::map<string, uint8_t>  RegLookupMap;
						uint8_t reg;
						int m_regtype;
						std::string m_regname;
						AccessMode m_am;
						uint8_t parseRegString(std::string& str);
 						static RegLookupMap m_regmap;
						static RegLookupMap _populate();
};
class BasicParameterNode{
		public:
					BasicParameterNode();
					void setType(int type);
					void setPtr(Operand* ptr);
					Operand* getPtr();
		private:
					int type;
					Operand* ptr;
};
#endif

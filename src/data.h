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
#include "common.h"

#define REGMAP(a,b)	ret[a] = b
#define RANGE(x,a,b)	((a <= x) && (x <= b))

using namespace std;

enum AccessWidth : std::uint8_t{
AW_UNSPECIFIED=0xFF,
AW_8BIT=0,
AW_16BIT=1,
};
enum class AccessMode : std::uint8_t{
	UNINITIALIZED=0,
	REG_DIRECT=4,				//use register as direct access.
	REG_ADDR=6,					//register contains address of operand.
	REG_OFFSET=7,				//uses indexed register mode
	IMMEDIATE=8,				//operand is an immediate
	IMMEDIATE_ADDR=9,			//immediate contains address to actual data.
	CONST=16,					//constant to be filled in by the assembler.
	CONST_ADDR=17,				//constant to be filled in by the assembler, addressof.
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
	BASE_DEC=64,
};



extern char *accessmodeLUT[4];
std::string hex2str(uint8_t* bytes, int count);
char convlower(char in);
class Operand{
	public:
			Operand();
			AccessMode getAccessMode();
			void setAccessMode(AccessMode am);
 			//virtual std::vector<uint8_t> getByteArray() = 0;
			virtual void repr(int indentlevel)=0;

			
	private:
			AccessMode m_am;
};
typedef vector<Operand*> Operands;
void catOperands(Operands* ptr1, Operands* ptr2);
class Immediate : public Operand{
	public:
			Immediate();
			Immediate(char* pValue,ImmediateEncoding base, AccessMode am);

			vector<uint8_t>& getBinEncoding();
			void repr(int indentlevel);
			Immediate* clone() const;
			int size();
	private:
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
					static bool exists(std::string reg);
					void repr(int indentlevel);
					Operands* getOffsetPtr();
					void setOffsetPtr(Operands* ptr);
					AccessWidth& getAccessWidth();
					RegType& getRegType();
					bool& isIndexable();
		private:
						typedef std::map<string, uint8_t>  RegLookupMap;
						uint8_t reg;
						bool m_isIndexBase;
						int m_regtype;
						AccessWidth m_aw;
						std::string m_regname;
						Operands* m_ptrOffset;
						uint8_t parseRegString(std::string& str);
 						static RegLookupMap m_regmap;
						static RegLookupMap _populate();
};

class Constant : public Operand{
		public:
					Constant(char*  pName);
					void repr(int indentlevel);
					std::string& getName();

		private:
					std::string m_name;

};		

#endif

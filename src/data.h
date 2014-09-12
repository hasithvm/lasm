#ifndef _DATA_H_
#define _DATA_H_
//defines for assembler information
#include <string>
#ifdef _MSC_VER
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#else
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#endif
#include <iostream>
#include <map>
#include <vector>
#include "common.h"

#define REGMAP(a,b) ret[a] = b
#define RANGE(x,a,b)    ((a <= x) && (x <= b))

using namespace std;

typedef struct 
{
    unsigned char instrID;
    char* pStr;
    int pos;

} parser_opcode;

enum AccessWidth{
AW_UNSPECIFIED=0xFF,
AW_8BIT=0,
AW_16BIT=1,
};
enum AccessMode {
    UNINITIALIZED=0,
    REG_DIRECT=4,               //use register as direct access.
    REG_ADDR=6,                 //register contains address of operand.
    REG_OFFSET=7,               //uses indexed register mode
    IMMEDIATE=8,                //operand is an immediate
    IMMEDIATE_ADDR=9,           //immediate contains address to actual data.
    CONST=16,                   //constant to be filled in by the assembler.
    CONST_ADDR=17,              //constant to be filled in by the assembler, addressof.
};

enum RegType{
    REG_GP=1,                   //general purpose register
    REG_SP=2,                   //special purpose register (SP, BP, SI, BI, IP)
    };

enum ImmediateEncoding{
    UNDEFINED=0,
    BASE_BIN=2,
    BASE_HEX=16,
    BASE_ASC=32,
    BASE_DEC=64,
};



extern const char *accessmodeLUT[4];
std::string hex2str(uint8_t* bytes, int count);
char convlower(char in);
class Operand{
    public:
        Operand();
        AccessMode& getAccessMode();
        void setAccessMode(AccessMode am);
 
        virtual ~Operand() {}
 
        virtual ostream& repr(ostream& out)
        {
            return out << Indent << "<Operand/>" << endl;
        }
 
        friend ostream& operator << (ostream& out, Operand& base) 
        {
            base.repr(out);
            return out;
        }   
            
    protected:
            AccessMode m_am;
};
typedef vector<Operand*> Operands;
void catOperands(Operands* ptr1, Operands* ptr2);
class Immediate : public Operand{
    public:
        Immediate();
        Immediate(char* pValue,ImmediateEncoding base, AccessMode am);

        vector<uint8_t>& getBinEncoding();
        virtual ostream& repr(ostream& stream);
        Immediate* clone() const;
        int size();
        std::string& getSourceRepr();
        uint16_t toWord();
        ~Immediate() {};
    private:
        vector<uint8_t> m_data;
        static vector<uint8_t> parse(std::string& in, ImmediateEncoding base);
        void init();
        std::string m_orig;
            
};

class Register : public Operand{
    public:
        Register();
        Register(char * pRegName, AccessMode accessmode);
        void setName(std::string& regname);
        uint8_t getBinEncoding();
        static bool exists(std::string reg);
        virtual ostream& repr(ostream& stream);
        Operands* getOffsetPtr();
        void setOffsetPtr(Operands* ptr);
        AccessWidth& getAccessWidth();
        RegType& getRegType();
        bool& isIndexable();
        std::string& getRegName();
        ~Register() {};

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
        virtual ostream& repr(ostream& stream);
        std::string& getName();
        ~Constant() {};
    private:
        std::string m_name;

};      

vector<Operand*>* sortOperands(Operand* op1, Operand* op2);
vector<Operand*>* sortOperands(Operand* op1, Operand* op2, Operand* op3);
std::vector<Operand*>* _helperOperandSort(vector<Operand*>* pList);

#endif

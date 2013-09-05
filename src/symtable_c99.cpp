#include "symtable.h"

SymTable::SymMap SymTable::m_opmap(SymTable::generate());

SymTable::SymMap SymTable::generate(){
	SymTable::SymMap ret;
	OpVars tmpVars;
	OpType tmpType;

	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x37);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["aaa"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xD5);
		tmpType.push_back(0x0A);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM);
		tmpType.push_back(0xD5);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["aad"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xD4);
		tmpType.push_back(0x0A);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM);
		tmpType.push_back(0xD4);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["aam"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x3F);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["aas"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x14);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x15);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x10);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x11);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x13);
		tmpType.push_back(REG16);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x02);
		tmpType.push_back(0x80);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x02);
		tmpType.push_back(0x81);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x02);
		tmpType.push_back(0x83);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["adc"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x04);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x05);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x00);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x01);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x03);
		tmpType.push_back(REG16);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0x80);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0x81);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0x83);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["add"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x24);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x25);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x20);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x21);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x23);
		tmpType.push_back(REG16);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0x80);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0x81);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0x83);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["and"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM);
		tmpType.push_back(0xE8);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x02 << 3);
		tmpType.push_back(0xFF);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["call"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x98);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["cbw"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xF8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["clc"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xFC);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["cld"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xFA);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["cli"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xF5);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["cmc"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x3C);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x3D);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x38);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x39);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x3A);
		tmpType.push_back(REG8);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x3B);
		tmpType.push_back(REG16);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0x80);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0x81);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0x83);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["cmp"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xA6);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["cmpsb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xA7);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["cmpsw"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x99);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["cwd"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x27);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["daa"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x2F);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["das"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x01<<3);
		tmpType.push_back(0xFE);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x01<<3);
		tmpType.push_back(0xFF);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["dec"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x06 << 3);
		tmpType.push_back(0xF6);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x06 << 3);
		tmpType.push_back(0xF7);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["div"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xF4);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["hlt"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0xF6);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0xF7);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["idiv"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xE4);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xE5);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xEC);
		tmpType.push_back(REG8 |REG_PRESET | REG_AL);
		tmpType.push_back(REG16 | REG_PRESET | REG_DX);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xED);
		tmpType.push_back(REG16 |REG_PRESET | REG_AX);
		tmpType.push_back(REG16 | REG_PRESET | REG_DX);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["in"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xFE);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xFF);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["inc"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM);
		tmpType.push_back(0xCD);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["int"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xCE);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["into"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xCF);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["iret"] = tmpVars;
	}

	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x77);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["ja"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x73);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jae"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x72);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x76);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jbe"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x72);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jc"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0xE3);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jcxz"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x74);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["je"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7f);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jg"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7d);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jge"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7C);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jl"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7E);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jle"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x76);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jna"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x72);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnae"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x73);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x77);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnbe"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x73);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnc"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x75);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jne"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7E);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jng"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7C);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnge"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7D);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnl"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7F);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnle"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x71);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jno"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7B);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnp"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x79);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jns"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x75);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jnz"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x70);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jo"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7A);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jp"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7A);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jpe"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x7B);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jpo"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x78);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["js"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0x74);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jz"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0xE9);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xFF);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["jmp"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x9F);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["lahf"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0xC5);
		tmpType.push_back(REG16);
		tmpType.push_back(MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["lds"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x8D);
		tmpType.push_back(REG16);
		tmpType.push_back(MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["lea"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0xC4);
		tmpType.push_back(REG16);
		tmpType.push_back(MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["les"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xAC);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["lodsb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xAD);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["lodsw"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0xE2);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["loop"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0xE1);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["loope"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_IS_JMP);
		tmpType.push_back(0xE0);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["loopne"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM | OP_REG_ADD);
		tmpType.push_back(0xB0);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM | OP_REG_ADD);
		tmpType.push_back(0xB8);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x88);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x89);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x8A);
		tmpType.push_back(REG8);
		tmpType.push_back(MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x8B);
		tmpType.push_back(REG8);
		tmpType.push_back(MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xC6);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xC7);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["mov"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xA4);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["movsb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xA5);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["movsw"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xF6);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xF7);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["mul"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0xF6);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0xF7);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["neg"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x90);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["nop"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x02 << 3);
		tmpType.push_back(0xF6);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x02 << 3);
		tmpType.push_back(0xF7);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["not"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x0C);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x0D);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x08);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x09);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x0A);
		tmpType.push_back(REG8);
		tmpType.push_back(REG8|MEM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x0B);
		tmpType.push_back(REG16);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x01 << 3);
		tmpType.push_back(0x80);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x01 << 3);
		tmpType.push_back(0x81);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x01 << 3);
		tmpType.push_back(0x83);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["or"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xEE);
		tmpType.push_back(REG16 |REG_PRESET | REG_DX);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xEF);
		tmpType.push_back(REG16 |REG_PRESET | REG_DX);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xE6);
		tmpType.push_back(IMM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xE7);
		tmpType.push_back(IMM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_AX);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["out"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_REG_ADD);
		tmpType.push_back(0x58);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0x8F);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["pop"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x61);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["popa"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x9D);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["popf"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM | OP_REG_ADD);
		tmpType.push_back(0x50);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_MODRM_EXT);
		tmpType.push_back(0x06 << 3);
		tmpType.push_back(0xFF);
		tmpType.push_back(REG16|MEM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM);
		tmpType.push_back(0x6A);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_ONE_OPERAND | OP_NO_MODRM);
		tmpType.push_back(0x68);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["push"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x60);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["pusha"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x9C);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["pushf"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x02 << 3);
		tmpType.push_back(0xD2);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x02 << 3);
		tmpType.push_back(0xD3);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x02 << 3);
		tmpType.push_back(0xC0);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x02 << 3);
		tmpType.push_back(0xC1);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["rcl"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0xD2);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0xD3);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0xC0);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0xC1);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["rcr"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xC3);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["ret"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xD2);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xD3);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear(); 
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xC0);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xC1);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["rol"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x01 << 3);
		tmpType.push_back(0xD2);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x01 << 3);
		tmpType.push_back(0xD3);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();  
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x01 << 3);
		tmpType.push_back(0xC0);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x01 << 3);
		tmpType.push_back(0xC1);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["ror"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x9E);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["sahf"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xD2);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xD3);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();  
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xC0);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xC1);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["sal"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0xD2);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0xD3);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear(); 
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0xC0);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x07 << 3);
		tmpType.push_back(0xC1);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["sar"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x1C);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x1D);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0x80);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0x81);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x03 << 3);
		tmpType.push_back(0x83);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x18);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x19);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x1A);
		tmpType.push_back(REG8);
		tmpType.push_back(MEM8 | REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x1B);
		tmpType.push_back(REG16);
		tmpType.push_back(MEM16 | REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["sbb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xAE);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["scasb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xAF);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["scasw"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xD2);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xD3);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();  
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xC0);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x04 << 3);
		tmpType.push_back(0xC1);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["shl"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x05 << 3);
		tmpType.push_back(0xD2);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x05 << 3);
		tmpType.push_back(0xD3);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG8 | REG_PRESET | REG_CL);
		tmpVars.push_back(tmpType);
		tmpType.clear(); 
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x05 << 3);
		tmpType.push_back(0xC0);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x05 << 3);
		tmpType.push_back(0xC1);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["shr"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xF9);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["stc"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xFD);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["std"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xFB);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["sti"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xAA);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["stosb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xAB);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["stosw"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x2C);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x2D);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x28);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x29);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x2A);
		tmpType.push_back(REG8);
		tmpType.push_back(MEM8 | REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x2B);
		tmpType.push_back(REG16);
		tmpType.push_back(MEM16 | REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x05 << 3);
		tmpType.push_back(0x80);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x05 << 3);
		tmpType.push_back(0x81);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x05 << 3);
		tmpType.push_back(0x83);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["sub"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xA8);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xA9);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xF6);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x00);
		tmpType.push_back(0xF7);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x85);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x85);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["test"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM | OP_REG_ADD);
		tmpType.push_back(0x90);
		tmpType.push_back(REG16);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x86);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x87);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["xchg"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_NO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0xD7);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["xlatb"] = tmpVars;
	}
	{
		tmpVars.clear();
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x34);
		tmpType.push_back(REG8 | REG_PRESET | REG_AL);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_NO_MODRM);
		tmpType.push_back(0x35);
		tmpType.push_back(REG16 | REG_PRESET | REG_AX);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x30);
		tmpType.push_back(REG8 | MEM8);
		tmpType.push_back(REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x31);
		tmpType.push_back(REG16 | MEM16);
		tmpType.push_back(REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x32);
		tmpType.push_back(REG8);
		tmpType.push_back(MEM8 | REG8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS);
		tmpType.push_back(0x33);
		tmpType.push_back(REG16);
		tmpType.push_back(MEM16 | REG16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x06 << 3);
		tmpType.push_back(0x80);
		tmpType.push_back(REG8|MEM8);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x06 << 3);
		tmpType.push_back(0x81);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM16);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		tmpType.push_back(OP_TWO_OPERANDS | OP_MODRM_EXT);
		tmpType.push_back(0x06 << 3);
		tmpType.push_back(0x83);
		tmpType.push_back(REG16|MEM16);
		tmpType.push_back(IMM8);
		tmpVars.push_back(tmpType);
		tmpType.clear();
		ret["xor"] = tmpVars;
	}
	return ret;
};


void SymTable::repr(){

	//clog << "p86asm supported opcodes:" << endl;
	for (SymMap::iterator it=m_opmap.begin(); it != m_opmap.end();it++)
	{
		//clog << "\t opcode: " <<it->first <<" encoding:  " << hex2str(&it->second,1)<<  endl;
	}
	clog << endl;

}

bool SymTable::exists(std::string opcode){
	//clog << "SymTable: matching " << opcode << endl;
	strToLowerCase(opcode);
	if (m_opmap.count(opcode) > 0){
		//	clog << "SymTable: match found!" << endl;
		return true;
	}
	//clog << "SymTable: not an opcode" << endl;
	return false;
}
OpVars& SymTable::at(std::string search){

	return m_opmap.at(search);
}
SymTable::SymTable(){
}



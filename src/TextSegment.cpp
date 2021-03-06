// ----------------------------------------------------------------------------
/*
    TextSegment.cpp
	Contains the definitions for a segment of executable code generated by an 
	instruction.

	Hasith Vidanamadura, 2013
*/
// ----------------------------------------------------------------------------

#include "TextSegment.hpp"
#include <stdint.h>

TextSegment::TextSegment()
	:m_data(),
	 m_updateAddress(false),
	 m_addr(NULL),
	 m_addrSize(AW_UNSPECIFIED),
	 m_addrStartIndex(0),
	 m_counter(0),
	 m_stringData(),
	 m_labelptr(NULL),
	 m_exprptr(NULL)
{
}

void TextSegment::setUpdateFlag(bool isRel)
{
	m_addrStartIndex = m_data.size();
	m_updateAddress = true;
	m_isRel = isRel;
}

void TextSegment::push_back(uint8_t byte)
{
	m_data.push_back(byte);
}

unsigned int TextSegment::getCounter()
{
	return m_counter;
}

void TextSegment::setCounter(unsigned int c)
{
	m_counter = c;
}

void TextSegment::setConstant(Constant* c)
{
	m_addr = c;
}

Constant* TextSegment::getConstant()
{
	return m_addr;
}

bool TextSegment::getUpdateFlag()
{
	return m_updateAddress;
}

void TextSegment::setAddrSize(AccessWidth aw)
{
	m_addrSize = aw;
}

AccessWidth& TextSegment::getAddrSize()
{
	return m_addrSize;
}

int TextSegment::getAddrStartIndex()
{
	return m_addrStartIndex;
}

uint8_t& TextSegment::operator[](int index)
{
	return m_data[index];
}

int TextSegment::size()
{
	return m_data.size();
}

bool TextSegment::getRelativeAddressFlag()
{
	return m_isRel;
}

void TextSegment::setStringData(std::string a)
{
	m_stringData = a;
}

std::string& TextSegment::getStringData()
{
	return m_stringData;
}

ostream& operator<<(ostream& output, const TextSegment& src)
{
	//note: setw isn't sticky. Reset it on every byte written.

	unsigned long nWidth = output.width();

	output << hex << setfill('0') << uppercase;
	if (nWidth == 0 || src.m_data.size() <= nWidth)
	{
		for (vector<uint8_t>::const_iterator it = src.m_data.begin(); it != src.m_data.end(); it++)
			output << setw(2) << (unsigned short) *it;
	}
	else
	{
		for (unsigned int i = 0;i < (nWidth - 1);i++)
			output << setw(2) << (unsigned short) src.m_data[i];
		output << setw(2) << "..";
	}

	return output;
}


void TextSegment::setLabel(LabelNode* ptr)
{
	m_labelptr = ptr;
}

LabelNode* TextSegment::getLabel()
{
	return m_labelptr;
}

void TextSegment::setSourceNode(BaseExpressionNode* ptr)
{
	m_exprptr = ptr;
}

BaseExpressionNode* TextSegment::getSourceNode()
{
	return m_exprptr;
}
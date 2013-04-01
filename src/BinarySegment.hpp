#ifndef BINARYSEGMENT_H_
#define BINARYSEGMENT_H_
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <vector>

#include "data.h"


class BinarySegment{
	public:		
			BinarySegment();
			void push_back(uint8_t byte);
			void setUpdateFlag(bool isRel);
			void setConstant(Constant* cst);
			void setAddrSize(AccessWidth aw);
			int getAddrStartIndex();
			uint8_t& operator[] (int index);
			int size();
			void setCounter(unsigned int c);
			unsigned int getCounter();
			bool getUpdateFlag();
			Constant* getConstant();
			AccessWidth& getAddrSize();
			bool getRelativeAddressFlag();
			void setStringData(std::string a);
			std::string& getStringData();
	private:
			vector<uint8_t> m_data;
			bool m_updateAddress;
			bool m_isRel;
			Constant* m_addr;
			AccessWidth m_addrSize;
			int m_addrStartIndex;
			int m_counter;
			string m_stringData;

};

#endif

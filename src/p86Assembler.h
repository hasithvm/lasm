#ifndef _P86ASSEMBLER_H_
#define _P86ASSEMBLER_H_
#include "data.h"
#include "Nodes.h"
#include "symtable.h"
#include "common.h"



void decodeOperands(Operands& ops, Register** rs, Immediate** imms, Constant** consts, bool isMem[]);


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

class p86Assembler{
	public:

		p86Assembler();
		void parse(ExpressionList& pExprList);
		unsigned int getStartingAddress();
		vector<BinarySegment*>& getSegments();

	private:
		void _handleOpNode(OpNode* op);
		void _handleControlNode(ControlNode* ctl);
		void _handleLabelNode(LabelNode* ctl);
		void _postpass();

		bool _construct(OpType pattern, Operands& ops);
 		void _addSeg(BinarySegment* binseg);
		SymTable st;
		vector<BinarySegment*> segs;	
		unsigned int counter;
		map<string,unsigned int> LocationMap;
		unsigned int m_codeStart;	
};

#endif

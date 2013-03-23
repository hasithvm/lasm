#ifndef _P86ASSEMBLER_H_
#define _P86ASSEMBLER_H_
#include "data.h"
#include "Nodes.h"
#include "symtable.h"
#include "common.h"


static bool type_match(Operand* reg);
void decodeOperands(Operands& ops, Register** rs, Immediate** imms, Constant** consts, bool isMem[]);


class BinarySegment{
	public:		
			BinarySegment();
			void push_back(uint8_t byte);
			void setAddrFlag();
			void setConstant(Constant* cst);
			void setAddrSize(AccessWidth aw);
			int getAddrStartIndex();
			uint8_t& operator[] (int index);
			int size();
			void setCounter(unsigned int c);
			unsigned int getCounter();
			bool NeedsUpdateAddress();
			Constant* getConstant();
			AccessWidth& getAddrSize();
	private:
			vector<uint8_t> data;
			bool updateAddress;
			Constant* addr;
			AccessWidth addrSize;
			int addrStartIndex;
			int m_counter;

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
		vector<uint8_t>* calculateOperands(Operands operands);
		bool _construct(OpType pattern, Operands& ops);
 		void printSeg(BinarySegment* binseg);
		SymTable st;
		vector<BinarySegment*> segs;	
		unsigned int counter;
		map<string,unsigned int> LocationMap;
		unsigned int m_codeStart;	
};

#endif

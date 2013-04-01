#ifndef _P86ASSEMBLER_H_
#define _P86ASSEMBLER_H_
#include "data.h"
#include "Nodes.h"
#include "symtable.h"
#include "common.h"
#include "BinarySegment.hpp"


void decodeOperands(Operands& ops, Register** rs, Immediate** imms, Constant** consts, bool isMem[]);

static std::string getSourceRepr(OpNode* ptr);


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

		int _construct(OpType pattern,OpNode* op, Operands& ops);
 		void _addSeg(BinarySegment* binseg);
		SymTable st;
		vector<BinarySegment*> segs;	
		unsigned int counter;
		map<string,unsigned int> LocationMap;
		unsigned int m_codeStart;	
};

#endif

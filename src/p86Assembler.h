#ifndef _P86ASSEMBLER_H_
#define _P86ASSEMBLER_H_
#include <stdexcept>      // std::out_of_range
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
		int parse(ExpressionList& pExprList);
		unsigned int getStartingAddress();
		vector<BinarySegment*>& getSegments();

	private:
		int _handleOpNode(OpNode* op);
		int _handleControlNode(ControlNode* ctl);
		int _handleLabelNode(LabelNode* ctl);
		bool _postpass();
		void updateLocationMap(unsigned int startFrom, int increment);
		int _construct(auto_ptr<OpType> pattern,OpNode* op, Operands& ops);
 		void _addSeg(auto_ptr<BinarySegment> binseg);
		SymTable st;
		vector<BinarySegment*> segs;	
		unsigned int counter;
		map<string,unsigned int> LocationMap;
		unsigned int m_codeStart;	
		LabelNode* pLastLabel;
};

#endif

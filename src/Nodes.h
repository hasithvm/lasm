#ifndef NODES_H
#define NODES_H
#include <string>
#include <cstdlib>

#include <cstddef>
#include "data.h"
using namespace std;
enum ExpressionType{
EXPR_OP=1,
EXPR_LABEL=2,
EXPR_CONTROL=4,
EXPR_COMMENT=5,
};


//Base class for the AST Nodes.
class BaseExpressionNode{
		public:
					virtual ExpressionType getType()=0;
					virtual void setContent(std::string a) =0;
					virtual std::string& getContent()=0;
					virtual void repr(int indentlevel)=0;
					BaseExpressionNode* getNextExpr();
					void setNextExpr(BaseExpressionNode* ptr);
		private:
				BaseExpressionNode* m_nextptr;
};


//AST node representing an assembly instruction.


				typedef enum ControlType_t{
				CONTROL_ORG,
				CONTROL_DB,
				CONTROL_EQU,
				CONTROL_DW,
				CONTROL_END
				} ControlNodeType;
class ControlNode : public BaseExpressionNode{
		public:
				ControlNode(char* e, Operand* i);
				ExpressionType getType();
				ControlNodeType getControlType();
				void setContent(std::string a);
				std::string& getContent();
				void setImmediate(Immediate* im);
				Operand* getValue();
				void setKey(char* a);
				std::string& getKey();
				void repr(int indentlevel);
				
		private:
				typedef std::map<std::string, ControlNodeType>  CtrlTypeLookupMap;
				Operand* imm;
				ControlNodeType ctrltype;
				ControlNodeType decodeText(std::string& text);
				std::string label;
				std::string m_key;
				static CtrlTypeLookupMap tlm;
				static CtrlTypeLookupMap _populate();
};
class CommentNode : public BaseExpressionNode{
		public:
				CommentNode(char* pContent);
				ExpressionType getType();
				void setContent(std::string a);
				std::string& getContent();
				void repr(int indentlevel);
		private:
				std::string cmt;	
};	

class LabelNode : public BaseExpressionNode{
		public:
				LabelNode(char* a);
				ExpressionType getType();
				void setContent(std::string a);
				std::string& getContent();
				void repr(int indentlevel);
		private:
				std::string m_label;

};	

class OpNode : public BaseExpressionNode{
		public:
					//constructor
					OpNode(char* pOpName, Operands* op);
					ExpressionType getType();
					void setContent(std::string a);
					std::string& getContent();
					int getOperandCount();
					Operands& getOperands();
					void setExplicitAccessModifier(AccessWidth aw);
					AccessWidth getExplicitAccessModifier();
					void repr(int indentlevel);
					void setLineNumber(int line);
					int getLineNumber();

		private:
					int m_line;					
					Operands ops;
					std::string opstr;
					AccessWidth m_aw;
					CommentNode* m_commentNode;
	
};


typedef std::vector<BaseExpressionNode*> ExpressionList;
#endif

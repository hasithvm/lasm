#ifndef NODES_H
#define NODES_H
#include <string>
#include <cstdlib>
#include <cstddef>
#include "data.h"
#include "symtable.h"

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
					int getLineNumber();
					void setLineNumber(int lineno);
					BaseExpressionNode* getNextExpr();
					void setNextExpr(BaseExpressionNode* ptr);
					virtual ostream& repr(ostream& stream)
					{
						return stream << "<BaseExpressionNode/>";
					}
					virtual ~BaseExpressionNode() {};
					friend ostream& operator<<(ostream& stream, BaseExpressionNode& node);

		private:
				int m_line;
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
				virtual ostream& repr(ostream& stream);				
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

/*
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
*/
class LabelNode : public BaseExpressionNode{
		public:
				LabelNode(char* a);
				ExpressionType getType();
				void setContent(std::string a);
				std::string& getContent();
				virtual ostream& repr(ostream& stream);				
		private:
				std::string m_label;

};	

class OpNode : public BaseExpressionNode{
		public:
					//constructor
					OpNode(unsigned int nOpcodeID, Operands* op);
					ExpressionType getType();
					const char* getContent();
					int getOperandCount();
					Operands& getOperands();
					void setExplicitAccessModifier(AccessWidth aw);
					AccessWidth getExplicitAccessModifier();
					virtual ostream& repr(ostream& stream);				
					std::string getSourceRepr();
					void setID(int ID);
					int getID() {return m_id;}
		private:
					Operands ops;
					std::string opstr;
					AccessWidth m_aw;
					int m_id;
};


typedef std::vector<BaseExpressionNode*> ExpressionList;
#endif

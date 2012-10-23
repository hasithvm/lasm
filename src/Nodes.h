#ifndef NODES_H
#define NODES_H
#include <string>
#include <cstdlib>
#include "data.h"
#include <cstddef>
using namespace std;
enum ExpressionType{
EXPR_OP=1,
EXPR_LABEL=2,
EXPR_CONTROL=4,
EXPR_COMMENT=5,
};

enum AccessWidth : std::uint8_t{
AW_UNSPECIFIED=0,
AW_8BIT=1,
AW_16BIT=2,
};
//Base class for the AST Nodes.
class BaseExpressionNode{
		public:
					virtual ExpressionType getType()=0;
					virtual void setContent(std::string a) =0;
					virtual std::string getContent()=0;
					virtual void repr(int indentlevel)=0;
};
typedef vector<Operand*> Operands;

//AST node representing an assembly instruction.
class OpNode : public BaseExpressionNode{
		public:
					//constructor
					OpNode(std::string opName, Operands* op);
					ExpressionType getType();
					void setContent(std::string a);
					std::string getContent();
					int getOperandCount();
					Operands& getOperands();
					void setExplicitAccessModifier(AccessWidth aw);
					AccessWidth getExplicitAccessModifier();
					void repr(int indentlevel);

		private:
					Operands ops;
					std::string opstr;
					AccessWidth m_aw;	
};

class ControlNode : public BaseExpressionNode{
		public:
				ControlNode(char* e, Immediate* i);
				ExpressionType getType();
				typedef enum ControlType_t{
				CONTROL_ORG,
				CONTROL_DB,
				CONTROL_EQU,
				CONTROL_DW,
				} ControlNodeType;
				ControlNodeType getControlType();
				void setContent(std::string a);
				std::string getContent();
				void setImmediate(Immediate* im);
				void setLabel(std::string lbl);
				Immediate* getImmediate();
				void repr(int indentlevel);
		private:
				Immediate* imm;
				ControlNode::ControlNodeType ctrltype;
				ControlNode::ControlNodeType decodeText(std::string& text);
				std::string label;
};
class CommentNode : public BaseExpressionNode{
		public:
				CommentNode(std::string a);
				ExpressionType getType();
				void setContent(std::string a);
				std::string getContent();
				void repr(int indentlevel);
		private:
				std::string cmt;	
};	

class LabelNode : public BaseExpressionNode{
		public:
				LabelNode(char* a);
				ExpressionType getType();
				void setContent(std::string a);
				std::string getContent();
				void repr(int indentlevel);
		private:
				std::string m_label;

};	
#endif

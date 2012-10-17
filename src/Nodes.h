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

class BaseExpressionNode{
		public:
					virtual ExpressionType getType()=0;
					virtual void setContent(std::string a) =0;
					virtual std::string getContent()=0;
					virtual void repr(int indentlevel)=0;
};
typedef vector<Operand*> Operands;
class OpNode : public BaseExpressionNode{
		public:
					OpNode(std::string opName, Operands* op);
					ExpressionType getType();
					void setContent(std::string a);
					std::string getContent();
					int getOperandCount();
					Operands& getOperands();
					void repr(int indentlevel);

		private:
					Operands ops;
					std::string opstr;
};

class ControlNode : public BaseExpressionNode{
		public:
				ControlNode(std::string controlname, Immediate i);
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
				Immediate* getImmediate();
				void repr(int indentlevel);
		private:
				Immediate* imm;
				ControlNode::ControlNodeType ctrltype;
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

#endif

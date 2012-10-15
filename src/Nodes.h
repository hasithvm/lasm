#ifndef NODES_H
#define NODES_H
#include <string>
#include <cstdlib>
#include "data.h"
#include <cstddef>
using namespace std;
enum ExpressionType{
Opcode=1,
Label=2,
Control=4,
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


#endif

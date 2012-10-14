#ifndef NODES_H
#define NODES_H

#include "data.h"
class BasicParameterNode{
		public:
					BasicParameterNode();
					void setType(int type);
					void setPtr(Operand* ptr);
					Operand* getPtr();
		private:
					int type;
					Operand* ptr;
};




#endif

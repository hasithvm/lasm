#ifndef _LASM_HPP_
#define _LASM_HPP_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstddef>
#include <vector>
#include <iostream>
#include <fstream>

#include "data.h"
#include "symtable.h"
#include "Nodes.h"
#include "common.h"
#include "p86Assembler.h"
#include "preprocessor.h"
#include "VirgoWriter.h"
#include "ListingWriter.hpp"





string strOutputFile;

ExpressionList list;		
using namespace std;

#ifdef VS2010
	extern FILE* yyin;
	extern int yylineno;
#endif

extern "C"
	{
#ifndef VS2010
		extern FILE *yyin;
		extern int yylineno;
#endif
		int yyparse(void);
		void yyerror(const char *str);
		int err_count();	
	}


extern void setExpressionList(ExpressionList* l);
int main(int argc, char **argv);


#endif

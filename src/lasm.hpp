#ifndef _LASM_HPP_
#define _LASM_HPP_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cstddef>
#include <vector>
#include "data.h"
#include "symtable.h"
#include "Nodes.h"
#include "common.h"
#include "p86Assembler.h"
#include <iostream>
#include <fstream>
#include "preprocessor.h"
#include "VirgoWriter.h"
#include "boost/program_options.hpp"



string strOutputFile;

ExpressionList list;		
using namespace std;
namespace opt = boost::program_options;


extern "C"
	{
		extern FILE *yyin;
		extern int yylineno;
		int yyparse(void);
		void yyerror(const char *str);
	}
extern void setExpressionList(ExpressionList* l);
int main(int argc, char **argv);


#endif

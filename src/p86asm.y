%{
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
	ExpressionList list;		
	using namespace std;
    extern "C"
	{
		extern FILE *yyin;
		extern int yylineno;
		int yyparse(void);


		int yylex(void);
		//extern int yydebug;
		int yywrap()
		{
			return 1;
		}

	}    
	void yyerror(const char *str)
	{
		cerr << endl << "parser error: " << yylineno << "\t" << str << endl;
	}
	




	main(int argc, char **argv)
	{
	  streambuf *psbuf, *backup;
	//	yydebug =1;
		ofstream toFile;
		toFile.open("log.txt");
		backup = clog.rdbuf();
		psbuf = toFile.rdbuf();
		clog.rdbuf(psbuf);

		list.reserve(80);
		FILE *myfile = fopen(argv[1], "r");
		yyin = myfile;

	if (!myfile) {
		yyerror("Invalid file specified!");
	yyin = stdin;
	}
		
		do {
		yyparse();
	} while (!feof(yyin));





	preprocess(list);

for (int i = 0; i< list.size();i++)
{
	list[i]->repr(0);
}
clog << "assembly started!" << endl;
	p86Assembler asmgen;
	asmgen.parse(list);


		clog.rdbuf(backup);  
		toFile.close();

	}


%}
%union
{
	char* pStr;
	std::vector<Operand*>* pListOperands;
	BaseExpressionNode* pExpr;
	uint8_t* pAccessWidth;
}
//%debug
%error-verbose
%token COLON  END COMMA WORDPTR BYTEPTR LSQBR RSQBR PLUS

%token <pStr> OPCODE HEX_PRE HEX_SUFF DEC BIN_PRE BIN_SUFF
%token <pStr> CMTSTR LABEL
%token <pStr> LITERAL TEXT
%token <pStr> DIRECTIVE
%glr-parser
%% 		
	statements:	|
				statements statement
				{
					list.push_back($<pExpr>2);
				};

	statement: 	program_expr
				;
				
				
	program_expr:code
				|
				label
				|
				directive
				|
				comment
				;


	comment:	CMTSTR
				{
					BaseExpressionNode* pComment = new CommentNode($<pStr>1);

					$<pExpr>$ = pComment;
				};

	directive:	directive_key DIRECTIVE directive_value
				{
					ControlNode* pControl = new ControlNode($2,$<pListOperands>3->at(0));
					
					free($<pStr>2);
					if ($<pStr>1 != nullptr)
				 	pControl->setKey($<pStr>1);
					$<pExpr>$ = pControl;

					
				}
				;

	directive_key:
				|
				TEXT
				{
					$<pStr>$ = $<pStr>1; 
				}
				;
	directive_value:reg_and_lookup_type
					|
					immediate_type
					;

	code:		 OPCODE modifier params
				{
					
					OpNode* pCode = new OpNode($<pStr>1, $<pListOperands>3);
					if ($<pAccessWidth>2)
					{
						pCode->setExplicitAccessModifier((AccessWidth)*$<pAccessWidth>2);
						free($<pAccessWidth>2);					
					}
						free($<pStr>1);
						$<pExpr>$ = pCode;
						
				}
				;

	modifier:	
				{
					$<pAccessWidth>$ = nullptr;			
				}
				|
				WORDPTR
				{
					uint8_t* p = (uint8_t*)malloc(sizeof(uint8_t));
					*p = (uint8_t)AccessWidth::AW_16BIT;
					$<pAccessWidth>$ = p;
				}
				|
				BYTEPTR
				{
					uint8_t* p = (uint8_t*)malloc(sizeof(uint8_t));
					*p = (uint8_t)AccessWidth::AW_8BIT;
					$<pAccessWidth>$ = p;
					clog << "byte ptr" << endl;
				
				}
				;

	params:		{
					$<pListOperands>$= new Operands();
				}
				|
				param COMMA param
				{
					Operands* p1 =$<pListOperands>1;
					Operands* p2 =$<pListOperands>3;
					if (p2->at(0) && (p2->size() == 1))
						p1->push_back(p2->at(0));
					$<pListOperands>$ = p1;
				}
				|
				param
				{
					$<pListOperands>$ = $<pListOperands>1;
				}
				;

	param:		operand
				{
					$<pListOperands>$  = $<pListOperands>1;
				}
				|	
				LSQBR operand RSQBR
				{
					if ($<pListOperands>2->size() != 0)
					{
					
					Operand* op = $<pListOperands>2->at(0);
					if (op->getAccessMode() == AccessMode::REG_DIRECT)
						op->setAccessMode(AccessMode::REG_ADDR);
					
					if (op->getAccessMode() == AccessMode::IMMEDIATE)
						op->setAccessMode(AccessMode::IMMEDIATE_ADDR);
					
					if (op->getAccessMode() == AccessMode::CONST)
						op->setAccessMode(AccessMode::CONST_ADDR);
					$<pListOperands>$ = $<pListOperands>2;
					}
					
				}
				|
				LSQBR operand PLUS operand RSQBR
				{
					Operand* op1 = $<pListOperands>2->at(0);
					Operand* op2 = $<pListOperands>4->at(0);
					if (op1->getAccessMode() == AccessMode::REG_DIRECT)
						{
							((Register*)op1)->setOffsetPtr($<pListOperands>4);
							$<pListOperands>$ = $<pListOperands>2;
						}
					else if (op2->getAccessMode()== AccessMode::REG_DIRECT)
						{
							((Register*)op2)->setOffsetPtr($<pListOperands>2);
							$<pListOperands>$ = $<pListOperands>4;
						}
					else
						yyerror("indexed mode must have at least one valid register!");
							$<pListOperands>$->at(0)->setAccessMode(AccessMode::REG_OFFSET);
				}
 				|
				LSQBR operand PLUS operand PLUS operand RSQBR
				{
					Operand* op1 = $<pListOperands>2->at(0);
					Operand* op2 = $<pListOperands>4->at(0);
					Operand* op3 = $<pListOperands>6->at(0);
					if (op1->getAccessMode() == AccessMode::REG_DIRECT)
						{
							catOperands($<pListOperands>4,$<pListOperands>6);
							((Register*)op1)->setOffsetPtr($<pListOperands>4);
							
							$<pListOperands>$ = $<pListOperands>2;
						}
					else if (op2->getAccessMode()== AccessMode::REG_DIRECT)
						{
							catOperands($<pListOperands>2,$<pListOperands>6);
							((Register*)op2)->setOffsetPtr($<pListOperands>2);
							$<pListOperands>$ = $<pListOperands>4;
						}
					else if (op3->getAccessMode()== AccessMode::REG_DIRECT)
						{
							catOperands($<pListOperands>2,$<pListOperands>4);
							((Register*)op3)->setOffsetPtr($<pListOperands>6);
							$<pListOperands>$ = $<pListOperands>6;
						}
					else
						yyerror("indexed mode must have at least one valid register!");

				}

				;
		
	operand:	reg_and_lookup_type
				|
				immediate_type
				;


	reg_and_lookup_type:TEXT
						{
							if (Register::exists($<pStr>1)){
							Register *reg = new Register($1, AccessMode::REG_DIRECT);
							free ($<pStr>1);
							Operands* ptr = new std::vector<Operand*>;
							ptr->push_back(reg);
							$<pListOperands>$ = ptr;
							}
							else{
								Constant* cnst = new Constant($1);
								cnst->setAccessMode(AccessMode::CONST);
								free($<pStr>1);
								Operands* ptr = new std::vector<Operand*>;
								ptr->push_back(cnst);
								$<pListOperands>$ = ptr;
								}
						}
						;
						
	immediate_type:hex_type
					|
					binary_type
					|
					decimal_type
					|
					LITERAL
					{
						Immediate *i = new Immediate($<pStr>1,BASE_ASC,AccessMode::IMMEDIATE);
						free($<pStr>1);
						Operands* ptr = new std::vector<Operand*>;
						ptr->push_back(i);
						$<pListOperands>$ = ptr;
					};

	binary_type:BIN_PRE
				{
					Immediate *i = new Immediate($<pStr>1,BASE_BIN,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				BIN_SUFF
				{
					Immediate *i = new Immediate($<pStr>1,BASE_BIN,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				};
				

	hex_type:	HEX_PRE
				{
					Immediate *i = new Immediate($<pStr>1,BASE_HEX,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				HEX_SUFF
				{
					Immediate *i = new Immediate($<pStr>1,BASE_HEX,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				;

	decimal_type:	DEC
					{
					Immediate *i = new Immediate($<pStr>1,BASE_DEC,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;

					};

	label:		LABEL COLON
				{
					LabelNode* pLabel = new LabelNode($<pStr>1);

					free($<pStr>1);
					$<pExpr>$ = pLabel;
				};

%%

%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <cstddef>
	#include <vector>
	#include "data.h"
	#include "symtable.h"
	#include "Nodes.h"
		
	using namespace std;
    extern "C"
	{
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
		cerr << endl << "parser error:" << yylineno << "\t" << str << endl;
	}
	




	main(int argc, char **argv)
	{
	//	yydebug =1;
		yyparse();
	}


%}
%union
{
	char* pStr;
	std::vector<Operand*>* pListOperands;
	vector<BaseExpressionNode*>* pListExpr;
	uint8_t* pAccessWidth;
}
//%debug
%error-verbose
%token COLON  END COMMA WORDPTR BYTEPTR LSQBR RSQBR

%token <pStr> OPCODE
%token <pStr> REG
%token <pStr> HEX_PRE HEX_SUFF DEC BIN_PRE BIN_SUFF
%token <pStr> CMTSTR
%token <pStr> LABEL
%token <pStr> LITERAL
%token <pStr> TEXT
%token <pStr> DIRECTIVE
%glr-parser
%% 		
	statements:
				|
				statements statement;

	statement:  endline
				|
				program_expr
				;
	program_expr:code
				|
				label
				|
				directive
				|
				comment;


	comment:	CMTSTR
				{
					BaseExpressionNode* pComment = new CommentNode(std::string($<pStr>1));
					pComment->repr(1);
					free(pComment);
				};

	directive:	directive_key DIRECTIVE hex_type
				{
				 	printf("%s directive, key=%s\n", $2, $<pStr>1);
					$<pListOperands>3->at(0)->repr(1);
				}
				;

	directive_key:
				|
				TEXT
				{
					$<pStr>$ = $<pStr>1; 
				}
				;

	code:		 OPCODE modifier params
				{
					
					OpNode* pCode = new OpNode(std::string($<pStr>1), $<pListOperands>3);
					if ($<pAccessWidth>2)
					{
						pCode->setExplicitAccessModifier((AccessWidth)*$<pAccessWidth>2);
						free($<pAccessWidth>2);					

					}
						free($<pStr>1);
						pCode->repr(1);
						
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

	params:		param COMMA param
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
					}
					$<pListOperands>$ = $<pListOperands>2;
				}
				;
		
	operand:	{
					$<pListOperands>$= new Operands();
				}
				|
				reg_and_lookup_type
				|

				;


	reg_and_lookup_type:TEXT
						{
							if (Register::exists(std::string($1))){
							printf ("this is a reg\n");
							Register *reg = new Register($1, AccessMode::REG_DIRECT);
							free ($<pStr>1);
							Operands* ptr = new std::vector<Operand*>;
							ptr->push_back(reg);
							$<pListOperands>$ = ptr;
							}
							else{
								Constant* cnst = new Constant($1);
								free($<pStr>1);
								Operands* ptr = new std::vector<Operand*>;
								ptr->push_back(cnst);
								$<pListOperands>$ = ptr;
								}
						}
						|
						immediate_type;

	immediate_type:hex_type
					|
					binary_type
					|
					decimal_type
					|
					LITERAL
					{
						Immediate *i = new Immediate(std::string($1),BASE_ASC,AccessMode::IMMEDIATE);
						free($<pStr>1);
						Operands* ptr = new std::vector<Operand*>;
						ptr->push_back(i);
						$<pListOperands>$ = ptr;
					};

	binary_type:BIN_PRE
				{
					Immediate *i = new Immediate(std::string($1).substr(2,-1),BASE_BIN,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				BIN_SUFF
				{
					Immediate *i = new Immediate(std::string($1),BASE_BIN,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				};
				

	hex_type:	HEX_PRE
				{
					Immediate *i = new Immediate(std::string($1).substr(2,-1),BASE_HEX,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				HEX_SUFF
				{
					Immediate *i = new Immediate(std::string($1),BASE_HEX,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				;

	decimal_type:	DEC
					{
					Immediate *i = new Immediate(std::string($1),BASE_DEC,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;

					};

	label:		LABEL COLON
				{
					LabelNode* pLabel = new LabelNode($<pStr>1);
					pLabel->repr(1);
					vector<BaseExpressionNode*>* pList = new vector<BaseExpressionNode*>();
					pList->push_back(pLabel);
					$<pListExpr>$ = pList;
				}
				;

	endline:	END TEXT
				{
					printf("program end\n");
				};

%%

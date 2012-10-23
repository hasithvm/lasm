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
		int yywrap()
		{
			return 1;
		}

	}    
	void yyerror(const char *str)
	{
		cerr << endl << yylineno << "\tparser error:" << str << endl;
	}
	




	main(int argc, char **argv)
	{
		yyparse();
	}


%}
%union
{
	char* pStr;
	std::vector<Operand*>* pListOperands;
	BaseExpressionNode* expr;
	uint8_t* pAccessWidth;
}

%error-verbose
%token SEMICOLON COLON  END COMMA NEWLN WORDPTR BYTEPTR LSQBR RSQBR

%token <pStr> OPCODE
%token <pStr> REG
%token <pStr> HEX
%token <pStr> BINARY
%token <pStr> CMTSTR
%token <pStr> LABEL
%token <pStr> LITERAL
%token <pStr> TEXT

%% 		
	statements:
				|
				statements statement;

	statement:	endline
				|
				code
				|
				comment
				|
				label;

	comment:	|
				SEMICOLON CMTSTR NEWLN
				{
					BaseExpressionNode* pComment = new CommentNode(std::string($<pStr>2));
					pComment->repr(1);
					
				};

	code:
				OPCODE modifier params comment
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
				
				}
				;

	params:		
				{
					
					$<pListOperands>$ = new Operands();
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
					}
					$<pListOperands>$ = $<pListOperands>2;
				}
				;
		
	operand:	{
					$<pListOperands>$= new Operands();
				}
				|
				REG 
				{
					Register *reg = new Register($1, AccessMode::REG_DIRECT);
					free ($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(reg);
					$<pListOperands>$ = ptr;
					
				}
				|
				HEX
				{
					Immediate *i = new Immediate(std::string($1),BASE_HEX,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
					
				}
				|
				BINARY
				{
					Immediate *i = new Immediate(std::string($1).substr(2,-1),BASE_BIN,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				LITERAL
				{
					Immediate *i = new Immediate(std::string($1),BASE_ASC,AccessMode::IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				;

	label:	
				|
				LABEL COLON
				{
					LabelNode* pLabel = new LabelNode($<pStr>1);
					pLabel->repr(1);
				}
				;

	endline:	END TEXT
				{
					printf("program end\n");
				};

%%

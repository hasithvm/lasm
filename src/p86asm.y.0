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
		std::vector<char> a;
	}


%}
%union
{
	char* pStr;
	std::vector<Operand*>* pListOperands;
	BaseExpressionNode* expr;
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
					printf("<comment:%s>\n",$2);
				};

	code:
				OPCODE modifier params comment
				{
					OpNode* pCode = new OpNode(std::string($<pStr>1), $<pListOperands>3);
					free($<pStr>1);
					pCode->repr(1);
					}
				;

	modifier:
				|
				WORDPTR
				{printf("word access");}
				|
				BYTEPTR
				{printf("byte access");}
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

	param:		
				{
					$<pListOperands>$= new Operands();
				}
				|
				REG 
				{
					Register *reg = new Register($1, REG_DIRECT);
					free ($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(reg);
					$<pListOperands>$ = ptr;
					
				}
				|
				LSQBR REG RSQBR
				{
					Register *reg = new Register($2, REG_ADDR);
					free ($<pStr>2);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(reg);
					$<pListOperands>$ = ptr;
					
				}
				|
				HEX
				{
					Immediate *i = new Immediate(std::string($1),BASE_HEX,IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
					
				}
				|
				LSQBR HEX RSQBR
				{
					Immediate *i = new Immediate(std::string($2),BASE_HEX,IMMEDIATE_ADDR);
					free($<pStr>2);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				BINARY
				{
					Immediate *i = new Immediate(std::string($1).substr(2,-1),BASE_BIN,IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				LITERAL
				{
					Immediate *i = new Immediate(std::string($1),BASE_ASC,IMMEDIATE);
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
					printf("<label:%s>\n",$1);
				}
				;

	endline:	END TEXT
				{
					printf("program end\n");
				};

%%

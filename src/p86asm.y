%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <cstddef>
	#include <vector>
	#include "data.h"
	#include "symtable.h"
	#define YYSTYPE char *
		
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
		cerr << endl << yylineno << "\n\tparser error:\n" << str << endl;
	}
	




	main(int argc, char **argv)
	{
		yyparse();
		std::vector<char> a;
	}


%}

%error-verbose
%token SEMICOLON COLON OPCODE REG HEX BINARY END CMTSTR COMMA NEWLN LABEL LITERAL WORDPTR BYTEPTR TEXT LSQBR RSQBR
%% 		
	statements:
				|
				statements statement;

	statement:
				|
				endline
				|
				code
				|
				comment
				|
				label_line;

	comment:			
				|
				SEMICOLON CMTSTR NEWLN
				{
					printf("<comment:%s>\n",$2);
				};

	code:
				label_line OPCODE modifier params comment
				{
					printf("<%d:instruction:%s>\n",yylineno, $2);
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
					
					//$$ =  new std::vector<Operand*>();
				}
				|
				param COMMA param
				{
				
				}
				|
				param
				;

	param:		REG 
				{
					Register *reg = new Register($1, REG_DIRECT);
					reg->repr();
					delete reg;
				}
				|
				LSQBR REG RSQBR
				{
					Register *reg = new Register($2, REG_ADDR);
					reg->repr();
					delete reg;
				}
				|
				HEX
				{
					Immediate *i = new Immediate(std::string($1),BASE_HEX,IMMEDIATE);
					i->repr();
					delete i;
				}
				|
				BINARY
				{
					Immediate *i = new Immediate(std::string($1).substr(2,-1),BASE_BIN,IMMEDIATE);
					i->repr();
					delete i;
				}
				|
				LITERAL
				
				;

	label_line:	LABEL COLON
				{
					printf("<label:%s>\n",$1);
				}
				;

	endline:
				END TEXT
				{
					printf("program end\n");
				};

%%

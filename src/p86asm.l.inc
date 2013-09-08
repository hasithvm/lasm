%{

#include <stdio.h>
	#include <vector>
	#include "data.h"
	#include "symtable.h"
	#include "Nodes.h"
	SymTable st;
  
extern "C"{
	int yylex(void);

}

parser_opcode p;


#include "p86asm.tab.h"



%}
%s CMT
%s PARAM
%option yylineno
%option case-insensitive
%option nodefault
%%
%{/*
This block takes care of removing comments from the input stream*/
%}
";"						{BEGIN(CMT);}
<CMT>[^\n]+			{/*eat comment text*/;}
<CMT>\n				{BEGIN(INITIAL);}
<CMT><<EOF>>		{BEGIN(INITIAL); yyterminate();}
,																	return COMMA;

":"																	return COLON;			
"word"																return WORDPTR;
"byte"																return BYTEPTR;
[hb]				{/*Eats leftover text*/}
%{/*OPCODES*/%}


([a-z][a-z0-9\_]*)/":"			{yylval.pStr=strdup(yytext);	BEGIN(PARAM);	return LABEL;}	
<PARAM>{
	[a-z][a-z0-9\_]*				{yylval.pStr=strdup(yytext); 	return TEXT;}
	"0x"[a-f0-9]{1,}			yylval.pStr=strdup(yytext);				return HEX_PRE;				
	[0-9][a-f0-9]{1,}/"h"			yylval.pStr=strdup(yytext);				return HEX_SUFF;				
	"0b"[0-1]{1,}				yylval.pStr=strdup(yytext);				return BIN_PRE;
	[0-1]{1,}/"b"				yylval.pStr=strdup(yytext);				return BIN_SUFF;
	"-"*[0-9]{1,}				yylval.pStr=strdup(yytext);				return DEC;
	\"(\\.|[^\\"])*\"			yylval.pStr=strdup(yytext);				return LITERAL;
	\'(\\.|[^\\'])*\'			yylval.pStr=strdup(yytext);				return LITERAL; 
\n							BEGIN(INITIAL);
}
[a-z][a-z0-9\_]+		yylval.pStr=strdup(yytext); 			return DIRECTIVE_KEY;



"."[a-z]{2,}				{
								yylval.pStr=strdup(yytext);	
								BEGIN(PARAM);
								return DIRECTIVE;
							}


\[																	return LSQBR;
\]																	return RSQBR;
\+																	return PLUS;

[ \t\r]+															/*whitespace*/
\n 							{/*ignore*/}
.							printf("unrecognized input character '%s' at line %d\n", yytext, yylineno);
<<EOF>>						yyterminate();
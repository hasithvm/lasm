%{

	#ifdef WIN32
	#define DUP _strdup
	#define fileno _fileno
	#else
	#define DUP strdup
	#endif

	#include <stdio.h>
	#include <vector>
	#include "data.h"
	#include "symtable.h"
	#include "Nodes.h"
	SymTable st;

extern "C"{
	int yylex(void);
}



#include "p86asm.tab.h"



%}
%x CMT
%s PARAM
%option yylineno
%option nodefault
%%
%{/*
This block takes care of removing comments from the input stream*/
%}
";"						{BEGIN(CMT);}
<CMT>[^\n]+			{/*eat comment text*/;}
<CMT>\n				 {BEGIN(INITIAL);}
<CMT><<EOF>>			{BEGIN(INITIAL); yyterminate();}

:																	return COLON;
[A-Za-z][A-Za-z]{1,}	{
								if (st.exists(std::string(yytext)))
									{
									yylval.pStr=DUP(yytext);
									BEGIN(PARAM);
									return OPCODE;
									}
								else
									REJECT;
							}



"word"																return WORDPTR;
"byte"																return BYTEPTR;
"0x"[A-Fa-f0-9]{1,}			yylval.pStr=DUP(yytext);				return HEX_PRE;
[0-9][A-Fa-f0-9]{1,}/[Hh]	yylval.pStr=DUP(yytext);				return HEX_SUFF;
"0b"[0-1]{1,}				yylval.pStr=DUP(yytext);				return BIN_PRE;
[0-1]{1,}/[Bb]				yylval.pStr=DUP(yytext);				return BIN_SUFF;
"-"*[0-9]{1,}				yylval.pStr=DUP(yytext);				return DEC;

([A-Za-z][A-Za-z0-9\_]+)/:	yylval.pStr=DUP(yytext); BEGIN(PARAM);	return LABEL;
\"(\\.|[^\\"])*\"			yylval.pStr=DUP(yytext);				return LITERAL;
\'(\\.|[^\\'])*\'			yylval.pStr=DUP(yytext);				return LITERAL;
,																	return COMMA;
<PARAM>[A-Za-z][A-Za-z0-9\_]+		yylval.pStr=DUP(yytext); 	return TEXT;


[A-Za-z][A-Za-z0-9\_]+		yylval.pStr=DUP(yytext); 			return DIRECTIVE_KEY;


"."[A-Za-z]{2,}				{
								yylval.pStr=DUP(yytext);	
								BEGIN(PARAM);
								return DIRECTIVE;
							}

\[																	return LSQBR;
\]																	return RSQBR;
\+																	return PLUS;

[ \t\r]+															/*whitespace*/
<PARAM>\n					BEGIN(INITIAL);							/*ignore these*/
\n																	/*and this*/
[HhBb]																/*leftover suffixes*/
.																printf("bad input character '%s' at line %d\n", yytext, yylineno);
<<EOF>>						yyterminate();
%%

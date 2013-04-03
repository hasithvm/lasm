%{

#include <stdio.h>
	#include <vector>
	#include "data.h"
	#include "symtable.h"
	#include "Nodes.h"
	SymTable st;
    extern "C"
    {

        int yylex(void);

    }



#include "p86asm.tab.h"



%}
%x CMT
%x ENDCOND
%option yylineno
%option nodefault
%%
;[^\n]*/\n					yylval.pStr=strdup(yytext);yylineno--;	return CMTSTR;		

:																	return COLON;			
[A-Za-z][A-Za-z]{1,}	{
								if (st.exists(std::string(yytext)))
									{
									yylval.pStr=strdup(yytext);
									return OPCODE;
									}
								else
									REJECT;
							}
"word"															return WORDPTR;
"byte"															return BYTEPTR;
"0x"[A-Faf0-9]{1,}			yylval.pStr=strdup(yytext);				return HEX_PRE;				
[0-9][A-Fa-f0-9]{1,}/[Hh]		yylval.pStr=strdup(yytext);			return HEX_SUFF;				
"0b"[0-1]{1,}				yylval.pStr=strdup(yytext);				return BIN_PRE;
[0-1]{1,}/[Bb]				yylval.pStr=strdup(yytext);				return BIN_SUFF;
"-"*[0-9]{1,}					yylval.pStr=strdup(yytext);			return DEC;

([A-Za-z][A-Za-z0-9\_]+)/:	yylval.pStr=strdup(yytext); 			return LABEL;	
\"(\\.|[^\\"])*\"			yylval.pStr=strdup(yytext);				return LITERAL;
\'(\\.|[^\\'])*\'			yylval.pStr=strdup(yytext);				return LITERAL; 
,																	return COMMA;
[A-Za-z][A-Za-z0-9_]+		yylval.pStr=strdup(yytext);				return TEXT;
"."[A-Za-z]{2,}				yylval.pStr=strdup(yytext);				return DIRECTIVE;
\[																	return LSQBR;
\]																	return RSQBR;
\+																	return PLUS;

[ \t\r\n]+						;									/*whitespace*/
[Hh]																/*leftover suffixes*/
.	printf("bad input character '%s' at line %d\n", yytext, yylineno);
%%

%{

#include <stdio.h>
	#include <vector>
	#include "data.h"
	#include "symtable.h"
	#include "Nodes.h"

    extern "C"
    {

        int yylex(void);

    }



#include "p86asm.tab.h"



%}
%x CMT
%x ENDCOND
%option yylineno
%%
;							BEGIN CMT;						return SEMICOLON;		/*printf("semicolon");*/
<CMT>(.*[^\n])\s*			yylval.pStr=strdup(yytext);		return CMTSTR;
<CMT>\n						BEGIN 0;						return NEWLN;
:															return COLON;			/*printf("colon");*/
[A-Za-z][A-Za-z]{2,4}		yylval.pStr=strdup(yytext);		return OPCODE;			/*printf("opcode");*/
"WORD PTR"													return WORDPTR;
"BYTE PTR"													return BYTEPTR;
[A-Da-dsS][xXHhLlIiPp]{1}	yylval.pStr=strdup(yytext);		return REG;				/*printf("reg");*/
"0x"[A-Faf0-9]{1,}			yylval.pStr=strdup(yytext);		return HEX;				/*printf("hex");*/
[A-Fa-f0-9]{1,}/[Hh]		yylval.pStr=strdup(yytext);		return HEX;				/*printf("hex");*/
"0b"[0-1]{1,}				yylval.pStr=strdup(yytext);		return BINARY;			/*printf("binary");*/
[0-1]{1,}/[Bb]
[\t ]*"end"|"END"			BEGIN ENDCOND;					return END;				/*printf("end token");*/
([A-Za-z0-9]+?)/:			yylval.pStr=strdup(yytext); 	return LABEL;			/*printf("text");*/
\"(\\.|[^\\"])*\"			yylval.pStr=strdup(yytext);		return LITERAL;
\'(\\.|[^\\'])*\'			yylval.pStr=strdup(yytext);		return LITERAL; 
,															return COMMA;
<ENDCOND>[A-Za-z0-9]+		BEGIN 0;yylval.pStr=strdup(yytext);		return TEXT;
"."[Aa-Zz]{2,}				yylval.pStr=strdup(yytext);		return DIRECTIVE;															
\[															return LSQBR;
\]															return RSQBR;
[\t \n]+						;													/*ignores whitespace*/
%%

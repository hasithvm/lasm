%{

#include <stdio.h>


    extern "C"
    {

        int yylex(void);

    }

		#define YYSTYPE char *


#include "p86asm.tab.h"



%}
%x CMT
%x ENDCOND
%option yylineno
%%
;														BEGIN CMT; return SEMICOLON;					/*printf("semicolon");*/
<CMT>(.*[^\n])\s*					yylval=strdup(yytext); return CMTSTR;
<CMT>\n											BEGIN 0;	return NEWLN;
:														return COLON;							/*printf("colon");*/					
[A-Za-z][A-Za-z]{2,4}				yylval=strdup(yytext);	return OPCODE;						/*printf("opcode");*/
"WORD PTR"									return WORDPTR;
"BYTE PTR"									return BYTEPTR;
[A-Da-dsS][xXHhLlIiPp]{1}					yylval=strdup(yytext);	return REG;								/*printf("reg");*/
"0x"[A-Faf0-9]{1,}					yylval=strdup(yytext);	return HEX;								/*printf("hex");*/
[A-Fa-f0-9]{1,}H						yylval=strdup(yytext);	return HEX;								/*printf("hex");*/
"0b"[0-1]{1,}								yylval=strdup(yytext);	return BINARY;						/*printf("binary");*/
[\t ]*"end"|"END"									BEGIN ENDCOND;			return END;								/*printf("end token");*/
([A-Za-z0-9]+?)/:						yylval=strdup(yytext); return LABEL;							/*printf("text");*/
\"(\\.|[^\\"])*\"						yylval=strdup(yytext);return LITERAL;
\'(\\.|[^\\'])*\'						yylval=strdup(yytext);return LITERAL; 
\n													;							/*prinf("endl");*/
,													return COMMA;
<ENDCOND>[A-Za-z0-9]+			BEGIN 0; yylval=strdup(yytext); return TEXT;
\[												return LSQBR;
\]												return RSQBR;
[\t ]+												;													/*ignores whitespace*/
%%

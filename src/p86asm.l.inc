%{

    #include <stdio.h>
    #include <vector>
    #include "data.h"
    #include "Nodes.h"
    #ifdef _MSC_VER
        #include <io.h>
    #else
        #include <unistd.h>
    #endif
    extern "C"{
        int yylex(void);

    }
parser_opcode p;
#include "p86asm.tab.hpp"
%}


%x CMT
%s PARAM
%option yylineno
%option case-insensitive
%option nodefault
%option nounistd

NEWLN   \x0d\x0a|\x0a

%%
%{/*
This block takes care of removing comments from the input stream*/
%}
";"                         {BEGIN(CMT);}
<CMT>[^\x0a\x0d]*               {/*eat comment text*/}
<CMT>{NEWLN}+                 {BEGIN(INITIAL);}
<CMT><<EOF>>                {BEGIN(INITIAL); yyterminate();}


,                                                                       return COMMA;

":"                                                                     return COLON;
"word"                                                                  return WORDPTR;
"byte"                                                                  return BYTEPTR;
"ptr"               

[hb]                {/*Eats leftover text*/}


%{/*OPCODES*/%}


([a-z][a-z0-9\_]*)/:            {yylval.pStr=strdup(yytext); 
                                BEGIN(PARAM);                           return LABEL;
                                }
<PARAM>
{
    [a-z][a-z0-9\_]*            { yylval.pStr=strdup(yytext);           return TEXT;        }
    "0x"[a-f0-9]{1,}            { yylval.pStr=strdup(yytext);           return HEX_PRE;     }
    [0-9][a-f0-9]{1,}/"h"       { yylval.pStr=strdup(yytext);           return HEX_SUFF;    }
    "0b"[0-1]{1,}               { yylval.pStr=strdup(yytext);           return BIN_PRE;     }
    [0-1]{1,}/"b"               { yylval.pStr=strdup(yytext);           return BIN_SUFF;    }
    "-"*[0-9]{1,}               { yylval.pStr=strdup(yytext);           return DEC;         }
    \".*\"			           	{ yylval.pStr=strdup(yytext);           return LITERAL;     }
    \'.*\'           			{ yylval.pStr=strdup(yytext);           return LITERAL;     }
{NEWLN}                         BEGIN(INITIAL);
}
[a-z][a-z0-9\_]+                yylval.pStr=strdup(yytext);             return DIRECTIVE_KEY;



"."[a-z]{2,}                    {
                                    yylval.pStr=strdup(yytext); 
                                    BEGIN(PARAM);                       return DIRECTIVE;
                                }


\[                                                                      return LSQBR;
\]                                                                      return RSQBR;
\+                                                                      return PLUS;

{NEWLN}+            /*blank lines*/
[ \t]+          /* more whitespace */
.                   printf("unrecognized input character '%s' at line %d\n", yytext, yylineno);
<<EOF>>             yyterminate();

    %{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <vector>
    #include "data.h"
    #include "symtable.h"
		#define YYSTYPE char *
		
    using namespace std;
    
    
    
    void yyerror(const char *str)
    {
            fprintf(stderr,"error: %s\n",str);
    }
    
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
    						statements statement
    						;
    						
    						
    statement:
    					|
    					endline
    					|
    					code_line
    					|
    					comment_line
    					|
    					label_line
    					;
    comment_line:
    						|
    						SEMICOLON CMTSTR NEWLN
    						{
    							printf("<comment:%s>\n",$2);
    						}					
    						;
		code_line:
							OPCODE modifier params comment_line
							{
							printf("<%d:instruction:%s>\n",yylineno, $1);
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
					|
					param COMMA param
					{
					}
					|
					param
					;
					
		param:REG 
					{
								Register reg($1,REG_DIRECT);
								reg.repr();
					}
					|
					LSQBR REG RSQBR
					{
					printf("address of reg:%s\n", $2);
					}
					|
					HEX
					|
					BINARY
					|
					LITERAL
					;
					
		label_line:
							|
							LABEL COLON
							{
							printf("<label:%s>\n",$1);
							}
							;
		endline:
						END TEXT
						{
						printf("program end\n");
					  }
					  ;
					  
		%%

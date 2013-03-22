/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton interface for Bison GLR parsers in C
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     COLON = 258,
     END = 259,
     COMMA = 260,
     WORDPTR = 261,
     BYTEPTR = 262,
     LSQBR = 263,
     RSQBR = 264,
     PLUS = 265,
     OPCODE = 266,
     HEX_PRE = 267,
     HEX_SUFF = 268,
     DEC = 269,
     BIN_PRE = 270,
     BIN_SUFF = 271,
     CMTSTR = 272,
     LABEL = 273,
     LITERAL = 274,
     TEXT = 275,
     DIRECTIVE = 276
   };
#endif


#ifndef YYSTYPE
typedef union YYSTYPE
{

/* Line 2663 of glr.c  */
#line 87 "p86asm.y"

	char* pStr;
	std::vector<Operand*>* pListOperands;
	BaseExpressionNode* pExpr;
	uint8_t* pAccessWidth;



/* Line 2663 of glr.c  */
#line 79 "p86asm.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{

  char yydummy;

} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



extern YYSTYPE yylval;




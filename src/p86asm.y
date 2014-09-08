%{
	#include <string.h>
	#include <stdlib.h>
	#include <vector>
	#include "data.h"
	#include "Nodes.h"
	ExpressionList* pList;
	using namespace std;

	#ifdef _MSC_VER
	extern FILE* yyin;
	extern int yylineno;
	extern int yynerrs;
	#endif

	extern "C"
	{
	#ifndef _MSC_VER
		extern FILE *yyin;
		extern int yylineno;
		extern int yynerrs;
	#endif
		int yyparse(void);
		void yyerror(const char *str);
		int yylex(void);
		int err_count();
		int yywrap(){


		return 1;
		}
	}

	int err_count(){
		return yynerrs;
	}
	

	void yyerror(const char *str)
	{ 
		cerr << endl << "ERROR: lasm-parser (" << yylineno << ") " << str << endl;
	}

	void setExpressionList(ExpressionList* l){

	 	pList = l;
	}
%}
%union
{
	char* pStr;
	std::vector<Operand*>* pListOperands;
	BaseExpressionNode* pExpr;
	uint8_t* pAccessWidth;
	parser_opcode* pOpcode;
}
%debug
%error-verbose
%token COLON 	":"
%token COMMA 	","
%token WORDPTR 	"word"
%token BYTEPTR  "byte"
%token LSQBR 	"["
%token RSQBR	"]"
%token PLUS		"+"
%token NEWLN	"newline"
%token <pOpcode> OPCODE "opcode"

%token <pStr> HEX_PRE	"prefixed-hex-value"
%token <pStr> HEX_SUFF	"suffixed-hex-value"
%token <pStr> DEC 	"decimal-value"
%token <pStr> BIN_PRE "prefixed-binary-value"
%token <pStr> BIN_SUFF "suffixed-binary-value"
%token <pStr> LITERAL TEXT 
%token <pStr> DIRECTIVE_KEY "directive-key"
%token <pStr> DIRECTIVE "directive"
%token <pStr> LABEL "label"


%% 		
	statements:	|
				statements statement
				{
					if ($<pExpr>2)
						pList->push_back($<pExpr>2);
				};
 
	statement: 	program_expr
				;


	program_expr:code
				|
				label
				|
				directive
				|
				keyword_error
				;


	keyword_error:	OPCODE COLON
				{
					yyerror("Cannot use reserved keyword for label!");
					$<pExpr>$ = NULL;
				};	
						

	directive:	directive_key DIRECTIVE directive_value
				{
					ControlNode* pControl = new ControlNode($2,$<pListOperands>3->at(0));
					
					free($<pStr>2);
					if ($<pStr>1)
					{
						pControl->setKey($<pStr>1);
						free($<pStr>1);		
					}
					free($<pListOperands>3);
					pControl->setLineNumber(yylineno);
					$<pExpr>$ = pControl;
					
				}
				;

	directive_key:
				{
					$<pStr>$ = NULL;
				}
				|
				DIRECTIVE_KEY
				{
					$<pStr>$ = $<pStr>1; 
				}
				;
	directive_value:reg_and_lookup_type
					|
					immediate_type
					;

	code:		 OPCODE modifier params
				{
					
					OpNode* pCode = new OpNode($<pOpcode>1->instrID, $<pListOperands>3);
					if ($<pAccessWidth>2)
					{
						pCode->setExplicitAccessModifier((AccessWidth)*$<pAccessWidth>2);
						free($<pAccessWidth>2);					
					}
						pCode->setLineNumber($<pOpcode>1->pos);
						free($<pOpcode>1);
						$<pExpr>$ = pCode;
						
				}
				;

	modifier:	
				{
					$<pAccessWidth>$ = NULL;			
				}
				|
				WORDPTR
				{
					uint8_t* p = (uint8_t*)malloc(sizeof(uint8_t));
					*p = (uint8_t)AW_16BIT;
					$<pAccessWidth>$ = p;
				}
				|
				BYTEPTR
				{
					uint8_t* p = (uint8_t*)malloc(sizeof(uint8_t));
					*p = (uint8_t)AW_8BIT;
					$<pAccessWidth>$ = p;
					clog << "byte ptr" << endl;
				
				}
				;

	params:		{
					$<pListOperands>$= new Operands();
				}
				|
				param COMMA param
				{
					Operands* p1 =$<pListOperands>1;
					Operands* p2 =$<pListOperands>3;
					if (p2->at(0) && (p2->size() == 1))
						p1->push_back(p2->at(0));
					free(p2);
					$<pListOperands>$ = p1;
				}
				|
				param
				{
					$<pListOperands>$ = $<pListOperands>1;
				}
				;

	param:		operand
				{
					$<pListOperands>$  = $<pListOperands>1;
				}
				|	
				LSQBR operand RSQBR
				{
					if ($<pListOperands>2->size() != 0)
					{
					
					Operand* op = $<pListOperands>2->at(0);
					if (op->getAccessMode() == REG_DIRECT)
						op->setAccessMode(REG_ADDR);
					
					if (op->getAccessMode() == IMMEDIATE)
						op->setAccessMode(IMMEDIATE_ADDR);
					
					if (op->getAccessMode() == CONST)
						op->setAccessMode(CONST_ADDR);
					$<pListOperands>$ = $<pListOperands>2;
					}
					
				}
				|
				LSQBR operand PLUS operand RSQBR
				{
					Operand* op1 = $<pListOperands>2->at(0);
					Operand* op2 = $<pListOperands>4->at(0);

					vector<Operand*>* pList = 	sortOperands(op1, op2);
					vector<Operand*>* pOffsetList = new vector<Operand*>();
					free($<pListOperands>2);
					free($<pListOperands>4);
					if (pList->at(0)->getAccessMode() == REG_DIRECT)
						pList->at(0)->setAccessMode(REG_OFFSET);
					
					pOffsetList->push_back(pList->at(1));
					((Register*) pList->at(0))->setOffsetPtr(pOffsetList);
					pList->pop_back();
					free(pOffsetList);
					$<pListOperands>$ = pList;

				}
 				|
				LSQBR operand PLUS operand PLUS operand RSQBR
				{
					Operand* op1 = $<pListOperands>2->at(0);
					Operand* op2 = $<pListOperands>4->at(0);
					Operand* op3 = $<pListOperands>6->at(0);
					
					vector<Operand*>* pList = 	sortOperands(op1, op2, op3);
					vector<Operand*>* pOffsetList = new vector<Operand*>();
					free($<pListOperands>2);
					free($<pListOperands>4);
					free($<pListOperands>6);

					if (pList->at(0)->getAccessMode() == REG_DIRECT)
						pList->at(0)->setAccessMode(REG_OFFSET);
					
					pOffsetList->push_back(pList->at(1));
					pOffsetList->push_back(pList->at(2));

					
					((Register*) pList->at(0))->setOffsetPtr(pOffsetList);
					pList->pop_back();
					pList->pop_back();

					
					$<pListOperands>$ = pList;


				}

				;
		
	operand:	reg_and_lookup_type
				|
				immediate_type
				;


	reg_and_lookup_type:TEXT
						{
							if (Register::exists($<pStr>1)){
							Register *reg = new Register($1, REG_DIRECT);
							free ($<pStr>1);
							Operands* ptr = new std::vector<Operand*>;
							ptr->push_back(reg);
							$<pListOperands>$ = ptr;
							}
							else{
								Constant* cnst = new Constant($1);
								cnst->setAccessMode(CONST);
								free($<pStr>1);
								Operands* ptr = new std::vector<Operand*>;
								ptr->push_back(cnst);
								$<pListOperands>$ = ptr;
								}
						}
						;
						
	immediate_type:hex_type
					|
					binary_type
					|
					decimal_type
					|
					LITERAL
					{
						Immediate *i = new Immediate($<pStr>1,BASE_ASC,IMMEDIATE);
						free($<pStr>1);
						Operands* ptr = new std::vector<Operand*>;
						ptr->push_back(i);
						$<pListOperands>$ = ptr;
					};

	binary_type:BIN_PRE
				{
					Immediate *i = new Immediate($<pStr>1 + 2,BASE_BIN,IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				BIN_SUFF
				{
					Immediate *i = new Immediate($<pStr>1,BASE_BIN,IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				};
				

	hex_type:	HEX_PRE
				{
					Immediate *i = new Immediate($<pStr>1 + 2,BASE_HEX,IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				|
				HEX_SUFF
				{
					Immediate *i = new Immediate($<pStr>1,BASE_HEX,IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;
				}
				;

	decimal_type:	DEC
					{
					Immediate *i = new Immediate($<pStr>1,BASE_DEC,IMMEDIATE);
					free($<pStr>1);
					Operands* ptr = new std::vector<Operand*>;
					ptr->push_back(i);
					$<pListOperands>$ = ptr;

					};

	label:		LABEL COLON
				{
					LabelNode* pLabel = new LabelNode($<pStr>1);
					free($<pStr>1);
					pLabel->setLineNumber(yylineno);
					$<pExpr>$ = pLabel;
				};

%%

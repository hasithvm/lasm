#include "preprocessor.h"
/**
*Performs simple preprocessing. Currently handles EQU statements
*@param pExprList Reference to list of asssembler expressions.
*@return None
*/



void preprocess(ExpressionList& pExprList){

    vector<ControlNode*> equ_defs;
    vector<Constant**> pConstants;
    for (unsigned int i =0;i < pExprList.size();i++)
    {
        if (pExprList[i]->getType() == EXPR_OP)
        {
            Operands& op = ((OpNode*) pExprList[i])->getOperands();
            for (unsigned int j = 0;j < op.size();j++)
            {
                if (op[j]->getAccessMode() == CONST)
                {
                    pConstants.push_back((Constant**)&op[j]);
                }

                if (op[j]->getAccessMode() == REG_OFFSET)
                {
                    Operands* p = ((Register*)op[j])->getOffsetPtr();
                    for (unsigned int k =0;k < p->size();k++)
                    {
                        if (p->at(k)->getAccessMode() == CONST)
                            pConstants.push_back((Constant**)&p->at(k));
                    }
                }
            }
        }
        else if (pExprList[i]->getType() == EXPR_CONTROL)
        {

            if (((ControlNode*) pExprList[i])->getControlType() == CONTROL_EQU)     
            equ_defs.push_back((ControlNode*) pExprList[i]);

        }
    }

    //replace named EQUs with Immediate values.

    for (unsigned int i=0;i< equ_defs.size();i++)
    {
        std::string& equ_key = equ_defs[i]->getKey();
        if (equ_key.empty())
        {
            cerr << "syntax error: .EQU requires a key (maybe you specified a label instead?) on line " << equ_defs[i]->getLineNumber() << endl;
            continue;
        }

        for (unsigned int j=0;j<pConstants.size();j++)
        {
            std::string& const_name = (*(pConstants[j]))->getName();
            if (equ_key.compare(const_name)==0)
            {
                    if ((*pConstants[j])->getAccessMode() == CONST_ADDR)
                    {
                        Immediate* pNew = ((Immediate*)equ_defs[i]->getValue())->clone();
                        pNew->setAccessMode(IMMEDIATE_ADDR);
                        (*pConstants[j]) = (Constant*)pNew; 
                    }
                    else
                    {
                        (*pConstants[j]) = (Constant*)equ_defs[i]->getValue(); 
                    }
            }
        }
    }

}






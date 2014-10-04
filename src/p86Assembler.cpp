#include "p86Assembler.h"
#include "Logger.hpp"
#define RV_IMMEDIATE_TOO_SMALL 4
#define RV_END_FOUND 5

#define IS_REG_DIRECT(r) (r->getAccessMode()==REG_DIRECT)
#define IS_IMM_DIRECT(r) (r->getAccessMode()==IMMEDIATE)
#define IS_CONST_DIRECT(r) (r->getAccessMode()==CONST)

#define PATTERN_PARAMS pattern[0]
#define VALID_INSTR isValidInstr = true;

#define PATTERN_IS_SET(r) (pattern[0] & r)

#define ASSERT_REGISTER_WIDTH(r,aw) if(r->getAccessWidth() != aw)\
                                        ERROR("16-bit register required!");

#define ASSERT_REGISTER_ISINDEXABLE(r) if (!(r->isIndexable()))\
                                        ERROR("Invalid base register for addressing mode");

#define ERROR(e)	{cerr << "ERROR: " << e << endl;\
					return -1;}
#define ERROR_RESUME(e) {cerr << "ERROR: " << e << endl;}


p86Assembler::p86Assembler(): segs(), counter(0),LocationMap(), m_codeStart(0), pLastLabel(NULL)
{

}

unsigned int p86Assembler::getStartingAddress()
{
    return m_codeStart;
}

vector<TextSegment*>& p86Assembler::getSegments()
{
    return segs;

}
int p86Assembler::parse(ExpressionList& pExprList)
{
// skip to control|label|opcode node.
    BaseExpressionNode* currentExpr;
    int retcode;
    bool entrySet = false;
    int errCount = 0;

    for (unsigned int i = 0; i < pExprList.size(); i++) {

        currentExpr = pExprList[i];
        switch(currentExpr->getType()) {

        case(EXPR_OP):
            retcode = _handleOpNode((OpNode*)currentExpr);
            break;
        case(EXPR_CONTROL):
            retcode = _handleControlNode((ControlNode*)currentExpr);
            break;
        case(EXPR_LABEL):
            retcode = _handleLabelNode((LabelNode*)currentExpr);
        default:
            break;
        }
        if (retcode == RV_END_FOUND)
        {
            entrySet = true;
            break;
        }
        else if (retcode)
           errCount++;

    }

    while (_postpass());
    if (!entrySet)
    {
        ERROR_RESUME("Entry point not set (.end missing?). Defaulting to 0000H");
        m_codeStart = 0;

    }
    return errCount;
}
bool p86Assembler::_postpass()
{

    int loc_target;
    int loc_addr;
    int finalAddress;
    int addrStart;
    bool rerunPass = false;
    for (unsigned int i = 0; i < segs.size(); i++) {
        if (segs[i]->getUpdateFlag()) {
            if (LocationMap.find(segs[i]->getConstant()->getName()) == LocationMap.end()){
                ERROR_RESUME("Undefined label " << segs[i]->getConstant()->getName() << " on line " << segs[i]->getSourceNode()->getLineNumber())
                continue;
            }
            loc_target = LocationMap[segs[i]->getConstant()->getName()];
            loc_addr = segs[i]->getCounter() + segs[i]->size();
            addrStart = segs[i]->getAddrStartIndex();

            if (segs[i]->getRelativeAddressFlag())
                finalAddress = loc_target - loc_addr;
            else
                finalAddress = loc_target;


            switch (segs[i]->getAddrSize()) {


            case (AW_8BIT):

                if (finalAddress < -128 || finalAddress > 127) {
                    Logger::Instance() << "WARNING: Jump offset is longer than 8 bits!" << endl;
                    //set the 16-bit bit field.
                    (*segs[i])[0] |= 0x01;
                    segs[i]->setAddrSize(AW_16BIT);
                    segs[i]->push_back(0);
                    updateLocationMap(loc_addr, 1);
                    rerunPass = true;
                } else
                    (*segs[i])[addrStart] = (int8_t)(finalAddress & 0x00FF);

                break;
            case (AW_16BIT):

                (*segs[i])[addrStart] = (finalAddress & 0x00FF);
                (*segs[i])[addrStart + 1] = (finalAddress & 0xFF00) >> 8;
                break;

            default:
                break;

            }


        }

        Logger::Instance() << "Computed word: " << hex2str(&(*segs[i])[0], segs[i]->size()) << " PC = " << segs[i]->getCounter() << endl;
    }
    return rerunPass;
}

void p86Assembler::updateLocationMap(unsigned int startFrom, int increment)
{
    std::map<std::string, unsigned int>::iterator itr;

    for (itr = LocationMap.begin(); itr != LocationMap.end(); ++itr) {
        if (itr->second > startFrom)
            itr->second = itr->second + increment;

    }

}

int p86Assembler::_handleLabelNode(LabelNode* label)
{
    if (LocationMap.count(label->getContent())) {
        //this label has been predefined
        ERROR("Multiply-defined label " << label->getContent()\
              << "on line" << label->getLineNumber())
    }
    std::string LabelContentCopy = label->getContent();
    LocationMap[LabelContentCopy] = counter;
    pLastLabel = label;
    Logger::Instance() << "Location " << label->getContent() << " maps to counter: " << counter << endl;
    return 0;
}

int p86Assembler::_handleControlNode(ControlNode* ctrl)
{

    int retval = 0;
    auto_ptr<TextSegment> binseg(new TextSegment());
    Immediate* immVal = NULL;
    Constant* constVal = NULL;
    binseg->setSourceNode(ctrl);
    if (ctrl->getValue()->getAccessMode() == IMMEDIATE)
        immVal = (Immediate*) ctrl->getValue();

    else if (ctrl->getValue()->getAccessMode() == CONST)
        constVal = (Constant*) ctrl->getValue();

    switch(ctrl->getControlType()) {


    case (CONTROL_DB):



        if (constVal) {
            ERROR_RESUME("reserving memory bytes requires an immediate operand");
            return -1;
        }
        for (int i = 0; i < immVal->size(); i++)
            binseg->push_back(immVal->getBinEncoding()[i]);
        binseg->setStringData(immVal->getSourceRepr());
        _addSeg(binseg);
        break;
        //TODO: Implement DW
    case (CONTROL_DW):
        if (constVal) {
            binseg->setUpdateFlag(false);
            binseg->setConstant(constVal);
            binseg->setAddrSize(AW_16BIT);
            binseg->push_back(0);
            binseg->push_back(0);
        } else if (immVal) {
            for (int i = 0; i < immVal->size(); i+=2) {
                binseg->push_back(immVal->getBinEncoding()[i]);
                binseg->push_back(immVal->getBinEncoding()[i+1]);
                binseg->setStringData(immVal->getSourceRepr());
            }


        } else
            ERROR("reserving memory requires an immediate or constant operand")

            _addSeg(binseg);
        break;

    case (CONTROL_ORG):
        if (immVal->size() > 2)
            cout << "WARN: ORG argument must be 16-bits wide! This may not be what you intended." << endl;
        counter = immVal->toWord();


        break;

    case (CONTROL_END):
        if (LocationMap.find(constVal->getName()) == LocationMap.end()){
            ERROR_RESUME("Start label " << constVal->getName() << " not found! defaulting to 0000h")
            m_codeStart = 0;
        }
        else{
            m_codeStart = LocationMap[constVal->getName()];

        }
        retval = RV_END_FOUND;

        break;


    default:
        break;
    }
    return retval;
}
int p86Assembler::_handleOpNode(OpNode* op)
{
    int match = 1;
    Operands& operands = op->getOperands();
    auto_ptr<OpVars> opv = SymTable::getVariants(op->getID());
    if (opv->size() == 0) {
        ERROR_RESUME("ERROR: Unimplemented mnemonic " << op->getContent()\
                     << " on line " << op->getLineNumber())
        return -1;
    }

    for (int j = 0; j < opv->size(); j++) {
        uint8_t op_prop = (*opv->get(j))[0];
        if ((op_prop & OP_OPERANDS) == operands.size()) {
            match = _construct(opv->get(j), op, operands);
            if (match == 0) {
                segs[segs.size()-1]->setStringData(getSourceRepr(op));
                break;
            }
            if (match == -1)
                break;

        } else
            match = 1;

    }
    if (match == 1)
        ERROR_RESUME("Incorrect operands for mnemonic " << \
                     op->getContent()  << " on line " << op->getLineNumber())
        else if (match ==-1)
            ERROR_RESUME("Assembly failed on " << op->getContent()\
                         << " at line " << op->getLineNumber())
    return match;
    opv.release();
}

/**
*Constructs an opcode from an optype param and given params
*@param pattern OpType to construct
*@param ops source operands specified in code
*@return None
*/
int p86Assembler::_construct(auto_ptr<OpType> pPattern,OpNode* op, Operands& ops)
{
    Register  *reg[3] = {NULL,NULL,NULL};
    Immediate* imm[3]= {NULL};
    Constant* consts[3] = {NULL};
    bool isMem[3] = {false, false};


    OpType pattern = *pPattern.release();

    int operandCount = pattern[0] & OP_OPERANDS;
    int arg0 = pattern.size() - operandCount;
    int arg1 = pattern.size() - operandCount + 1;
    int opcodeIndex = 1;

    uint8_t modrm = 0;
    //set to FF to catch a misencoded instruction
    uint8_t opcode = 0xFF;

    bool hasDisp = false;
    AccessWidth aw_disp = AW_UNSPECIFIED;
    bool hasModRm = true;
    bool hasImm = false;
    bool zeroDisp = false;
    bool dispIsRel = false;
    bool dispIsImmediate = false;
    bool isValidInstr = false;
    bool copyPattern = false;

    Immediate* immSrc;
    Immediate* dispSrc;
    Constant* constSrc;

    //break out the operands correctly
    decodeOperands(ops, &reg[0],&imm[0],&consts[0],isMem);


    //zip(operands in pattern, args)
    for (int j = 0; j < operandCount; j++) {

        if (reg[j] && IS_REG_DIRECT(reg[j])) {

            //if the pattern says the param isn't a reg, reject.
            if (!IS_SET(pattern[arg0 + j], REG))
                return 1;

            //check if accesswidths match the pattern.
            else if (reg[j]->getAccessWidth() !=  (pattern[arg0 + j] & OPERAND_WIDTH))
                return 1;

            //if there's a preset reg flag on the pattern, check it.
            else if (IS_SET(pattern[arg0 + j], REG_PRESET)) {

                if (reg[j]->getBinEncoding() != pattern[arg0 + j]  >> 5)
                    return 1;
            }
        }


        if (imm[j] && IS_IMM_DIRECT(imm[j]) && !IS_SET(pattern[arg0 + j],IMM))
            return 1;
        if (consts[j] && IS_CONST_DIRECT(consts[j]) && !IS_SET(pattern[arg0 + j],IMM))
            return 1;
        if (isMem[j] && !IS_SET(pattern[arg0 + j], MEM))
            return 1;
    }

    //single byte opcodes
    if (PATTERN_IS_SET(OP_NO_MODRM)  &&  (operandCount == 0)) {
        //don't need operands!
        if (ops.size() != 0)
            ERROR( "Opcode "<< op->getContent() << " does not require operands!")


        copyPattern = true;
        hasImm = false;
        hasModRm = false;
        hasDisp = false;

        isValidInstr = true;
    }

    /*
    *This block takes care of one-operand instructions like CALL/JMP
    *that don't require modRMs. This precludes memory addresses from being used.
    *
    */
    else if (PATTERN_IS_SET(OP_NO_MODRM) && operandCount == 1) {

        hasModRm = false;
        hasDisp = false;
        hasImm = false;
        opcode = pattern[opcodeIndex];

        if (reg[0]) {

            /*
                Note: for the Virgo/Libra architecture, the only two opcodes that
                use one operand implied are PUSH/POP
            */


            if (PATTERN_IS_SET(OP_REG_ADD))
                opcode |= reg[0]->getBinEncoding();

            isValidInstr = true;

        } else if (IS_SET(pattern[arg0],IMM)) {

            if (consts[0]) {
                constSrc = consts[0];
                hasDisp = true;
                aw_disp = (pattern[arg0] & 0x01) ? AW_16BIT : AW_8BIT;
                isValidInstr = true;
                dispIsRel = true;
            }

            //
            else if (imm[0]) {
                immSrc = imm[0];
                hasImm = true;
                isValidInstr = true;
            }
        }




    }

    //switch on the DEST of this opcode
    else if (!isMem[0]) {

        //source isn't a memory location
        if (!isMem[1]) {

            //arguments are inherent to the opcode
            if (PATTERN_IS_SET(OP_NO_MODRM)) {

                opcode = pattern[opcodeIndex];
                hasModRm = false;

                if (PATTERN_IS_SET(OP_REG_ADD))
                    opcode |= reg[0]->getBinEncoding();

                if (reg[0] && reg[1]) {

                    isValidInstr = true;
                }

                else if (imm[1]) {

                    hasImm = true;
                    immSrc = imm[1];
                    isValidInstr = true;

                } else if (consts[1]) {
                    constSrc = consts[1];
                    ASSERT_REGISTER_WIDTH(reg[0], AW_16BIT);
                    hasDisp = true;
                    aw_disp = AW_16BIT;
                    isValidInstr = true;

                }



            } else {
                /*
                    NB: For single reg-operand mnemonics (eg MUL AX), the modrm extension
                    should take care of it automatically.
                */
                if (PATTERN_IS_SET(OP_MODRM_EXT)) {

                    modrm |= pattern[1];
                    opcodeIndex = opcodeIndex + 1;
                }


                opcode = pattern[opcodeIndex];
                modrm |= reg[0]->getBinEncoding();
                modrm |= 0xC0; //MOD field is b11


                //encodes to opcode modrm with one reg -- see MUL.
                if (operandCount == 1)
                    isValidInstr = 1;


                //Reg1<-Reg2
                if ((reg[1])) {

                    modrm |= 0xC0; //MOD field is b11
                    if (!PATTERN_IS_SET(OP_MODRM_EXT))
                        modrm |= reg[1]->getBinEncoding() << 3;
                    isValidInstr = true;

                }

                //REG <--IMM
                else if ((imm[1])) {
                    immSrc = imm[1];
                    hasImm = true;
                    isValidInstr = true;

                }


                //REG <--IMM [CONST]. This is reserved for final pass label updates.
                /*
                Syntax example: mov BX, label, where label is a label pointing to data and
                not a simple constant. Only reserved for 16-bit registers

                */
                else if ((consts[1]) && (pattern[arg1] & IMM)) {

                    ASSERT_REGISTER_WIDTH(reg[0], AW_16BIT);
                    constSrc = consts[1];
                    hasDisp = true;
                    aw_disp = AW_16BIT;
                    isValidInstr = true;

                }
            }

        }


        else {
            //REG <--MEM
            modrm = 0x00; //reset modrm
            hasImm = false;
            hasDisp = false;
            hasModRm = true;
            //special case to handle intel encoding quirk.
            zeroDisp = false;

            if (PATTERN_IS_SET(OP_MODRM_EXT)) {
                modrm |= pattern[1];
                opcodeIndex = opcodeIndex + 1;

            }

            opcode = pattern[opcodeIndex];
            modrm |= reg[0]->getBinEncoding() << 3;

            if (imm[1]) {
                //addressing mode is disp16
                hasDisp = true;
                modrm |= 0x06;
                dispSrc = imm[1];
                dispIsImmediate = true;
                isValidInstr = true;
                aw_disp = AW_16BIT;

            } else if (consts[1]) {
                //addressing mode is disp16
                hasDisp = true;
                modrm |= 0x06;
                isValidInstr = true;
                aw_disp = AW_16BIT;
                constSrc = consts[1];

            } else if (reg[1]) {

                int retcode = _generateIndexedRegisterEncoding(reg[1],&modrm,&aw_disp,
                 &dispIsImmediate, &hasDisp, &zeroDisp,&dispSrc, &constSrc);
                if (retcode)
                    return retcode;
                isValidInstr = true;
            }


        }

    }
    else if (isMem[0]) {
        /*
        Acceptable memory operations:
        R->Mem
        Imm->Mem


        #note: no inherent operations. There is a set of one-memory operand opcodes
        (INV and NEG)? that take in a m8/16 operand.
        */
        

        if (isMem[1]){
            ERROR("Memory to memory operations are not supported!")
        }





        if (PATTERN_IS_SET(OP_MODRM_EXT)) {
            modrm |= pattern[1];
            opcodeIndex = opcodeIndex + 1;
        }
            opcode = pattern[opcodeIndex];

        //if constant and constant refers to memory addr
        if(consts[0]) {
            modrm |= 0x06;
            constSrc = consts[0];
            dispIsRel = false;
            hasDisp = true;
            aw_disp = AW_16BIT;            

            if (reg[1]) {
                modrm |= reg[1]->getBinEncoding() << 3;
                isValidInstr = true;

            } else if (imm[1]) {

                if (op->getExplicitAccessModifier() == AW_UNSPECIFIED) {
                    cerr << "ERROR: Ambiguous operand size for " << op->getContent() << endl;
                    return -1;
                }
                if ((uint8_t)op->getExplicitAccessModifier() != (pattern[arg0] & 0x01))
                    return 1;

                immSrc = imm[1];
                dispIsRel = false;
                isValidInstr = true;
            }

            else if (consts[1]){
                ERROR("Unimplemented mode!")
            }


        }

        else if (reg[0]) {

            hasDisp = false;
            hasImm = false;
            int retcode = _generateIndexedRegisterEncoding(reg[0],&modrm,&aw_disp,
                 &dispIsImmediate, &hasDisp, &zeroDisp,&dispSrc, &constSrc);

            if (retcode != 0)
                    return retcode;
            

            if (imm[1]) {
               int sz = imm[1]->size();
               sz = sz > 2 ? 2 : sz;
               if (sz > (pattern[arg1] & 0x01) + 1)
                return 1;
                immSrc = imm[1];
                hasImm = true;
                isValidInstr = true;


            }

            if (reg[1]){
                modrm |= reg[1]->getBinEncoding() << 3;
                isValidInstr = true;
            }

            if (consts[1])
                ERROR("Currently Unimplemented")
            

        } else if (imm[0]) {
            // Ensure size matches up
            modrm |= 0x06; //set to immediate address mode

            // Since mod field is taken, three possibilites remain
            // 1. reg field used for extended opcode -> second arg immediate
            // 2. reg field used for register -> second arg register
            // 3. only one argument
            hasDisp = true;
            dispSrc = imm[0];
            dispIsImmediate = true;
            dispIsRel = false;
            aw_disp = AW_16BIT;

            if (ops.size() == 1){
                hasImm = false;
                isValidInstr = true;
            }

            if (reg[1]){

                modrm |= reg[1]->getBinEncoding() << 3;
                isValidInstr = true;
            }

            if (imm[1]){
                if (op->getExplicitAccessModifier() == AW_UNSPECIFIED)
                    ERROR("Ambiguous operand size for operation" << op->getContent())
                
                if (op->getExplicitAccessModifier() != (pattern[arg1] & OPERAND_WIDTH))
                    return 1;

                immSrc = imm[1];
                hasImm = true;
                isValidInstr = true;

            }
            if (consts[1])
                ERROR("Currently Unimplemented")
            
        }
    }

    if (isValidInstr) {
       auto_ptr<TextSegment> binseg(new TextSegment());
       binseg->setSourceNode(op);
        if (copyPattern) {

            for (int i=opcodeIndex; i < pattern.size(); i++)
                binseg->push_back(pattern[i]);

        } else {
            binseg->push_back(opcode);
            if (hasModRm)
                binseg->push_back(modrm);
            if (hasDisp) {
                if (zeroDisp){
                    binseg->push_back(0);
                }
                else if (dispIsImmediate) {
                    uint16_t tImmediateVal = dispSrc->toWord();
                    binseg->push_back((uint8_t)tImmediateVal & 0x00FF);
                    if (aw_disp == AW_16BIT)
                        binseg->push_back((uint8_t)((tImmediateVal & 0xFF00) >> 8));


                } else {
                    binseg->setUpdateFlag(dispIsRel);
                    binseg->setConstant(constSrc);
                    binseg->setAddrSize(aw_disp);

                    binseg->push_back(0);
                    if(aw_disp == AW_16BIT)
                        binseg->push_back(0);
                }
            }
            if (hasImm) {
                int len =(pattern[arg1] & OPERAND_WIDTH) + 1;
                //if there is a 2-byte imm required and the immediate isn't long enough
                //or is too long,
                //get the toWord representation, and push it on correctly.
                if (len == 2 && (immSrc->size() != len)) {
                    uint16_t data =immSrc->toWord();
                    binseg->push_back((uint8_t)(data & 0x00FF));
                    binseg->push_back((uint8_t)(data & 0xFF00));

                } else {
                    for (int i = 0; i < len; i++)
                        binseg->push_back(immSrc->getBinEncoding()[i]);
                }
            }
        }
        _addSeg(binseg);
        return 0;
    }

    return 1;

}



void p86Assembler::_addSeg(auto_ptr<TextSegment> binseg)
{
    if (pLastLabel) {
        binseg->setLabel(pLastLabel);
        pLastLabel = NULL;

    }

    binseg->setCounter(counter);

    counter = counter + binseg->size();
    segs.push_back(binseg.release());


}
inline void decodeOperands(Operands& ops, Register** rs, Immediate** imms, Constant** consts, bool isMemory[])
{



    for (unsigned int i = 0 ; i < ops.size(); i++) {
        AccessMode& am = ops[i]->getAccessMode();

        switch (am) {
        case (REG_DIRECT):
        case (REG_ADDR):
        case (REG_OFFSET):
            rs[i] =(Register*) ops[i];
            break;

        case (IMMEDIATE):
        case (IMMEDIATE_ADDR):
            imms[i] = (Immediate*) ops[i];
            break;
        case (CONST):
        case (CONST_ADDR):
            consts[i] =(Constant*) ops[i];
            break;
        case (UNINITIALIZED):
        cerr << "Something happened in " << __FILE__ ":"<< __LINE__ <<  ". Please contact the devs." << endl; 
        break;
        }

        switch (am) {
        case (IMMEDIATE_ADDR):
        case (CONST_ADDR):
        case (REG_ADDR):
        case (REG_OFFSET):
            isMemory[i] = true;
            break;
        case (UNINITIALIZED):
        cerr << "Something happened in " << __FILE__ ":"<< __LINE__ <<  ". Please contact the devs." << endl; 
        break;
        default:
            break;
        }
    }

}


std::string getSourceRepr(OpNode* ptr)
{
    std::string returnStr(ptr->getContent());;

    std::string paramStr;
    Operands& ops = ptr->getOperands();
    Operands* offsets;
    for (unsigned int i = 0; i < ops.size(); i++) {
        paramStr = "";
        switch (ops[i]->getAccessMode()) {
        case (REG_DIRECT):
            paramStr =((Register*) ops[i])->getRegName();
            break;
        case (REG_ADDR):
            paramStr = "[" + ((Register*) ops[i])->getRegName() + "]";
            break;
        case (REG_OFFSET):
            paramStr = "[" + ((Register*) ops[i])->getRegName();
            offsets = ((Register*)ops[i])->getOffsetPtr();
            for (unsigned int k = 0; k < offsets->size(); k++) {
                switch (offsets->at(k)->getAccessMode()) {

                case(REG_DIRECT):
                    paramStr += " + " + ((Register*) offsets->at(k))->getRegName();
                    break;
                case(IMMEDIATE):
                    paramStr += " + " + ((Immediate*) offsets->at(k))->getSourceRepr();
                    break;
                default:
                    break;
                }

            }
            paramStr += "]";
            break;
        case (CONST_ADDR):
            paramStr = "[";
            paramStr+=((Constant*) ops[i])->getName();
            paramStr += "]";
            break;
        case (IMMEDIATE):
            paramStr =((Immediate*) ops[i])->getSourceRepr();
            break;
        case (IMMEDIATE_ADDR):
            paramStr ="[" + ((Immediate*) ops[i])->getSourceRepr() + "]";
            break;
        case (CONST):
            paramStr = ((Constant*) ops[i])->getName();
            break;
        default:
            paramStr = "";
            break;

        }
        returnStr+= " ";
        returnStr+=paramStr;
        if (i < (ops.size() - 1))
            returnStr += ",";
    }
    return returnStr;

}

inline int _generateIndexedRegisterEncoding(Register* regSrc, uint8_t* modrm, AccessWidth* aw_disp,bool* dispIsImmediate, bool* hasDisp, bool* zeroDisp, Immediate** dispSrc, Constant** constSrc){
                ASSERT_REGISTER_ISINDEXABLE(regSrc);
                uint8_t auxModRM = 0;
                Operands* offsets;
                uint8_t binEnc = regSrc->getBinEncoding();
                bool twoOffsetRegs = false;
                Immediate* pDisp;
                switch (regSrc->getBinEncoding()) {
                case (ENC_REG_BP):
                    *hasDisp = true;
                    *zeroDisp = true;
                    auxModRM |= 0x46;
                    break;
                case (ENC_REG_SI):
                    auxModRM |= 0x04;
                    break;
                case (ENC_REG_DI):
                    auxModRM |= 0x05;
                    break;
                case (ENC_REG_BX):
                    auxModRM |= 0x07;
                    break;
                default:
                    ERROR("Undefined register-addressing mode!");
                }


                if (regSrc->getAccessMode() == REG_OFFSET) {

                    //if there's an offset, that means we don't need to escape the special
                    //case for BP.
                    *zeroDisp = false;
                    //clear out the mod bits
                    auxModRM &= 0x3F;
                    
                    offsets = regSrc->getOffsetPtr();
                    twoOffsetRegs = (offsets->size() >= 1) &&\
                                    offsets->at(0)->getAccessMode() == REG_DIRECT;

                    if ((twoOffsetRegs) && (offsets->size() == 2) &&\
                            offsets->at(1)->getAccessMode() != IMMEDIATE) {
                        ERROR("invalid indexed addressing mode!")
                    }

                    if (offsets->back()->getAccessMode() == IMMEDIATE) {

                           
                        *hasDisp = true;
                        pDisp = (Immediate*) offsets->back();
                        *dispIsImmediate = true;
                        auxModRM |= pDisp->size() > 1 ? 0x02 << 6 : 0x01 << 6;
                        *aw_disp = pDisp->size() > 1 ? AW_16BIT : AW_8BIT;
                        *dispSrc = pDisp;
                    }
                    //if a const hasn't been changed by the preprocessor, it has to be 16-bits
                    else if (offsets->back()->getAccessMode() == CONST){

                    	*hasDisp = true;
                    	*dispIsImmediate = false;
                    	auxModRM |= 0x02 << 6;
                    	*aw_disp = AW_16BIT;
                    	*constSrc = (Constant*) offsets->back();

                    }

                }


                if (twoOffsetRegs) {
                    //clear out RM field.
                    auxModRM &= 0xF8;
                    if (binEnc == ENC_REG_BX)
                        auxModRM |= 0x00;
                    else if (binEnc == ENC_REG_BP)
                        auxModRM |= 0x02;
                    else
                        ERROR("Undefined register addressing mode")

                        binEnc = ((Register*)offsets->at(0))->getBinEncoding();
                    if (binEnc == ENC_REG_SI)
                        auxModRM |= 0x00;
                    else if (binEnc == ENC_REG_DI)
                        auxModRM |= 0x01;
                    else
                        ERROR("Undefined register addressing mode")

                }
            
                *modrm |= auxModRM;
                return 0;
}

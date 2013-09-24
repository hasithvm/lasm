#include "p86Assembler.h"


#define IS_REG_DIRECT(r) (r->getAccessMode()==REG_DIRECT)
#define IS_IMM_DIRECT(r) (r->getAccessMode()==IMMEDIATE)
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

vector<BinarySegment*>& p86Assembler::getSegments()
{
    return segs;

}
int p86Assembler::parse(ExpressionList& pExprList)
{
// skip to control|label|opcode node.
    BaseExpressionNode* currentExpr;
    int retcode;
    int errCount = 0;

    for (int i = 0; i < pExprList.size(); i++) {

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
        if (retcode)
            errCount++;

    }

    while (_postpass());
    return errCount;
}
bool p86Assembler::_postpass()
{

    int loc_target;
    int loc_addr;
    int finalAddress;
    int addrStart;
    bool rerunPass = false;
    for (int i = 0; i < segs.size(); i++) {
        if (segs[i]->getUpdateFlag()) {
            try {
                loc_target = LocationMap.at(segs[i]->getConstant()->getName());
            }

            catch (std::out_of_range& e) {
                ERROR_RESUME("ERROR: Undefined label " << segs[i]->getConstant()->getName())
                continue;
            }

            loc_addr = segs[i]->getCounter() + segs[i]->size();
            addrStart = segs[i]->getAddrStartIndex();

            if (segs[i]->getRelativeAddressFlag())
                finalAddress = loc_target - loc_addr;
            else
                finalAddress = loc_target;


            switch (segs[i]->getAddrSize()) {


            case (AW_8BIT):

                if (finalAddress < -128 || finalAddress > 127) {
                    clog << "WARNING: Jump offset is longer than 8 bits!" << endl;
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

        clog << "Computed word: " << hex2str(&(*segs[i])[0], segs[i]->size()) << " PC = " << segs[i]->getCounter() << endl;
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
    clog << "Location " << label->getContent() << " maps to counter: " << counter << endl;
    return 0;
}

int p86Assembler::_handleControlNode(ControlNode* ctrl)
{

    auto_ptr<BinarySegment> binseg = auto_ptr<BinarySegment>(new BinarySegment());
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
        try {
            m_codeStart = LocationMap.at(((Constant*)ctrl->getValue())->getName());
        } catch (std::out_of_range& e) {
            ERROR_RESUME("Start label " << ((Constant*)ctrl->getValue())->getName() << " not found! defaulting to 0000h")
            m_codeStart = 0;
        }
        break;


    default:
        break;
    }
    return 0;
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
        int opcode_offset = 1;
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
}

/**
*Constructs an opcode from an optype param and given params
*@param pattern OpType to construct
*@param ops source operands specified in code
*@return None
*/
int p86Assembler::_construct(auto_ptr<OpType> pPattern,OpNode* op, Operands& ops)
{
    //auto_ptr to clean up if the pointer goes out of scope.
    auto_ptr<BinarySegment> binseg = auto_ptr<BinarySegment>(new BinarySegment());
    binseg->setSourceNode(op);
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

    //temporary variable to store auxillary modRM encoding patterns.
    uint8_t auxModRM = 0;

    bool hasDisp = false;
    AccessWidth aw_disp = AW_UNSPECIFIED;
    bool hasModRm = true;
    bool hasImm = false;
    bool zeroDisp = false;
    bool dispIsRel = false;
    bool dispIsImmediate = false;
    bool isValidInstr = false;
    bool copyPattern = false;
    bool twoOffsetRegs;

    Operands* offsets;


    Immediate* immSrc;
    Immediate* dispSrc;
    Constant* constSrc;

    vector<uint8_t> immediateData;
    unsigned int immSz = 0;

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
                immSrc = imm[9];
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
            modrm = 0x00; //MOD field is b11
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

                ASSERT_REGISTER_ISINDEXABLE(reg[1]);

                switch (reg[1]->getBinEncoding()) {
                case (ENC_REG_BP):
                    hasDisp = true;
                    zeroDisp = true;
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


                if (reg[1]->getAccessMode() == REG_OFFSET) {
                    offsets = reg[1]->getOffsetPtr();

                    twoOffsetRegs = (offsets->size() >= 1) &&\
                                    offsets->at(0)->getAccessMode() == REG_DIRECT;

                    if ((twoOffsetRegs) && (offsets->size() == 2) &&\
                            offsets->at(1)->getAccessMode() != IMMEDIATE) {
                        ERROR("invalid indexed addressing mode!")
                    }

                    if (offsets->back()->getAccessMode() == IMMEDIATE) {
                        hasDisp = true;
                        dispSrc = (Immediate*) offsets->back();
                        dispIsImmediate = true;
                        modrm |= (dispSrc->size() % 3) << 6;
                    }

                }


                if (twoOffsetRegs) {
                    //clear out RM field.
                    modrm &= 0xF8;
                    uint8_t binEnc = reg[1]->getBinEncoding();
                    if (binEnc == ENC_REG_BX)
                        modrm |= 0x00;
                    else if (binEnc == ENC_REG_BP)
                        modrm |= 0x02;
                    else
                        ERROR("Undefined register addressing mode")

                        binEnc = ((Register*)offsets->at(0))->getBinEncoding();
                    if (binEnc == ENC_REG_SI)
                        modrm |= 0x00;
                    else if (binEnc == ENC_REG_DI)
                        modrm |= 0x01;
                    else
                        ERROR("Undefined register addressing mode")

                }
            
                modrm |= auxModRM;
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
        //if constant and constant refers to memory addr

        if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT) {
            modrm |= pattern[1];
            opcodeIndex = opcodeIndex + 1;
        }

        if(consts[0]) {

            if (reg[1] && (pattern[arg1] & REG)) {
                if( (pattern[arg1] & 0x01) != (uint8_t) reg[1]->getAccessWidth())
                    return 1;

                modrm |= reg[1]->getBinEncoding() << 3;
            } else if (imm[1] && (pattern[arg1] & IMM)) {



                if (op->getExplicitAccessModifier() == AW_UNSPECIFIED) {
                    cerr << "ERROR: Ambiguous operand size for " << op->getContent() << endl;
                    return -1;
                }
                if ((uint8_t)op->getExplicitAccessModifier() != (pattern[arg0] & 0x01))
                    return 1;

                immediateData = imm[1]->getBinEncoding();
                immSz = (unsigned int)op->getExplicitAccessModifier() + 1;


            } else if (isMem[1]) {

                cerr << "ERROR: memory-to-memory operations not supported!" << endl;
                return -1;
            }

            modrm |= 0x06;
            binseg->push_back(pattern[opcodeIndex]);
            binseg->push_back(modrm);
            binseg->setUpdateFlag(false);
            binseg->setConstant(consts[0]);


            //addr is 16-bit offset
            binseg->setAddrSize(AW_16BIT);
            binseg->push_back(0);
            binseg->push_back(0);
            if (immSz) {
                //sign-extend the operand
                if (immediateData.size() < immSz) {
                    int16_t data =(int16_t) immediateData[0];
                    binseg->push_back((uint8_t)(data & 0xFF00));
                    binseg->push_back((uint8_t)(data & 0x00FF));
                } else {
                    for (int i = 0; i < immSz; i++)
                        binseg->push_back(immediateData[i]);
                }

            }


            _addSeg(binseg);
            return 0;

        }


        else if (reg[0]) {
            if (imm[1]) {
                if (imm[1]->size() > ((pattern[arg1] & 0x01) + 1))
                    return 1;
                if (!(pattern[arg1] & IMM))
                    return 1;
            }
            if (imm[1] )
                if (reg[1] && (reg[1]->getAccessWidth() != (pattern[arg1] & 0x01)))
                    return 1;

            hasDisp = false;
            hasImm = false;
            bool isWordAccess = false;
            if( reg[0]->getAccessMode() == REG_ADDR) {
                if (reg[0]->isIndexable()) {
                    switch (reg[0]->getBinEncoding()) {
                    case (ENC_REG_BP):
                        hasDisp = true;
                        zeroDisp = true;
                        modrm |= 0x46;
                        break;
                    case (ENC_REG_SI):
                        modrm |= 0x04;
                        break;
                    case (ENC_REG_DI):
                        modrm |= 0x05;
                        break;
                    case (ENC_REG_BX):
                        modrm |= 0x07;
                        break;
                    default:
                        cerr << "ERROR: Undefined register-addressing mode!" << endl;
                        break;
                    }

                } else {
                    cerr << "ERROR: Undefined register-addressing mode!" << endl;
                    return -1;
                }

            }

            else if (reg[0]->getAccessMode() == REG_OFFSET) {
                if (reg[0]->isIndexable()) {
                    Operands* offsets = reg[0]->getOffsetPtr();
                    bool twoOffsetRegs = (offsets->size() >= 1) &&\
                                         offsets->at(0)->getAccessMode() == REG_DIRECT;

                    if ((twoOffsetRegs) & (offsets->size() == 2) &&\
                            offsets->at(1)->getAccessMode() != IMMEDIATE) {
                        ERROR("invalid indexed addressing mode!")
                    }

                    if (offsets->back()->getAccessMode() == IMMEDIATE) {
                        hasDisp = true;
                        imm[0] = (Immediate*) offsets->back();
                        modrm |= (imm[0]->size() % 3) << 6;
                    }

                    switch (reg[0]->getBinEncoding()) {
                    case (ENC_REG_BP):
                        modrm |= 0x46;
                        break;
                    case (ENC_REG_SI):
                        modrm |= 0x04;
                        break;
                    case (ENC_REG_DI):
                        modrm |= 0x05;
                        break;
                    case (ENC_REG_BX):
                        modrm |= 0x07;
                        break;
                    default:
                        cerr << "ERROR: Undefined register-addressing mode!" << endl;
                        return -1;
                        break;

                    }


                    if (twoOffsetRegs) {
                        modrm &= 0xF8;
                        uint8_t binEnc = reg[0]->getBinEncoding();
                        if (binEnc == ENC_REG_BX)
                            modrm |= 0x00;
                        else if (binEnc == ENC_REG_BP)
                            modrm |= 0x02;
                        else
                            ERROR("Undefined register addressing mode")

                            binEnc = ((Register*)offsets->at(0))->getBinEncoding();
                        if (binEnc == ENC_REG_SI)
                            modrm |= 0x00;
                        else if (binEnc == ENC_REG_DI)
                            modrm |= 0x01;
                        else
                            ERROR("Undefined register addressing mode")

                            if (offsets->size() == 2) {

                                //last one has to be an immediate.
                                uint8_t mod = imm[0]->size() % 3;
                                modrm |= mod << 6;
                                hasDisp = true;

                            }
                    }
                    //if (offsets->at(0)->getBinEncoding() == ENC_REG_SI)
                } else {
                    ERROR("Invalid register for addressing")
                }


            }

            else
                cerr << "ERROR: Unimplemented register addressing mode!" << endl;

            if (reg[1] && IS_REG_DIRECT(reg[1]) && (pattern[arg1] & REG)) {
                modrm |= reg[1]->getBinEncoding() <<  3;
                op->setExplicitAccessModifier(reg[1]->getAccessWidth());
            } else if (imm[1] && imm[1]->getAccessMode() == IMMEDIATE)
                hasImm = true;
            else {
                cerr << "ERROR: Unsupported source for operation" << endl;
                return -1;
            }
            binseg->push_back(pattern[opcodeIndex]);
            binseg->push_back(modrm);
            if (hasDisp) {
                if (zeroDisp) {
                    binseg->push_back(0);
                } else {
                    binseg->push_back(imm[0]->getBinEncoding()[0]);
                    if (imm[0]->getBinEncoding().size() > 1)
                        binseg->push_back(imm[0]->getBinEncoding()[1]);
                }
            }
            if (hasImm) {
                if (op->getExplicitAccessModifier() == AW_UNSPECIFIED) {
                    cerr << "ERROR: Ambiguous operand sizes. Use \"byte\" or \"word\" modifier" << endl;
                    return -1;
                }

                int len = ((int) op->getExplicitAccessModifier()) + 1;

                if (imm[1]->size() < len) {
                    binseg->push_back(imm[1]->getBinEncoding()[0]);
                    binseg->push_back(0);
                } else
                    for (int i = 0; i < len; i++)
                        binseg->push_back(imm[1]->getBinEncoding()[i]);


            }

            _addSeg(binseg);
            return 0;
        } else if (imm[0]->getAccessMode() == IMMEDIATE_ADDR) {
            // Ensure size matches up


            modrm |= 0x06; //set to immediate address mode

            // Since mod field is taken, three possibilites remain
            // 1. reg field used for extended opcode -> second arg immediate
            // 2. reg field used for register -> second arg register
            // 3. only one argument

            if (operandCount == 2) {
                // There's a second argument

                if ((pattern[0] & OP_MODRM_EXT) == 0) {
                    // The reg field is free, check if it's necessary
                    // Only direct reg access allowed, because there's a memory access in arg 0
                    if(reg[1] && ((pattern[arg1] & REG) == REG)) {
                        // arg1 is a register
                        // check that it's the right width
                        if (reg[1]->getAccessWidth() == (pattern[arg1] & AW_16BIT))
                            modrm |= reg[1]->getBinEncoding() << 3; // size is right
                        else
                            return 1; // Size is mismatched

                        binseg->push_back(pattern[opcodeIndex]);
                        binseg->push_back(modrm);
                        for (int i = 0; i < imm[0]->size(); i++)
                            binseg->push_back(imm[0]->getBinEncoding()[i]);
                        if (imm[0]->size() < 2)
                            binseg->push_back(0);
                        _addSeg(binseg);
                        return 0;
                    }

                } else if(imm[1] && ((pattern[arg1] & IMMEDIATE) == IMMEDIATE)) {
                    // arg1 is an immediate
                    if(((imm[1]->size() == 2) && ((pattern[arg1] & AW_16BIT) == AW_16BIT)) || ((imm[1]->size() == 1) && ((pattern[arg1] & AW_16BIT) == 0))) {
                        // size matches
                        binseg->push_back(pattern[opcodeIndex]);
                        binseg->push_back(modrm);
                        for (int i = 0; i < imm[0]->size(); i++)
                            binseg->push_back(imm[0]->getBinEncoding()[i]);
                        if (imm[0]->size() < 2)
                            binseg->push_back(0);

                        for (int i = 0; i < imm[1]->size(); i++)
                            binseg->push_back(imm[1]->getBinEncoding()[i]);
                        _addSeg(binseg);
                        return 0;
                    } else {
                        // size mismatch
                        return 1;
                    }
                }
            } else {
                // Only one argument
                binseg->push_back(pattern[opcodeIndex]);
                binseg->push_back(modrm);
                for (int i = 0; i < imm[0]->size(); i++)
                    binseg->push_back(imm[0]->getBinEncoding()[i]);
                if (imm[0]->size() < 2)
                    binseg->push_back(0);
                _addSeg(binseg);
                return 0;

            }

        }
    }

    if (isValidInstr) {
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
                if (dispIsImmediate) {
                    binseg->push_back(dispSrc->getBinEncoding()[0]);
                    if (aw_disp == AW_16BIT)
                        binseg->push_back(dispSrc->getBinEncoding()[1]);


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



void p86Assembler::_addSeg(auto_ptr<BinarySegment> binseg)
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



    for (int i = 0 ; i < ops.size(); i++) {
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

        }
        switch (am) {
        case (IMMEDIATE_ADDR):
        case (CONST_ADDR):
        case (REG_ADDR):
        case (REG_OFFSET):
            isMemory[i] = true;
            break;
        default:
            break;
        }
    }

}


static std::string getSourceRepr(OpNode* ptr)
{
    std::string returnStr(ptr->getContent());;

    std::string paramStr;
    Operands& ops = ptr->getOperands();
    Operands* offsets;
    for (int i = 0; i < ops.size(); i++) {
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
            for (int k = 0; k < offsets->size(); k++) {
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



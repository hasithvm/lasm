#include "p86Assembler.h"




BinarySegment::BinarySegment()
:m_data(),
m_updateAddress(false),
m_addr(NULL),
m_addrSize(AccessWidth::AW_UNSPECIFIED),
m_addrStartIndex(0),
m_counter(0),
m_stringData(){


}

void BinarySegment::setUpdateFlag(bool isRel){

	m_addrStartIndex = m_data.size();
	m_updateAddress = true;
	m_isRel = isRel;

}
void BinarySegment::push_back(uint8_t byte){

	m_data.push_back(byte);
}

unsigned int BinarySegment::getCounter(){
	return m_counter;

}
void BinarySegment::setCounter(unsigned int c){
	m_counter = c;
}

void BinarySegment::setConstant(Constant* c){
	m_addr = c;

}

Constant* BinarySegment::getConstant(){
	return m_addr;

}

bool BinarySegment::getUpdateFlag(){

	return m_updateAddress;
}
void BinarySegment::setAddrSize(AccessWidth aw){

	m_addrSize = aw;
}

AccessWidth& BinarySegment::getAddrSize(){

	return m_addrSize;
}

int BinarySegment::getAddrStartIndex(){
	return m_addrStartIndex;
}
uint8_t& BinarySegment::operator[](int index){
	return m_data[index];
}

int BinarySegment::size(){
	return m_data.size();
}

bool BinarySegment::getRelativeAddressFlag(){
	return m_isRel;

}
void BinarySegment::setStringData(std::string a){
	m_stringData = a;

}

std::string& BinarySegment::getStringData(){
	return m_stringData;
}





















p86Assembler::p86Assembler(): st(), segs(), counter(0),LocationMap(){

}

unsigned int p86Assembler::getStartingAddress(){
	return m_codeStart;
}

vector<BinarySegment*>& p86Assembler::getSegments(){
	return segs;

}
void p86Assembler::parse(ExpressionList& pExprList){
// skip to control|label|opcode node.
	BaseExpressionNode* currentExpr;
	for (int i = 0; i < pExprList.size(); i++){

		currentExpr = pExprList[i];
		switch(currentExpr->getType()){

			case(ExpressionType::EXPR_OP):
			_handleOpNode((OpNode*)currentExpr);
			break;
			case(ExpressionType::EXPR_CONTROL):
			_handleControlNode((ControlNode*)currentExpr);
			break;
			case(ExpressionType::EXPR_LABEL):
			_handleLabelNode((LabelNode*)currentExpr);
			default:
			break;
	}


}
	_postpass();
}
void p86Assembler::_postpass(){


	int loc_target;
	int loc_addr;
	int finalAddress;
	int addrStart;

	for (int i = 0; i < segs.size();i++){
	if (segs[i]->getUpdateFlag()){
		loc_target = LocationMap.at(segs[i]->getConstant()->getName()); 
		loc_addr = segs[i]->getCounter() + segs[i]->size();
		addrStart = segs[i]->getAddrStartIndex();
	
		if (segs[i]->getRelativeAddressFlag())
			finalAddress = loc_target - loc_addr;
		else
			finalAddress = loc_target;
	
		switch (segs[i]->getAddrSize()){
		
			case (AccessWidth::AW_8BIT):		
			if (finalAddress < -128 || finalAddress > 127)
				clog << "ERROR: Jump offset is longer than 8 bits!" << endl;
			else{
				 (*segs[i])[addrStart] = (int8_t)(finalAddress & 0x00FF);
				}
			break;
			case (AccessWidth::AW_16BIT):

				(*segs[i])[addrStart] = (finalAddress & 0x00FF);
				(*segs[i])[addrStart + 1] = (finalAddress & 0xFF00) >> 8;
			break;
			
			default:
			break;	
	
			}
	

		}

	clog << "Computed word: " << hex2str(&(*segs[i])[0], segs[i]->size()) << " PC = " << segs[i]->getCounter() << endl;
	}
}

void p86Assembler::_handleLabelNode(LabelNode* label){
	LocationMap[label->getContent()] = counter;
	clog << "Location " << label->getContent() << " maps to counter: " << counter << endl;
}

void p86Assembler::_handleControlNode(ControlNode* ctrl){
	
	BinarySegment* binseg = new BinarySegment();
	Immediate* imm = (Immediate*) ctrl->getValue();
	switch(ctrl->getControlType()){
	case (ControlNodeType::CONTROL_DB):


		
		binseg->setCounter(counter);
		for (int i = 0;i < imm->size();i++)
			binseg->push_back(imm->getBinEncoding()[i]);
		segs.push_back(binseg);
		counter = counter + binseg->size();
		break;

	case (ControlNodeType::CONTROL_ORG):

		if (imm->size() != 2){
		clog << "WARNING: ORG argument must be 16-bits wide! Skipping" << endl;
		return;		
		}
		counter = (imm->getBinEncoding()[1] << 8) | imm->getBinEncoding()[0];
		break;

	case (ControlNodeType::CONTROL_END):

	m_codeStart = LocationMap.at(((Constant*)ctrl->getValue())->getName());
 
		break;

		
	default:
		break;
	}
}
void p86Assembler::_handleOpNode(OpNode* op){
		bool match = false;
		Operands& operands = op->getOperands();
		OpVars& opv = st.at(op->getContent());
		if (opv.size() == 0){
			clog << "ERROR: Unimplemented mnemonic " << op->getContent()  << " on line " << op->getLineNumber() << endl;
			return;			
			}

		for (int j = 0;j < opv.size();j++)
			{
				uint8_t op_prop = opv[j][0];
				int opcode_offset = 1;
			if ((op_prop & OP_OPERANDS) == operands.size()){
				match = _construct(opv[j], operands);
				if (match){
					segs[segs.size()-1]->setStringData(op->getSourceRepr());
					 break;
				}				
			
			}   
			
			}
		if (!match)
			clog << "ERROR: Incorrect operands for mnemonic " << op->getContent()  << " on line " << op->getLineNumber() << endl;
}

/**
*Constructs an opcode from an optype param and given params
*@param pattern OpType to construct
*@param ops source operands specified in code
*@return None
*/
bool p86Assembler::_construct(OpType pattern, Operands& ops){

BinarySegment* binseg = new BinarySegment();
Register  *reg[3] = {NULL,NULL,NULL};
Immediate* imm[3]= {NULL};
Constant* consts[3] = {NULL};
bool isMem[3] = {false, false};

int operandCount = pattern[0] & OP_OPERANDS;
int arg0 = pattern.size() - operandCount;
int arg1 = pattern.size() - operandCount + 1;
int opcodeIndex = 1;
uint8_t modrm = 0;
//break out the operands correctly
decodeOperands(ops, &reg[0],&imm[0],&consts[0],isMem);
//single byte operands

if ((pattern[0] & OP_NO_MODRM)  &&  ((pattern[0] & OP_OPERANDS) == 0)){
			//don't need operands!
	
			binseg->push_back(pattern[opcodeIndex]);
			_addSeg(binseg);
			return true;
}
if ((pattern[arg0] & IMM) == IMM){

	if (consts[0]){
//used primarily for the CALL/JMP instruction.
	binseg->push_back(pattern[opcodeIndex]);
	binseg->setUpdateFlag(true);
	binseg->setConstant(consts[0]);

	//default to 8-bit offset
	binseg->setAddrSize(AccessWidth::AW_8BIT);
	binseg->push_back(0);

	if (pattern[arg0] & 0x01){
		binseg->setAddrSize(AccessWidth::AW_16BIT);
		binseg->push_back(0);
	}


	_addSeg(binseg);
	return true;
}
}
//switch on the DEST of this opcode
if (!isMem[0]){

		//source isn't a memory location
	if (!isMem[1]){
		//first afg isn't a reg
		if (!reg[0] || ! (reg[0]->getAccessMode() == AccessMode::REG_DIRECT))
			return false;
		//arguments are inherent to the opcode
		if (pattern[0] & OP_NO_MODRM){

			if ((pattern[0] & OP_OPERANDS) == 1){ //there is no arg2 to this operand
			/**
				Note: for the Virgo/Libra architecture, the only two opcodes that 
				use one operand implied are PUSH/POP
			**/
				uint8_t opcode = pattern[opcodeIndex];

				//OR in the register address
				if (pattern[0] & OP_REG_ADD)
					opcode |= reg[0]->getBinEncoding();


				binseg->push_back(opcode);
				_addSeg(binseg);
				return true;
			}



			//without a mod/rm byte, it has to be a REG(preset) <-IMM
			if (reg[0]->getBinEncoding() != (pattern[arg0] >> 5))
				return false;
			

			if (reg[1] && (pattern[arg1] & REG_PRESET) &&
				 (reg[1]->getBinEncoding() == pattern[arg1] >> 5) &&
					(reg[1]->getAccessWidth() == (pattern[arg1] & 0x01)))
			{

			//this is an OUT DX,A[L|X] instruction.
				binseg->push_back(pattern[opcodeIndex]);
				_addSeg(binseg);
				return true;

			}

			//reject if the next arg isn't an immediate
			if (!imm[1] || ((uint8_t)reg[0]->getAccessWidth() != (pattern[arg0] & 0x01)))
				return false;

			//this is now not a REG<-mem operation (no mod/rm), it has to be a REG<-imm
						binseg->push_back(pattern[opcodeIndex]);
						vector<uint8_t>& imm_data = imm[1]->getBinEncoding();

						int len = (pattern[arg0] & 0x01 ) + 1;


						for (int i = 0; i < len;i++){		
							binseg->push_back(imm_data[imm_data.size() - i - 1]);
						}
						_addSeg(binseg);
						return true;
		
			

		}
		else{
			//single operand opcode that requires a mod/rm byte
			if (!reg[1] && !imm[1] && (arg1 == pattern.size()) && !isMem[0])
			{

				if ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth()){	
					clog << "ERROR: Register widths do not match!" << endl;
					return false;
					}			

				modrm = 0xC0; //MOD field is b11
				if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT)
				{
					modrm |= pattern[1];
					opcodeIndex = opcodeIndex + 1;
				}
				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);

				binseg->push_back(modrm);
				_addSeg(binseg);
				return true;
	


			}

			//Reg1<-Reg2
			if ((reg[1]) && ((pattern[arg1] & REG) == REG)){


				if ((reg[1]->getAccessWidth() != reg[0]->getAccessWidth()) ||
				 ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth())){	
					clog << "ERROR: Register widths do not match!" << endl;
					return false;
					}			

				modrm = 0xC0; //MOD field is b11
				modrm |= (reg[1]->getBinEncoding() << 3) |  (reg[0]->getBinEncoding());
				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				_addSeg(binseg);
				return true;	

			}
			
			//REG <--IMM
			if ((imm[1]) && ((pattern[arg1] & IMM) == IMM)){
				if ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth()){

					return false;
					}			

				modrm = 0xC0; //MOD field is b11
				if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT)
				{
					modrm |= pattern[1];
					opcodeIndex = opcodeIndex + 1;
				}
				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				vector<uint8_t>& imm_data = imm[1]->getBinEncoding();
				
				int len =reg[0]->getAccessWidth()+ 1;
				if (imm_data.size() < len){
					int16_t data =(int16_t) imm_data[0];
					binseg->push_back((uint8_t)(data & 0xFF00));	
					binseg->push_back((uint8_t)(data & 0x00FF));	
				}
				else {
					for (int i = 0; i < len;i++)		
						binseg->push_back(imm_data[i]);
				}
				_addSeg(binseg);
				return true;


			}
			//REG <--IMM [CONST]. This is reserved for final pass label updates.
			/*
			Syntax example: mov BX, label, where label is a label pointing to data and
			not a simple constant. Only reserved for 16-bit registers
	
			*/
			if ((consts[1]) && ((pattern[arg1] & IMM)== IMM)){
		

				//is 16-bit register only!
				if (((pattern[arg0] & 0x01) != 0x01) || ((uint8_t) reg[0]->getAccessWidth() != 0x01)){

					return false;
					}			

				modrm = 0xC0; //MOD field is b11
				if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT)
				{
					modrm |= pattern[1];
					opcodeIndex = opcodeIndex + 1;
				}
				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				binseg->setUpdateFlag(false);
				binseg->setConstant(consts[1]);
				binseg->setAddrSize(AccessWidth::AW_16BIT);
				binseg->push_back(0);
				binseg->push_back(0);
				_addSeg(binseg);
				return true;


			}




	}


}
		else{
			if ((pattern[arg1] & MEM) == 0)
				return false;

			//REG <--MEM
				if ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth()){

					return false;
				}			

				modrm = 0x00; //MOD field is b11
				bool	hasDisp = false;
				//special case to handle intel encoding snafu
				bool zeroDisp = false;

				if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT)
				{
					modrm |= pattern[1];
					opcodeIndex = opcodeIndex + 1;
				}
				if (imm[1] && imm[1]->getAccessMode() == AccessMode::IMMEDIATE_ADDR){
					//addressing mode is disp16
					hasDisp = true;
					modrm = 0x06;
				}
				else if (consts[1] && consts[1]->getAccessMode() == AccessMode::CONST_ADDR){
					//addressing mode is disp16
					hasDisp = true;
					modrm = 0x06;
				}
				else if (reg[1] && reg[1]->getAccessMode() == AccessMode::REG_ADDR){
					if (reg[1]->isIndexable()){ 


						switch (reg[1]->getBinEncoding()){
							case (ENC_REG_BP):
							hasDisp = true;
							zeroDisp = true;							
							modrm = 0x46;
							break;
							case (ENC_REG_SI):
							modrm = 0x04;
							break;
							case (ENC_REG_DI):
							modrm = 0x05;
							break;
							case (ENC_REG_BX):
							modrm = 0x07;
							break;
							default:
							clog << "ERROR: Undefined register-addressing mode!" << endl;
							break;
						}
					 
					}
				}
				/*else if (reg[1] && reg[1]->getAccessMode() == AccessMode::REG_OFFSET){
					if (reg[1]->isIndexable()){ 

						Operands* offsets = reg[1]->getOffsetPtr();
						switch (reg[1]->getBinEncoding()){
							case (REG_BP):
							if (offsets->at(0)->
							break;


							case (REG_BX):
							modrm = 0x07;
							break;
						}
					 
					}
				}*/


				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				if (hasDisp){
				if (consts[1]){
				binseg->setUpdateFlag(false);
				binseg->setConstant(consts[1]);
				binseg->setAddrSize(AccessWidth::AW_16BIT);
				}
				binseg->push_back(0);
				binseg->push_back(0);
				}
				_addSeg(binseg);
				return true;


			



		}

}
if (isMem[0]){
/*
Acceptable memory operations:
R->Mem
Imm->Mem


#note: no inherent operations. There is a set of one-memory operand opcodes
(INV and NEG)? that take in a m8/16 operand.
*/
	//if constant and constant refers to memory addr
	if(consts[0] && isMem[0]){
	//used primarily for the CALL/JMP instruction.
	binseg->push_back(pattern[opcodeIndex]);
	binseg->setUpdateFlag(true);
	binseg->setConstant(consts[0]);

	//default to 8-bit offset
	binseg->setAddrSize(AccessWidth::AW_8BIT);
	binseg->push_back(0);

	if (pattern[arg0] & 0x01){
		binseg->setAddrSize(AccessWidth::AW_16BIT);
		binseg->push_back(0);
	}


	_addSeg(binseg);
	return true;

	}

	/*//immediates specify absolute addresses
	if(imm[0] && isMem[0]){
	//used primarily for the CALL/JMP instruction.
	binseg->push_back(pattern[opcodeIndex]);
	binseg->setUpdateFlag(false);
	binseg->setConstant(consts[0]);


	//default to 8-bit offset
	binseg->setAddrSize(AccessWidth::AW_8BIT);
	binseg->push_back(0);

	if (pattern[arg0] & 0x01){
		binseg->setAddrSize(AccessWidth::AW_16BIT);
		binseg->push_back(0);
	}


	_addSeg(binseg);
	return true;

	}
	*/




}

return false;
}



void p86Assembler::_addSeg(BinarySegment* binseg){
	binseg->setCounter(counter);
	segs.push_back(binseg);
	counter = counter + binseg->size();
}
void decodeOperands(Operands& ops, Register** rs, Immediate** imms, Constant** consts, bool isMemory[]){


	
	for (int i = 0 ; i < ops.size(); i++){
		AccessMode& am = ops[i]->getAccessMode();

		switch (am){
		case (AccessMode::REG_DIRECT):
		case (AccessMode::REG_ADDR):
		case (AccessMode::REG_OFFSET):
			rs[i] =(Register*) ops[i]; 
			break;

		case (AccessMode::IMMEDIATE):
		case (AccessMode::IMMEDIATE_ADDR):
			imms[i] = (Immediate*) ops[i];
			break;
		case (AccessMode::CONST):
		case (AccessMode::CONST_ADDR):
			consts[i] =(Constant*) ops[i];
			break;

		}
		switch (am){
		case (AccessMode::IMMEDIATE_ADDR):
		case (AccessMode::CONST_ADDR):
		case (AccessMode::REG_ADDR):
			isMemory[i] = true;
			break;
		default:
			break;
		}
	}

}



#include "p86Assembler.h"




BinarySegment::BinarySegment():data(),updateAddress(false),addr(NULL),addrSize(AccessWidth::AW_UNSPECIFIED), addrStartIndex(0),m_counter(0){


}

void BinarySegment::setAddrFlag(){

	addrStartIndex = data.size();
	updateAddress = true;

}
void BinarySegment::push_back(uint8_t byte){

	data.push_back(byte);
}

unsigned int BinarySegment::getCounter(){
	return m_counter;

}
void BinarySegment::setCounter(unsigned int c){
	m_counter = c;
}

void BinarySegment::setConstant(Constant* c){
	addr = c;

}

Constant* BinarySegment::getConstant(){
	return addr;

}

bool BinarySegment::NeedsUpdateAddress(){

	return updateAddress;
}
void BinarySegment::setAddrSize(AccessWidth aw){

	addrSize = aw;
}

AccessWidth& BinarySegment::getAddrSize(){

	return addrSize;
}

int BinarySegment::getAddrStartIndex(){
	return addrStartIndex;
}
uint8_t& BinarySegment::operator[](int index){
	return data[index];
}

int BinarySegment::size(){
	return data.size();
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
	for (int i = 0; i < segs.size();i++){
	if (segs[i]->NeedsUpdateAddress()){
	int loc_target = LocationMap.at(segs[i]->getConstant()->getName()); 
	int loc_addr = segs[i]->getCounter() + segs[i]->size();
	int addrStart = segs[i]->getAddrStartIndex();
	int diff = loc_target - loc_addr;
	switch (segs[i]->getAddrSize()){
		
		case (AccessWidth::AW_8BIT):		
		if (diff < -128 || diff > 127)
			clog << "ERROR: Jump offset is longer than 8 bits!" << endl;
		else{
			 (*segs[i])[addrStart] = (int8_t)diff & 0x00FF;
			}
		break;
		case (AccessWidth::AW_16BIT):
			printf("diff = %d", diff);
			(*segs[i])[addrStart] = (diff & 0x00FF);
			(*segs[i])[addrStart + 1] = (diff & 0xFF00) >> 8;
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
		clog << "WARNING: ORG argument must be 16-bits wide" << endl;
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
		if (opv.size() == 0)
			clog << "ERROR: Unimplemented mnemonic " << op->getContent()  << " on line " << op->getLineNumber() << endl;


		for (int j = 0;j < opv.size();j++)
			{
				uint8_t op_prop = opv[j][0];
				int opcode_offset = 1;
			if ((op_prop & OP_OPERANDS) == operands.size()){
				match = _construct(opv[j], operands);
				if (match) break;
				
			
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
//break out the operands correctly
decodeOperands(ops, &reg[0],&imm[0],&consts[0],isMem);
//single byte operands

if ((pattern[0] & OP_NO_MODRM)  &&  ((pattern[0] & OP_OPERANDS) == 0)){
			//don't need operands!
	
			binseg->push_back(pattern[opcodeIndex]);
			printSeg(binseg);
			return true;
}
if ((pattern[arg0] & IMM) == IMM){

	if (consts[0]){
//used primarily for the CALL/JMP instruction.
	binseg->push_back(pattern[opcodeIndex]);
	binseg->setAddrFlag();
	binseg->setConstant(consts[0]);

	//default to 8-bit offset
	binseg->setAddrSize(AccessWidth::AW_8BIT);
	binseg->push_back(0);

	if (pattern[arg0] & 0x01){
		binseg->setAddrSize(AccessWidth::AW_16BIT);
		binseg->push_back(0);
	}


	printSeg(binseg);
	return true;
}
}
//switch on the DEST of this opcode
if ((pattern[arg0] & REG) == REG){

		//source isn't a memory location
	if (!isMem[1]){
		//first afg isn't a reg
		if (!reg[0] || ! (reg[0]->getAccessMode() == AccessMode::REG_DIRECT))
			return false;
		//arguments are inherent to the opcode
		if (pattern[0] & OP_NO_MODRM){

			if ((pattern[0] & OP_OPERANDS) == 1){ //there is no arg2 to this operand
			/**
				Note: for the Virgo/Libra architecture, the only opcode that 
				uses one operand implied is PUSH
			**/
				uint8_t opcode = pattern[opcodeIndex];

				//OR in the register address
				if (pattern[0] & OP_REG_ADD)
					opcode |= reg[0]->getBinEncoding();


				binseg->push_back(opcode);
				printSeg(binseg);
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
				printSeg(binseg);
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
						printSeg(binseg);
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

				uint8_t modrm = 0xC0; //MOD field is b11
				if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT)
				{
					modrm |= pattern[1];
					opcodeIndex = opcodeIndex + 1;
				}
				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);

				binseg->push_back(modrm);
				printSeg(binseg);
				return true;
	


			}

			//Reg1<-Reg2
			if ((reg[1]) && ((pattern[arg1] & REG) == REG)){


				if ((reg[1]->getAccessWidth() != reg[0]->getAccessWidth()) ||
				 ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth())){	
					clog << "ERROR: Register widths do not match!" << endl;
					return false;
					}			

				uint8_t modrm = 0xC0; //MOD field is b11
				modrm |= (reg[1]->getBinEncoding() << 3) |  (reg[0]->getBinEncoding());
				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				printSeg(binseg);
				return true;	

			}
			
			//REG <--IMM
			if ((imm[1]) && ((pattern[arg1] & IMM) == IMM)){
				if ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth()){

					return false;
					}			

				uint8_t modrm = 0xC0; //MOD field is b11
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
						binseg->push_back(imm_data[imm_data.size() - i - 1]);
				}
				printSeg(binseg);
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

				uint8_t modrm = 0xC0; //MOD field is b11
				if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT)
				{
					modrm |= pattern[1];
					opcodeIndex = opcodeIndex + 1;
				}
				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				binseg->setAddrFlag();
				binseg->setConstant(consts[1]);
				binseg->setAddrSize(AccessWidth::AW_16BIT);
				binseg->push_back(0);
				binseg->push_back(0);
				printSeg(binseg);
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

				uint8_t modrm = 0x00; //MOD field is b11
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
				binseg->push_back(0);
				binseg->push_back(0);
				}
				printSeg(binseg);
				return true;


			



		}

}
if (pattern[arg0] & MEM == MEM){


}

return false;
}
void p86Assembler::printSeg(BinarySegment* binseg){
binseg->setCounter(counter);
segs.push_back(binseg);


counter = counter + binseg->size();
}
void decodeOperands(Operands& ops, Register** rs, Immediate** imms, Constant** consts, bool isMemory[]){



	for (int i = 0 ; i < ops.size(); i++){
		if (ops[i]->getAccessMode() == AccessMode::REG_DIRECT || ops[i]->getAccessMode() == AccessMode::REG_ADDR){
			rs[i] =(Register*) ops[i]; 
		}
		else if (ops[i]->getAccessMode() == AccessMode::IMMEDIATE || ops[i]->getAccessMode() == AccessMode::IMMEDIATE_ADDR)
			imms[i] = (Immediate*) ops[i];
		else if (ops[i]->getAccessMode() == AccessMode::CONST || ops[i]->getAccessMode() == AccessMode::CONST_ADDR)
			consts[i] =(Constant*) ops[i];
	
	if ((ops[i]->getAccessMode() == AccessMode::IMMEDIATE_ADDR) ||
		 (ops[i]->getAccessMode() == AccessMode::CONST_ADDR) ||
		 (ops[i]->getAccessMode() == AccessMode::REG_ADDR))
		isMemory[i] = true;

	}

}

static bool type_match(Operand* op){
		clog << "This is a reg!" << endl;
	if (dynamic_cast<Immediate*> (op))
		clog << "This is a imm!" << endl;
	if (dynamic_cast<Constant*> (op))
		clog << "This is a const!" << endl;

}   



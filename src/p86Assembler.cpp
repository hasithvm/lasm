#include "p86Assembler.h"


#define IS_REG_DIRECT(r) (r->getAccessMode()==AccessMode::REG_DIRECT)



p86Assembler::p86Assembler(): st(), segs(), counter(0),LocationMap(), m_codeStart(0){

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
	while (_postpass());
}
bool p86Assembler::_postpass(){

	int loc_target;
	int loc_addr;
	int finalAddress;
	int addrStart;
	bool rerunPass = false;
	for (int i = 0; i < segs.size();i++){
		if (segs[i]->getUpdateFlag()){
			try{
				loc_target = LocationMap.at(segs[i]->getConstant()->getName()); 
			}

			catch (std::out_of_range& e){
				cerr << "ERROR: Undefined label " << segs[i]->getConstant()->getName() << endl;
				continue;
			}

			loc_addr = segs[i]->getCounter() + segs[i]->size();
			addrStart = segs[i]->getAddrStartIndex();

			if (segs[i]->getRelativeAddressFlag())
				finalAddress = loc_target - loc_addr;
			else
				finalAddress = loc_target;


			switch (segs[i]->getAddrSize()){


				case (AccessWidth::AW_8BIT):		

				if (finalAddress < -128 || finalAddress > 127){
					clog << "WARNING: Jump offset is longer than 8 bits!" << endl;
				//set the 16-bit bit field.
					(*segs[i])[0] |= 0x01;
					segs[i]->setAddrSize(AccessWidth::AW_16BIT);
					segs[i]->push_back(0);
					updateLocationMap(loc_addr, 1);
					rerunPass = true;
				}
				else
					(*segs[i])[addrStart] = (int8_t)(finalAddress & 0x00FF);

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
	return rerunPass;
}

void p86Assembler::updateLocationMap(unsigned int startFrom, int increment){
	std::map<std::string, unsigned int>::iterator itr;
	
	for (itr = LocationMap.begin();itr != LocationMap.end();++itr){
		if (itr->second > startFrom)
			itr->second = itr->second + increment;

	}

}

void p86Assembler::_handleLabelNode(LabelNode* label){
	if (LocationMap.count(label->getContent())){
		//this label has been predefined
		cerr << "ERROR: Multiply-defined label " << label->getContent() << "on line" << label->getLineNumber() << endl;
	}

	LocationMap[label->getContent()] = counter;
	clog << "Location " << label->getContent() << " maps to counter: " << counter << endl;
}

void p86Assembler::_handleControlNode(ControlNode* ctrl){
	
	BinarySegment* binseg = new BinarySegment();
	Immediate* immVal = NULL;
	Constant* constVal = NULL;

	if (ctrl->getValue()->getAccessMode() == AccessMode::IMMEDIATE)
		immVal = (Immediate*) ctrl->getValue();

	else if (ctrl->getValue()->getAccessMode() == AccessMode::CONST)
		constVal = (Constant*) ctrl->getValue();

	switch(ctrl->getControlType()){


		case (ControlNodeType::CONTROL_DB):


		
		binseg->setCounter(counter);
		if (constVal){
			cerr << "ERROR: reserving memory bytes requires an immediate operand" << endl;
			return;
		}
		for (int i = 0;i < immVal->size();i++)
			binseg->push_back(immVal->getBinEncoding()[i]);
		binseg->setStringData(immVal->getSourceRepr());
		segs.push_back(binseg);
		counter = counter + binseg->size();
		break;
	//TODO: Implement DW 
		case (ControlNodeType::CONTROL_DW):
		binseg->setCounter(counter);
		if (constVal){
			binseg->setUpdateFlag(false);
			binseg->setConstant(constVal);
			binseg->setAddrSize(AccessWidth::AW_16BIT);
			binseg->push_back(0);
			binseg->push_back(0);
			segs.push_back(binseg);
		}
		else if (immVal){
			for (int i = 0;i < immVal->size();i+=2){
				binseg->push_back(immVal->getBinEncoding()[i]);
				binseg->push_back(immVal->getBinEncoding()[i+1]);
			}
			segs.push_back(binseg);

		}
		else
			cerr << "ERROR: reserving memory requires an immediate or constant operand" << endl;

		counter = counter + binseg->size();
		break;

		case (ControlNodeType::CONTROL_ORG):

		if (immVal->size() != 2){
			cout << "WARNING: ORG argument must be 16-bits wide! Skipping" << endl;
			return;		
		}
		counter = (immVal->getBinEncoding()[1] << 8) | immVal->getBinEncoding()[0];
		break;

		case (ControlNodeType::CONTROL_END):

		m_codeStart = LocationMap.at(((Constant*)ctrl->getValue())->getName());

		break;

		
		default:
		break;
	}
}
void p86Assembler::_handleOpNode(OpNode* op){
	int match = 1;
	Operands& operands = op->getOperands();
	OpVars& opv = st.at(op->getContent());
	if (opv.size() == 0){
		cerr << "ERROR: Unimplemented mnemonic " << op->getContent()  << " on line " << op->getLineNumber() << endl;
		return;			
	}

	for (int j = 0;j < opv.size();j++)
	{
		uint8_t op_prop = opv[j][0];
		int opcode_offset = 1;
			//if ((op_prop & OP_OPERANDS) == operands.size()){
		match = _construct(opv[j], op, operands);
		if (match == 0){
			segs[segs.size()-1]->setStringData(getSourceRepr(op));
			break;
		}
		if (match == -1)
			break;

			//}   

	}
	if (match == 1)
		cerr << "ERROR: Incorrect operands for mnemonic " << op->getContent()  << " on line " << op->getLineNumber() << endl;
	else if (match ==-1)
		cerr << "ERROR: Assembly failed on " << op->getContent()  << " at line " << op->getLineNumber() << endl;
}

/**
*Constructs an opcode from an optype param and given params
*@param pattern OpType to construct
*@param ops source operands specified in code
*@return None
*/
int p86Assembler::_construct(OpType pattern,OpNode* op, Operands& ops){

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
	bool hasDisp = false;
	bool zeroDisp = false;
	vector<uint8_t> immediateData;
	unsigned int immSz = 0;

//break out the operands correctly
	decodeOperands(ops, &reg[0],&imm[0],&consts[0],isMem);
//single byte operands

	if ((pattern[0] & OP_NO_MODRM)  &&  ((pattern[0] & OP_OPERANDS) == 0)){
			//don't need operands!
		if (ops.size() != 0){
			cerr << "ERROR: opcode "<< op->getContent() << " does not require operands!" << endl;
			return -1;
		}
		binseg->push_back(pattern[opcodeIndex]);
		_addSeg(binseg);
		return 0;
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
			return 0;
		}
		if (imm[0] && ops.size() == 1){

			binseg->push_back(pattern[opcodeIndex]);
			immediateData = imm[0]->getBinEncoding();

			immSz = (pattern[arg0] & 0x01) + 1;
		//sign-extend the operand
			if (immediateData.size() < immSz){
				int16_t data =(int16_t) immediateData[0];
				binseg->push_back((uint8_t)(data & 0xFF00));	
				binseg->push_back((uint8_t)(data & 0x00FF));	
			}
			else {
				for (int i = 0; i < immSz;i++)		
					binseg->push_back(immediateData[i]);
			}

			_addSeg(binseg);
			return 0;


		}

	//if (imm[0] && )

	}
//switch on the DEST of this opcode
	if (!isMem[0]){

		//source isn't a memory location
		if (!isMem[1]){
		//first afg isn't a reg
			if (!reg[0] || ! (reg[0]->getAccessMode() == AccessMode::REG_DIRECT))
				return 1;
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
				return 0;
			}



			//without a mod/rm byte, it has to be a REG(preset) <-IMM
			if (reg[0]->getBinEncoding() != (pattern[arg0] >> 5))
				return 1;
			

			if (reg[1] && (pattern[arg1] & REG_PRESET) &&
				(reg[1]->getBinEncoding() == pattern[arg1] >> 5) &&
				(reg[1]->getAccessWidth() == (pattern[arg1] & 0x01)))
			{

			//this is an OUT DX,A[L|X] instruction.
				binseg->push_back(pattern[opcodeIndex]);
				_addSeg(binseg);
				return 0;

			}

			//reject if the next arg isn't an immediate
			if (!imm[1] || ((uint8_t)reg[0]->getAccessWidth() != (pattern[arg0] & 0x01)))
				return 1;

			//this is now not a REG<-mem operation (no mod/rm), it has to be a REG<-imm
			binseg->push_back(pattern[opcodeIndex]);
			immediateData = imm[1]->getBinEncoding();

			int len = (pattern[arg0] & 0x01 ) + 1;


			for (int i = 0; i < len;i++){		
				binseg->push_back(immediateData[immediateData.size() - i - 1]);
			}
			_addSeg(binseg);
			return 0;

			

		}
		else{
			if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT){
				modrm |= pattern[1];
				opcodeIndex = opcodeIndex + 1;
			}



			//single operand opcode that requires a mod/rm byte
			if (!reg[1] && !imm[1] && (arg1 == pattern.size()) && !isMem[0])
			{

				if ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth()){	
					return 1;
				}			

				modrm |= 0xC0; //MOD field is b11

				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);

				binseg->push_back(modrm);
				_addSeg(binseg);
				return 0;



			}

			//Reg1<-Reg2
			if ((reg[1]) && ((pattern[arg1] & REG) == REG)){


				if (reg[1]->getAccessWidth() != (pattern[arg1] & 0x01) ||
					((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth())){	
					return 1;
			}
			if ((pattern[arg1] & REG_PRESET)){
				if (reg[1]->getBinEncoding() != pattern[arg1] >> 5)
					return 1;
			}

				modrm |= 0xC0; //MOD field is b11
				modrm |= (reg[0]->getBinEncoding());

				if (!(pattern[0] & OP_MODRM_EXT))
					modrm |= reg[1]->getBinEncoding() << 3;

				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				_addSeg(binseg);
				return 0;	

			}
			
			//REG <--IMM
			if ((imm[1]) && ((pattern[arg1] & IMM) == IMM)){
				if ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth()){

					return 1;
				}			

				modrm |= 0xC0; //MOD field is b11

				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				vector<uint8_t>& imm_data = imm[1]->getBinEncoding();
				
				int len =reg[0]->getAccessWidth()+ 1;
				if (imm_data.size() < len){
					int16_t data =(int16_t) imm_data[0];
					binseg->push_back((uint8_t)(data & 0x00FF));	
					binseg->push_back((uint8_t)(data & 0xFF00));	

				}
				else {
					for (int i = 0; i < len;i++)		
						binseg->push_back(imm_data[i]);
				}
				_addSeg(binseg);
				return 0;


			}
			//REG <--IMM [CONST]. This is reserved for final pass label updates.
			/*
			Syntax example: mov BX, label, where label is a label pointing to data and
			not a simple constant. Only reserved for 16-bit registers
	
			*/
			if ((consts[1]) && ((pattern[arg1] & IMM)== IMM)){


				//is 16-bit register only!
				if ((pattern[arg0] & 0x01) != 0x01)
					return 1;
				if (reg[0]->getAccessWidth() != 0x01){
					cerr << "ERROR: 16-bit register required" << endl;
					return -1;
				}			

				modrm |= 0xC0; //MOD field is b11

				modrm |= reg[0]->getBinEncoding();
				binseg->push_back(pattern[opcodeIndex]);
				binseg->push_back(modrm);
				binseg->setUpdateFlag(false);
				binseg->setConstant(consts[1]);
				binseg->setAddrSize(AccessWidth::AW_16BIT);
				binseg->push_back(0);
				binseg->push_back(0);
				_addSeg(binseg);
				return 0;


			}




		}


	}
	else {
		if ((pattern[arg1] & MEM) == 0)
			return 1;

			//REG <--MEM
		if ((pattern[arg0] & 0x01) != (uint8_t) reg[0]->getAccessWidth()){

			return 1;
		}			

				modrm = 0x00; //MOD field is b11
				hasDisp = false;
				//special case to handle intel encoding snafu
				zeroDisp = false;

				if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT)
				{
					modrm |= pattern[1];

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
							cerr << "ERROR: Undefined register-addressing mode!" << endl;
							return -1;
							break;
						}

					}
				}
				else if (reg[1] && reg[1]->getAccessMode() == AccessMode::REG_OFFSET){
					if (reg[1]->isIndexable()){ 

						Operands* offsets = reg[1]->getOffsetPtr();
						switch (reg[1]->getBinEncoding()){
							case (REG_BP):
							//if (offsets->at(0)->
							break;


							case (REG_BX):
							modrm = 0x07;
							break;
						}

					}

				}

					modrm |= reg[0]->getBinEncoding() << 3;
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
					return 0;






				}

			}
	if (isMem[0] && (pattern[arg0] & MEM)){
/*
Acceptable memory operations:
R->Mem
Imm->Mem


#note: no inherent operations. There is a set of one-memory operand opcodes
(INV and NEG)? that take in a m8/16 operand.
*/
	//if constant and constant refers to memory addr

if ((pattern[0] & OP_MODRM_EXT) == OP_MODRM_EXT){
	modrm |= pattern[1];
	opcodeIndex = opcodeIndex + 1;
}

if(consts[0]){

		if (reg[1] && (pattern[arg1] & REG)){
			if( (pattern[arg1] & 0x01) != (uint8_t) reg[1]->getAccessWidth())
				return 1;

			modrm |= reg[1]->getBinEncoding() << 3;
		}
		else if (imm[1] && (pattern[arg1] & IMM)){



			if (op->getExplicitAccessModifier() == AW_UNSPECIFIED){
				cerr << "ERROR: Ambiguous operand size for " << op->getContent() << endl;
				return -1;
			}
			if ((uint8_t)op->getExplicitAccessModifier() != (pattern[arg0] & 0x01))
				return 1;

			immediateData = imm[1]->getBinEncoding();
			immSz = (unsigned int)op->getExplicitAccessModifier() + 1;


		}
		else if (isMem[1]){

			cerr << "ERROR: memory-to-memory operations not supported!" << endl;
			return -1;
		}

		modrm |= 0x06;
		binseg->push_back(pattern[opcodeIndex]);
		binseg->push_back(modrm);
		binseg->setUpdateFlag(false);
		binseg->setConstant(consts[0]);


	//addr is 16-bit offset
		binseg->setAddrSize(AccessWidth::AW_16BIT);
		binseg->push_back(0);
		binseg->push_back(0);
		if (immSz){
		//sign-extend the operand
			if (immediateData.size() < immSz){
				int16_t data =(int16_t) immediateData[0];
				binseg->push_back((uint8_t)(data & 0xFF00));	
				binseg->push_back((uint8_t)(data & 0x00FF));	
			}
			else {
				for (int i = 0; i < immSz;i++)		
					binseg->push_back(immediateData[i]);
			}

		}


		_addSeg(binseg);
		return 0;

	}


	else if (reg[0]){
		hasDisp = false;
		bool hasImm = false;
		bool isWordAccess = false;
		if( reg[0]->getAccessMode() == AccessMode::REG_ADDR){
			if (reg[0]->isIndexable()){ 
				switch (reg[0]->getBinEncoding()){
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

			}
			else{
				cerr << "ERROR: Undefined register-addressing mode!" << endl;
				return -1;
			}

		}
		else
			cerr << "ERROR: Unimplemented register addressing mode!" << endl;

		if (reg[1] && IS_REG_DIRECT(reg[1]) && (pattern[arg1] & REG))
			modrm |= reg[1]->getBinEncoding() <<  3;
		else if (imm[1] && imm[1]->getAccessMode() == AccessMode::IMMEDIATE)
			hasImm = true;
		else{
			cerr << "ERROR: Unsupported source for operation" << endl;
			return -1;
		}
		binseg->push_back(pattern[opcodeIndex]);
		binseg->push_back(modrm);
		if (hasImm){
			if (op->getExplicitAccessModifier() == AW_UNSPECIFIED){
				cerr << "ERROR: Ambiguous operand sizes. Use \"byte\" or \"word\" modifier" << endl;
				return -1;
			}

			int len = ((int) op->getExplicitAccessModifier()) + 1;
			
			if (imm[1]->size() < len){
				binseg->push_back(imm[1]->getBinEncoding()[0]);
				binseg->push_back(0);
			}
			else
				for (int i = 0;i < len;i++)
					binseg->push_back(imm[1]->getBinEncoding()[i]);	


		}


		return 0;
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

return 1;
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


static std::string getSourceRepr(OpNode* ptr){
	std::string returnStr;
	std::string paramStr;
	returnStr = ptr->getContent();
	Operands& ops = ptr->getOperands();
	for (int i = 0; i < ops.size();i++)
	{
		paramStr = "";
		switch (ops[i]->getAccessMode()){
			case (AccessMode::REG_DIRECT):
			paramStr =((Register*) ops[i])->getRegName();
			break;
			case (AccessMode::REG_ADDR):
			paramStr = "[" + ((Register*) ops[i])->getRegName() + "]";
			break;
			case (AccessMode::CONST_ADDR):
			paramStr = "[";
				paramStr+=((Constant*) ops[i])->getName();
				paramStr += "]";
break;
case (AccessMode::IMMEDIATE):
paramStr =((Immediate*) ops[i])->getSourceRepr();
break;
case (AccessMode::IMMEDIATE_ADDR):
paramStr ="[" + ((Immediate*) ops[i])->getSourceRepr() + "]";
case (AccessMode::CONST):
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



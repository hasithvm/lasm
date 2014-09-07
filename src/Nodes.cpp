#include "Nodes.h"
#ifndef _MSC_VER
#include <stdint.h>
#else
#include <cstdint>
#endif

BaseExpressionNode* BaseExpressionNode::getNextExpr(){
	return m_nextptr;

}

void BaseExpressionNode::setNextExpr(BaseExpressionNode* ptr){
	m_nextptr = ptr;
}


void BaseExpressionNode::setLineNumber(int lineno){
	m_line  = lineno;

}

int BaseExpressionNode::getLineNumber(){
	return m_line;

}

ostream& operator<<(ostream& stream, BaseExpressionNode& node)
{
	node.repr(stream);
	return stream;
}

//================================================================


OpNode::OpNode(unsigned int nOpcodeID, Operands* op)
{

	m_id = nOpcodeID;
	m_aw = AW_UNSPECIFIED;
	ops = *op;
}

ExpressionType OpNode::getType(){
	return EXPR_OP;
}


const char* OpNode::getContent(){

	return SymTable::translateIDToName(m_id);;
}

Operands& OpNode::getOperands(){
	return ops;
}

void OpNode::setID(int id){
	m_id = id;
}
int OpNode::getOperandCount(){
	int tmp_len =0;
	for (unsigned int i = 0; i < ops.size();i++)
		tmp_len = tmp_len + !ops[i] ? 0 : 1;
	return tmp_len;
}

ostream& OpNode::repr(ostream& stream){
	stream << Indent << "<statement>" << endl;
	stream << IncreaseIndent;
	stream << Indent << "<type>opcode</type>" << endl;


	stream << Indent <<  "<opcode>" << getContent() << "</opcode>" << endl;
	stream << Indent << "<id>" << m_id << "</id>" << endl;

	stream << Indent << "<params>" << endl;
	stream << IncreaseIndent;
	for (unsigned int i = 0; i < ops.size();i++)
	{
		if (ops[i])
			stream << *ops[i];
	}
	stream << DecreaseIndent;
	stream << Indent << "</params>" << endl;
	stream << DecreaseIndent;
	stream << Indent << "</statement>" << endl;
	

	return stream;

}


AccessWidth OpNode::getExplicitAccessModifier(){
	return m_aw;
}
void OpNode::setExplicitAccessModifier(AccessWidth aw){
	m_aw = aw;
}



/*===========================================================*/
ControlNode::ControlNode(char* e, Operand* i){
	label = std::string(e+1);
 	ctrltype = ControlNode::decodeText(label);
	imm = i;
	m_key = std::string();
}

ControlNode::CtrlTypeLookupMap ControlNode::tlm(ControlNode::_populate());

ControlNode::CtrlTypeLookupMap  ControlNode::_populate(){

	ControlNode::CtrlTypeLookupMap ret;
	ret["db"] = CONTROL_DB;
	ret["dw"] = CONTROL_DW;
	ret["equ"] = CONTROL_EQU;
	ret["org"] = CONTROL_ORG;
	ret["end"] = CONTROL_END;
	return ret;

}

ExpressionType ControlNode::getType(){
	return EXPR_CONTROL;
}

ControlNodeType ControlNode::getControlType(){
	return ctrltype;
}

ControlNodeType ControlNode::decodeText(std::string& text){
	strToLowerCase(text);
	return tlm[text];
}

ostream& ControlNode::repr(ostream& stream){
		
	stream << Indent << "<control>" << endl;
	stream << IncreaseIndent;
	stream << Indent << "<name>" << label << "</name>" << endl;
	stream << Indent << "<key>" << m_key << "<key>" << endl;
	stream << Indent << "<value>" << endl;
	stream << IncreaseIndent;
	stream << *imm;
	stream << DecreaseIndent;
	stream << Indent << "<value>" << endl;
	stream << DecreaseIndent;
	stream << Indent << "<control>" << endl;
	return stream;
}

void ControlNode::setContent(std::string a){
	label = a;
}

std::string& ControlNode::getContent(){
	return label;
}


void ControlNode::setImmediate(Immediate* im){
imm = im;

}

void ControlNode::setKey(char* a){
	m_key = std::string(a);

}

std::string& ControlNode::getKey(){
	return m_key;
}

Operand* ControlNode::getValue(){
	return imm;
}

LabelNode::LabelNode(char* a){
	m_label = std::string(a);
}

std::string& LabelNode::getContent(){
	return m_label;
}

void LabelNode::setContent(std::string a){
	m_label = a;
}

ostream& LabelNode::repr(ostream& stream){
	stream << Indent << "<label>" << endl;
	stream << IncreaseIndent;
	stream << Indent << "<name>" << m_label << "</name>" << endl;
	stream << DecreaseIndent;
	stream << Indent << "<label>" << endl;
	return stream;
}

ExpressionType LabelNode::getType(){
	return EXPR_LABEL;
}


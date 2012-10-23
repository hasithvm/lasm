#include "Nodes.h"

OpNode::OpNode(std::string opName, Operands* op)
{
	ops = *op;
	opstr = opName;
	strToLowerCase(opstr);
	m_aw = AccessWidth::AW_UNSPECIFIED;
}

ExpressionType OpNode::getType(){
	return ExpressionType::EXPR_OP;
}

void OpNode::setContent(std::string a)
{
	opstr = a;
	strToLowerCase(opstr);
}

std::string OpNode::getContent(){
	return opstr;
}

int OpNode::getOperandCount(){
	int tmp_len =0;
	for (int i = 0; i < ops.size();i++)
		tmp_len = tmp_len + !ops[i] ? 0 : 1;
	return tmp_len;
}

void OpNode::repr(int indentlevel){
	std::string indenter(indentlevel, '\t');	
	clog << indenter << "<statement>" << endl;
	clog << indenter << "\t<type>opcode</type>" << endl;
	clog << indenter << "\t<opcode>" << opstr << "</opcode>" << endl;
	clog << indenter << "\t<params>" << endl;
	for (int i = 0; i < ops.size();i++)
	{
		if (ops[i])
			ops[i]->repr(indentlevel + 2);
	}
	clog << indenter << "\t</params>" << endl;
	clog << indenter << "</statement>" << endl;
	

}
AccessWidth OpNode::getExplicitAccessModifier(){
	return m_aw;
}
void OpNode::setExplicitAccessModifier(AccessWidth aw){
	m_aw = aw;
}


ControlNode::ControlNode(char* pText, Immediate* i){
std::string a = std::string(pText);

decodeText(a);

}


ControlNode::ControlNodeType ControlNode::decodeText(std::string& text){
	strToLowerCase(text);
	
}





CommentNode::CommentNode(std::string a){
 cmt = a;
}

ExpressionType CommentNode::getType(){
	return ExpressionType::EXPR_COMMENT;
}

void CommentNode::setContent(std::string a){
cmt = a;
}
std::string CommentNode::getContent(){
	return cmt;
}

void CommentNode::repr(int indentlevel){
	std::string indenter(indentlevel, '\t');	
	clog << indenter << "<comment>" << endl;
	clog << indenter << "\t<content>" << cmt << "</content>" << endl;
	clog << indenter << "<comment>" << endl;
}

LabelNode::LabelNode(char* a){
	m_label = std::string(a);
}

std::string LabelNode::getContent(){
	return m_label;
}

void LabelNode::setContent(std::string a){
	m_label = a;
}

void LabelNode::repr(int indentlevel){
	std::string indenter(indentlevel, '\t');	
	clog << indenter << "<label>" << endl;
	clog << indenter << "\t<name>" << m_label << "</name>" << endl;
	clog << indenter << "<label>" << endl;
}

ExpressionType LabelNode::getType(){
	return ExpressionType::EXPR_LABEL;
}


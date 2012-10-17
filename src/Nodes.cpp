#include "Nodes.h"

OpNode::OpNode(std::string opName, Operands* op)
{
	ops = *op;
	opstr = opName;
	strToLowerCase(opstr);
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

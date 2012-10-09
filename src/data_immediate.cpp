#include "data.h"
Immediate::Immediate(): m_am(UNINITIALIZED),m_data(){

}

Immediate::Immediate(std::string a,ImmediateEncoding base, AccessMode am):
m_am(am)
{
	cerr << a << endl;
	m_data = Immediate::parse(a,base);
}

void Immediate::repr(){

	clog << "<Immediate>" << endl;
	if (m_data.size() == 0)
		clog << "\tvalue:\tuninitialized"<<endl;	
	clog << "\tvalue:\t" << hex2str(&m_data[0], m_data.size()) << endl;
	clog << "</Immediate>" << endl;
}

AccessMode Immediate::getAccessMode(){
return m_am;
}

vector<uint8_t> Immediate::getBinEncoding(){
return m_data;
}


vector<uint8_t> Immediate::parse(std::string in, ImmediateEncoding base)
{
string::reverse_iterator it;
vector<uint8_t> out;
int padding = 0;
int bytes_written=0;
unsigned int tmp = 0;
std::transform(in.begin(),in.end(), in.begin(), convlower);

//pad with zero if not an even mult

switch(base)
{
	case BASE_HEX:
	if (in.length() % 2 != 0)
		in.insert(0,1,'0');
	
	out.resize(in.length() / 2);	
	//in.resize(4);
	for (it = in.rbegin();it!=in.rend();++it)
{
	
	out[bytes_written] = parseDigit(*it) | parseDigit(*++it) << 4 ;
	bytes_written++;	

}

	break;
	case BASE_BIN:
	padding =  8 - (in.length() % 8);
	if (padding != 8)
		in.insert(0, padding, (char)in[0]);
	out.resize(in.length() / 8);
	for (it = in.rbegin();it < in.rend();it++)
	{
	
		for (int i = 0; i < 8;i++)
		{
					out[bytes_written] |= parseDigit(*it) << i;
					it++;
			}
		clog << out[bytes_written];
	}
	break;
	default:
	break;

}

	return out;
}

char convlower(char in)
{
	if ( in >= 'A'&& in <='Z')
		return in-('A' -'a');

	return in;
}

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
int bytes_written=0;
unsigned int tmp = 0;
std::transform(in.begin(),in.end(), in.begin(), convlower);

//pad with zero if not an even mult
if (in.length() % 2 != 0)
	in.insert(0,1,'0');
switch(base)
{
	case BASE_HEX:
	out.resize(in.length() / 2);	
	//in.resize(4);
	for (it = in.rbegin();it!=in.rend();++it)
{
	printf("%s\n",&*it);
	sscanf((const char *)&*it,"%x",&tmp);
	it++;
	
out[bytes_written] = tmp;
		printf("%s\n",&*it);
	sscanf((const char *)&*it,"%x",&tmp);

	out[bytes_written] = tmp << 4;
	bytes_written++;
	tmp = 0;
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

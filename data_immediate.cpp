#include "data.h"
Immediate::Immediate(): m_am(UNINITIALIZED),m_data(){

}

Immediate::Immediate(std::string a,int base, AccessMode am):
m_am(am)
{
	cerr << a << endl;
	m_data = Immediate::parse(a,base);
}

void Immediate::repr(){
clog << "<Immediate>" << endl;
clog << "\tvalue:\t" << hex2str(&m_data[0], m_data.size()) << endl;
clog << "</Immediate>" << endl;



}

AccessMode Immediate::getAccessMode(){
return m_am;
}

std::vector<uint8_t> Immediate::getByteArray(){
return m_data;
}


std::vector<uint8_t> Immediate::parse(std::string in, int base)
{
vector<uint8_t> out;
string::reverse_iterator it;

int bytes_written=0;
unsigned int tmp = 0;
std::transform(in.begin(),in.end(), in.begin(), convlower);

switch(base)
{
	case 16:
	out.resize(in.size() / 2);
	for (it = in.rbegin();it<in.rend();it+=2)
{	

	sscanf((const char *)&*it,"%x",&tmp);
	it++;

	out[bytes_written] = tmp;
	//clog << tmp<< endl;
	sscanf((const char *)&*it,"%x",&tmp);
//clog << tmp << endl;	
	out[bytes_written] = tmp << 4;
	bytes_written++;
	tmp = 0;
}

	break;
	default:
	break;

}

clog << hex2str(&out[0],out.size()) << endl;
return out;
}

char convlower(char in)
{
if ( in >= 'A'&& in <='Z')
	return in-('A' -'a');
return in;
}

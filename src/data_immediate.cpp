#include "data.h"

Immediate::Immediate(): m_data()
{
}

Immediate* Immediate::clone() const
{
	return new Immediate(*this);
}

Immediate::Immediate(char* pValue, ImmediateEncoding base, AccessMode am)
{
	Operand::setAccessMode(am);
	m_orig = std::string(pValue);

	m_data = Immediate::parse(m_orig, base);
	switch (base)
	{
	case BASE_HEX:
		strToUpperCase(m_orig);
		m_orig.append("H") ;
		break;
	case BASE_ASC:
		m_orig = "'" + m_orig;
		m_orig = m_orig + "'";
		break;

	case BASE_BIN:
		m_orig = "0b" + m_orig;
		break;
	default:
		break;
	}
}

ostream& Immediate::repr(ostream& stream)
{

	stream << Indent << "<Immediate>" << endl;
	stream << IncreaseIndent;
	if (m_data.size() == 0)
		stream << Indent << "<value>uninitialized</value>" << endl;
	else
		stream << Indent << "<value>";

		for (std::vector<uint8_t>::const_reverse_iterator ri = m_data.rbegin();
			 ri != m_data.rend(); ri++)
			{
				stream << std::hex << *ri;		
			} 
		stream << "</value>" << endl;

	stream << DecreaseIndent;
	stream << Indent << "</Immediate>" << endl;
	return stream;
}


vector<uint8_t>& Immediate::getBinEncoding()
{
	return m_data;
}

string& Immediate::getSourceRepr()
{
	return m_orig;
}

uint16_t Immediate::toWord()
{
	if (m_data.size() >= 2)
		return (uint16_t)((m_data[1] << 8) | (m_data[0]));
	else if (m_data.size() == 1)
		return (uint16_t) m_data[0];
	return 0;
}

vector<uint8_t> Immediate::parse(std::string& in, ImmediateEncoding base)
{
	string::iterator it_fwd;
	string::reverse_iterator it;
	vector<uint8_t> out;
	int padding = 0;
	int bytes_written = 0;
	unsigned int tmp = 0;
	int counter = 1;
	std::string intermediate;

	//pad with zero if not an even mult

	switch (base)
	{
	case BASE_HEX:
		strToLowerCase(in);

		if (in.length() % 2 != 0)
			in.insert(0, 1, '0');

		//skip the delmiting commas.
		out.resize((in.length()) / 2);
		//in.resize(4);
		for (it = in.rbegin();it != in.rend();++it)
		{
			out[bytes_written] = parseDigit(*it) | parseDigit(*++it) << 4 ;
			bytes_written++;
		}
		while ((out.size() > 1) && out.back() == 0x00)
			out.pop_back();

		break;
	case BASE_BIN:
		strToLowerCase(in);
		padding = 8 - (in.length() % 8);
		if (padding != 8)
			in.insert(0, padding, (char)in[0]);
		out.resize(in.length() / 8);

		for (it = in.rbegin();it != in.rend();)
		{
			for (int i = 0; i < 8;i++)
			{
				out[bytes_written] |= (*it - '0') << i;
				it++;
			}
			bytes_written++;
		}
		while ((out.size() > 1) && out.back() == 0x00)
			out.pop_back();
		break;
	case BASE_ASC:
		out.resize(in.length() - 2);
		trim(in, in[0]);

		for (it_fwd = in.begin(); it_fwd < in.end();it_fwd++)
		{
			out[bytes_written] = (uint8_t)*it_fwd;
			bytes_written++;
		}

		break;
	case BASE_DEC:

		intermediate = in.at(0) == '-' ? in.substr(1, -1) : in;

		for (it = intermediate.rbegin();it < intermediate.rend();it++)
		{
			tmp = tmp + (counter) * parseDigit(*it);
			counter *= 10;
		}

		if (in.at(0) == '-')
			tmp = (tmp * -1) & 0xFFFF;

		do
		{
			out.push_back((uint8_t)(tmp & 0xFF));
			tmp >>= 8;
		}
		while (tmp > 0);

	default:
		break;

		//clear out any zero-padding to make sure it fits in smallest byte count.
		//zero bytes isn't valid though...
	}


	return out;
}

int Immediate::size()
{
	return m_data.size();
}

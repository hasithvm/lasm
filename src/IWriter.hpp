#ifndef _I_WRITER_H_
#define _I_WRITER_H_

class IWriter{
	IWriter();
	virtual void WriteFile(vector<TextSegment*> segment, std::string& filename) = 0;
};








#endif
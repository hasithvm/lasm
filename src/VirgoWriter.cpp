#include "VirgoWriter.h"

#ifdef VS2010
#define snprintf(a, b, c, ...) _snprintf_s(a, b, b, c, __VA_ARGS__)
#endif

std::string hexdump(uint8_t* bytes, int count);
std::string hexdump(unsigned int a);
std::string hexdump_reverse(unsigned int a);
void writeFile(vector<BinarySegment*>& segments, string& filename, unsigned int startingAddress){
	ofstream outfile;		
	int bytes_written = 0;
	clog << "Starting object-write process!" << endl; 
	outfile.open (filename);
	Constant* label;
	 	
	outfile << "3 " << segments.size() << " " << startingAddress << "       " << endl;


	for (int i = 0; i < segments.size();i++){

		outfile <<  "\x1f" << hexdump_reverse(segments[i]->getCounter()) <<  "\x1f";
		outfile <<  "\x1f" << segments[i]->size() <<  "\x1f";
		bytes_written += segments[i]->size();
		outfile << "\x1f"  << hexdump(&(*segments[i])[0], segments[i]->size()) << "\x1f";

		outfile << "\x1f\x1f";
		if (segments[i]->getStringData() != "")
			outfile << "\x1f" << segments[i]->getStringData() << "\x1f";
		else
			outfile << "\x1f" << "\x1f";
		outfile << "\x1f" <<20 <<"\x1f" << endl;
		}


	outfile.seekp(0);
	outfile << "3 " << segments.size() << " " << startingAddress << " " << bytes_written << " ";
	outfile.close();

}



/**
TODO: ew, really? clean this up!
**/
std::string hexdump(unsigned int a){
return hexdump((uint8_t*)&a, 2);

}
std::string hexdump_reverse(unsigned int a){

return hex2str((uint8_t*)&a, 2);
}

std::string hexdump(uint8_t* bytes, int count)
{
std::string temp;
temp.reserve(count*2);
char* str = (char*)malloc(sizeof(char)*4);
for (int i=0;i <count;i++)
{
snprintf(str,3, "%02X", bytes[i]);
temp.append((const char *)str,2);
}
free(str);
	return temp;

}

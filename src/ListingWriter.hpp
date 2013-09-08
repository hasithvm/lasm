#ifndef _LISTING_WRITER_H_
#define _LISTING_WRITER_H_
#include "IWriter.hpp"
#include <iostream>
#include <iomanip>


class ListingWriter : public IWriter {

		static void writeFile(vector<BinarySegment*>& segments, std::string& filename, unsigned int startingAddress);
		


};





#endif
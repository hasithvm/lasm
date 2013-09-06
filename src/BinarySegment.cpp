#include "BinarySegment.hpp"
BinarySegment::BinarySegment()
:m_data(),
m_updateAddress(false),
m_addr(NULL),
m_addrSize(AW_UNSPECIFIED),
m_addrStartIndex(0),
m_counter(0),
m_stringData(){


}

void BinarySegment::setUpdateFlag(bool isRel){

	m_addrStartIndex = m_data.size();
	m_updateAddress = true;
	m_isRel = isRel;

}
void BinarySegment::push_back(uint8_t byte){

	m_data.push_back(byte);
}

unsigned int BinarySegment::getCounter(){
	return m_counter;

}
void BinarySegment::setCounter(unsigned int c){
	m_counter = c;
}

void BinarySegment::setConstant(Constant* c){
	m_addr = c;

}

Constant* BinarySegment::getConstant(){
	return m_addr;

}

bool BinarySegment::getUpdateFlag(){

	return m_updateAddress;
}
void BinarySegment::setAddrSize(AccessWidth aw){

	m_addrSize = aw;
}

AccessWidth& BinarySegment::getAddrSize(){

	return m_addrSize;
}

int BinarySegment::getAddrStartIndex(){
	return m_addrStartIndex;
}
uint8_t& BinarySegment::operator[](int index){
	return m_data[index];
}

int BinarySegment::size(){
	return m_data.size();
}

bool BinarySegment::getRelativeAddressFlag(){
	return m_isRel;

}
void BinarySegment::setStringData(std::string a){
	m_stringData = a;

}

std::string& BinarySegment::getStringData(){
	return m_stringData;
}





#include "data.h"
#include "gtest/gtest.h"

TEST(Registers,Initialization){
Register* pReg = new Register("AX",AccessMode::REG_DIRECT);

ASSERT_EQ(pReg->getBinEncoding(),0x00); 
delete pReg;
pReg = new Register("DP",AccessMode::REG_DIRECT);
ASSERT_EQ(pReg->getBinEncoding(),0xFF);
ASSERT_FALSE(Register::exists("AXI"));

}

int main(int argc,char **argv){
testing::InitGoogleTest(&argc,argv);
return RUN_ALL_TESTS();

}

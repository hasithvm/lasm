TARGET=lasm
SRC=symtable.cpp data.cpp lex.yy.c data_immediate.cpp p86asm.tab.c common.cpp Nodes.cpp BinarySegment.cpp p86Assembler.cpp preprocessor.cpp lasm.cpp VirgoWriter.cpp ListingWriter.cpp
OBJ=$(SRC:.cpp=.o)
CXXFLAGS=-g
CCFLAGS=-g
all:$(TARGET)

lasm: $(OBJ) 
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) 
				
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.o : %.c
	$(CC) -c $(CCFLAGS) $<


p86asm.l: p86asm.l.inc opcodes.dat
	python update-opcodes.py


lex.yy.c :p86asm.l
	flex -i p86asm.l

		
p86asm.tab.c:p86asm.y
		bison -d p86asm.y

clean:
			rm -rf $(TARGET) *.o lex.yy.c p86asm.tab.* p86asm.l symtable-generated.hpp


depend: .depend

.depend: $(SRC)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

TARGET=lasm
SRC=symtable.cpp data.cpp lex.yy.c data_immediate.cpp p86asm.tab.c common.cpp Nodes.cpp BinarySegment.cpp p86Assembler.cpp preprocessor.cpp lasm.cpp VirgoWriter.cpp
OBJ=$(SRC:.cpp=.o)
CXXFLAGS=-std=c++0x -g 
all:$(TARGET)

lasm: $(OBJ) 
		g++ -g $(CXXFLAGS) $(OBJ) -o $(TARGET) 
				
%.o : %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $<

%.o : %.c
	$(CC) -c $(CCFLAGS) -c $<

lex.yy.c :p86asm.l
	flex p86asm.l

		
p86asm.tab.c:p86asm.y
		bison -d p86asm.y

clean:
			rm -rf $(TARGET) *.o lex.yy.c p86asm.tab.*



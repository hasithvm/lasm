TARGET=p86asm
SRC=symtable.cpp data.cpp lex.yy.c
OBJ=$(SRC:.cpp=.o)
all:$(TARGET)

p86asm: flex bison $(OBJ) 
		g++ -g p86asm.tab.c $(SRC) -ll -o $(TARGET) 
				
%.o : %.cpp %.c
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $<

flex:p86asm.l
	flex p86asm.l

		
bison:p86asm.y
		bison -d p86asm.y

clean:
			rm -rf $(TARGET) *.o lex.yy.c y.tab.*


testlex: p86asm.l
				flex p86asm.l
				gcc lex.yy.c -o test_lexer -lfl

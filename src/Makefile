TARGET=p86asm
SRC=symtable.cpp data.cpp lex.yy.c data_immediate.cpp p86asm.tab.c
OBJ=$(SRC:.cpp=.o)
all:$(TARGET)

p86asm: $(OBJ) 
		g++ -g $(SRC) -ll -o $(TARGET) 
				
%.o : %.cpp
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $<

%.o : %.c
	$(CC) -c $(CCFLAGS)  -ll $<

lex.yy.c :p86asm.l
	flex p86asm.l

		
p86asm.tab.c:p86asm.y
		bison -d p86asm.y

clean:
			rm -rf $(TARGET) *.o lex.yy.c y.tab.*



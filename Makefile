TARGET=p86asm

all:$(TARGET)

p86asm: p86asm.l p86asm.y
			 flex p86asm.l
				bison -d p86asm.y
				g++ -g p86asm.tab.c lex.yy.c symtable.cpp data.cpp -ll -o $(TARGET) 
				

clean:
			rm -rf $(TARGET) *.o lex.yy.c y.tab.*


testlex: p86asm.l
				flex p86asm.l
				gcc lex.yy.c -o test_lexer -lfl

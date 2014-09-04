OBJDIR=.
BINDIR=.

MKDIR=mkdir -p
TARGET=$(BINDIR)/lasm
SRC=symtable.cpp data.cpp lex.yy.cpp data_immediate.cpp p86asm.tab.cpp common.cpp Nodes.cpp BinarySegment.cpp p86Assembler.cpp preprocessor.cpp lasm.cpp VirgoWriter.cpp ListingWriter.cpp
OBJ=$(addprefix $(OBJDIR)/,$(SRC:.cpp=.o))
DEPS=$(addprefix $(OBJDIR)/,$(SRC:.cpp=.d))
CXXFLAGS=
CCFLAGS=
all:$(TARGET)

$(TARGET): $(OBJ) 
	$(MKDIR) $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@
				
$(OBJDIR)/%.o : %.cpp
	$(MKDIR) $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o : %.c
	$(CC) -c $(CCFLAGS) $<


p86asm.l: p86asm.l.inc opcodes.dat
	python update-opcodes.py


lex.yy.cpp :p86asm.l
	flex -o $@ -i p86asm.l

		
p86asm.tab.cpp:p86asm.y
	bison -o $@ -d p86asm.y

clean:
	echo $(OBJ)
	rm -rf $(TARGET) $(OBJ) lex.yy.c p86asm.tab.* p86asm.l symtable-generated.hpp $(DEPS)


depend: $(DEPS)

$(OBJDIR)/%.d: %.cpp
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) -MM $< -MG -MT $(OBJDIR)/$(<:.cpp=.o) -MF $@

include $(DEPS)

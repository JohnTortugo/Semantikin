
###### Of course this should be improved...

SOURCES = 	Parsin.tab.cc lex.yy.cc Driver.cpp main.cpp AbstractSyntaxTree.cpp AstToDotVisitor.cpp \
			SymbolTable.cpp AstSemaVisitor.cpp AstTACGenVisitor.cpp IR.cpp ErrorReporting.cpp	\
			ControlFlowGraph.cpp BasicBlock.cpp
OBJS1	= $(SOURCES:.cc=.o)
OBJS	= $(OBJS1:.cpp=.o)

all: $(OBJS)
	g++ -g -std=c++0x $(OBJS) -o Semantikin

%.o: %.cpp
	g++ -g -c -std=c++0x $<
	
%.o: %.cc
	g++ -g -c -std=c++0x $<

lex.yy.cc: Lexin.l
	flex++ Lexin.l

Parsin.tab.cc: Parsin.yy
	bison -d Parsin.yy
	
test:
	./Semantikin ${INPUT}
	
view-ast:
	dot -Tpng Semantikin.dot -o Semantikin.png
	geeqie &
	
clean:
	rm -rf Semantikin Parsin.tab.cc Parsin.tab.hh Parsin.output location.hh position.hh stack.hh lex.yy.cc *.o tmp Semantikin.dot Semantiking.png

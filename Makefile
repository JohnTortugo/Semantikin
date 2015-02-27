SOURCES 	= 	Parsin.tab.cc \
				lex.yy.cc \
				Driver.cpp \
				main.cpp \
				AbstractSyntaxTree.cpp \
				SymbolTable.cpp \
				AstSemaVisitor.cpp \
				IR.cpp \
				ErrorReporting.cpp \
				AstToDotVisitor.cpp \
				AstTACGenVisitor.cpp \
				ControlFlowGraph.cpp \
				BasicBlock.cpp
							
OBJS1		= $(SOURCES:.cc=.o)
OBJS		= $(OBJS1:.cpp=.o)

CFGS		= $(shell ls "cfg_"*".dot" 2> /dev/null)
CFGS_FIGS	= $(CFGS:.dot=.png)

ASTS		= $(shell ls "ast_"*".dot" 2> /dev/null)
AST_FIGS	= $(ASTS:.dot=.png)

all: $(OBJS)
	g++ -O0 -g -std=c++0x $(OBJS) -o Semantikin

%.o: %.cpp
	g++ -O0 -g -c -std=c++0x $<
	
%.o: %.cc
	g++ -O0 -g -c -std=c++0x $<

%.png: %.dot
	dot -Tpng $< -o $@

lex.yy.cc: Lexin.l
	flex++ Lexin.l

Parsin.tab.cc: Parsin.yy
	bison -d Parsin.yy
	
test:
	./Semantikin ${INPUT}
	
view-ast: $(AST_FIGS)
	geeqie &

view-cfg: $(CFGS_FIGS)
	geeqie &

clean:
	rm -rf Semantikin Parsin.tab.cc Parsin.tab.hh Parsin.output location.hh position.hh stack.hh lex.yy.cc *.o tmp *.dot *.png

###### Of course this should be improved...
	
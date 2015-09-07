
###### Of course this should be improved...

ASTS_DOT	= 	$(shell find $(SOURCEDIR) -name 'ast_*.dot')
CFGS_DOT	= 	$(shell find $(SOURCEDIR) -name 'cfg_*.dot')
ASTS_PNG	= 	$(ASTS_DOT:.dot=.png)
CFGS_PNG	= 	$(CFGS_DOT:.dot=.png)

SOURCES = $(shell find . -regextype sed -regex '\..*\(\.cpp\|\.cc\)')
PAR_FIR = Parsin.tab.cc Parsin.tab.o lex.yy.cc lex.yy.o
OBJS1	= $(SOURCES:.cc=.o)
OBJS	= $(OBJS1:.cpp=.o)

all: $(PAR_FIR) $(OBJS)
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
	
%.png:%.dot
	dot -Tpng $< -o $@

view-cfg:$(CFGS_PNG)


view-ast:$(ASTS_PNG)


clean:
	rm -rf Semantikin Parsin.tab.cc Parsin.tab.hh Parsin.output location.hh position.hh stack.hh lex.yy.cc tmp *.dot *.png *.o

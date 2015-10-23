ASTS_DOT	= 	$(shell find $(SOURCEDIR) -name 'ast_*.dot')
CFGS_DOT	= 	$(shell find $(SOURCEDIR) -name 'cfg_*.dot')
ASTS_PNG	= 	$(ASTS_DOT:.dot=.png)
CFGS_PNG	= 	$(CFGS_DOT:.dot=.png)

CFLAGS	= -g -std=c++0x 

# All source files
SOURCES = $(shell find . -regextype sed -regex '\..*\(\.cpp\|\.cc\)')

# Parsing objects
PAR_FIR = Parsin.tab.cc Parsin.tab.o lex.yy.cc lex.yy.o
OBJS1	= $(SOURCES:.cc=.o)
OBJS	= $(OBJS1:.cpp=.o)

all: $(PAR_FIR) $(OBJS) CLANG_COMPLETE
	g++ $(CFLAGS) $(OBJS) -o Semantikin

%.o: %.cpp
	g++ -c $(CFLAGS) $<
	
%.o: %.cc
	g++ -c $(CFLAGS) $<

lex.yy.cc: Lexin.l
	flex++ Lexin.l

Parsin.tab.cc: Parsin.yy
	bison -d Parsin.yy

rebuild-run: CLEAR clean all
	./Semantikin -i tests/latest.lxn -dumps -dumpIRTree && dot -Tpng tests/latest.lxn.dot > tests/latest.lxn.dot.png

run: CLEAR all
	./Semantikin -i tests/latest.lxn -dumps -dumpIRTree && dot -Tpng tests/latest.lxn.dot > tests/latest.lxn.dot.png

tests: all
	./tests/regression/run_regression.sh

%.png:%.dot
	dot -Tpng $< -o $@

view-cfg:$(CFGS_PNG)


view-ast:$(ASTS_PNG)

clean:
	rm -rf Semantikin Parsin.tab.cc Parsin.tab.hh Parsin.output location.hh position.hh stack.hh lex.yy.cc tmp *.dot *.png *.o
	find . -name "*.dot" -type f -delete
	find . -name "*.png" -type f -delete



###############################################################################
######### Just to clear the screen
CLEAR:
	#@clear



###############################################################################
######### This will update the .clang_complete file with all sources and CFLAGS
CLANG_COMPLETE:
	$(shell echo -std=c++11 > .clang_complete)
	$(foreach src, $(SOURCES), $(shell echo -include $(src) >> .clang_complete))
	$(foreach flg, $(CFLAGS), $(shell echo $(flg) >> .clang_complete))

define print_src
	@echo -include $(1)

endef

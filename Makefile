all:
	#clear
	flex++ Lexin.l
	bison -d Parsin.yy
	g++ -g -std=c++0x lex.yy.cc Parsin.tab.cc Driver.cpp main.cpp AbstractSyntaxTree.cpp AstToDotVisitor.cpp SymbolTable.cpp AstSemaVisitor.cpp -o Semantikin
	./Semantikin ../Lexin/tests/fatorial.lxn
	#dot -Tpng Semantikin.dot -o Semantikin.png
	#geeqie &
	
clean:
	rm Semantikin Parsin.tab.cc Parsin.tab.hh Parsin.output location.hh position.hh stack.hh lex.yy.cc Semantikin.dot Semantiking.png

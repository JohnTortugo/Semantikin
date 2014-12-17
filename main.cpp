#include "Semantikin.h"
#include "Driver.h"
#include "AstVisitors.h"
#include <iostream>

int main(const int argc, const char **argv) {
   Parser::Driver driver;
   Parser::CompilationUnit* astModule = nullptr;

   if (driver.parse(argv[1], astModule)) {
	   AstToDotVisitor ast("Semantikin.dot");
	   astModule->accept(&ast);

	   AstSemaVisitor semantic;
	   astModule->accept(&semantic);

	   AstTACGenVisitor irgen(semantic.currentOffset());
	   astModule->accept(&irgen);

	   delete astModule;

	   shared_ptr<IR::Module> irModule = irgen.module();

	   irModule->dump();
   }
   else {
	   /* Parsing error. */
   }

   return 0;
}

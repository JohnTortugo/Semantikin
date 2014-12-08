#include "Driver.h"
#include "AstVisitors.h"
#include <iostream>
#include <sstream>

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

	   shared_ptr<IR::Module> irModule = irgen.module();

	   std::stringstream buffer;
	   irModule->dump(buffer);

	   cout << "This is the IR: " << buffer.str();
   }
   else {
	   std::cout << "Parsing error!" << std::endl;
   }

   delete astModule;

   return 0;
}

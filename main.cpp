#include <iostream>

#include "AstVisitors.h"
#include "Driver.h"

int main(const int argc, const char **argv) {
   Parser::Driver driver;
   Parser::CompilationUnit* module = nullptr;

   int status = driver.parse(argv[1], module);

   if (status == 0) {
	   AstToDotVisitor ast("Semantikin.dot");
	   module->accept(&ast);

	   AstSemaVisitor semantic;
	   module->accept(&semantic);
   }
   else {
	   std::cout << "Something went wrong bro." << std::endl;
   }

   delete module;

   return 0;
}

#include "Semantikin.h"
#include "Driver.h"
#include "AstVisitors.h"
#include <iostream>

int main(const int argc, const char **argv) {
   Parser::Driver driver;
   Parser::CompilationUnit* astModule = nullptr;

   /* Parsing. */
   if (!driver.parse(argv[1], astModule)) return -1;

   /* Do semantic analyzis. */
   AstSemaVisitor semantic;
   astModule->accept(&semantic);

   /* Generate TAC-SSA IR. */
//   AstTACGenVisitor irgen(semantic.currentOffset());
//   astModule->accept(&irgen);
//
//   /* Obtain pointer to the IR generated module. */
//   shared_ptr<IR::Module> irModule = irgen.module();
//
//
//   // Just print out the AST of the function
//   for (auto& func : *astModule->getFunctions()) {
//	   AstToDotVisitor ast("ast_" + func->getName() + ".dot");
//	   func->accept(&ast);
//   }
//
//
//   // print out the cfg of the function
//   for (auto& func : *irModule->functions()) {
//	   auto cfg = func->cfg();
//   	   cfg->dumpToDot("cfg_" + func->addr()->getName() + ".dot");
//	}


   // Just dump the IR
// irModule->dump();

   return 0;
}

#include <fstream>

#include "Driver.h"

Parser::Driver::~Driver(){
   delete(scanner);
   scanner = nullptr;

   delete(parser);
   parser = nullptr;
}

int Parser::Driver::parse(const char *filename, Parser::CompilationUnit*& module) {
   std::ifstream in_file(filename);

   scanner = new Parser::FlexScanner(&in_file);

   parser = new Parser::BisonParser(*scanner, *this, module);

   /* Returns 1 if parsing went OK. */
   return !parser->parse();
}

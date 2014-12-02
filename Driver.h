#ifndef __DRIVER_H_
#define __DRIVER_H_

#include "FlexScanner.h"
#include "Parsin.tab.hh"
#include "AbstractSyntaxTree.h"

namespace Parser {

	class Driver {
		private:
		   Parser::BisonParser *parser;
		   Parser::FlexScanner *scanner;

		public:
		   Driver() : parser(nullptr), scanner(nullptr) {};

		   virtual ~Driver();

		   int parse(const char *filename, Parser::CompilationUnit*& module);
	};

}

#endif

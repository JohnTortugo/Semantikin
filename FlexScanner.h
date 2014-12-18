#ifndef __FlexScanner__
#define __FlexScanner__ 1

#ifndef yyFlexLexerOnce
	#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL int Parser::FlexScanner::yylex()

#include "Parsin.tab.hh"
#include "AbstractSyntaxTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iomanip>

namespace Parser {
	class FlexScanner : public yyFlexLexer {
		private:
			int yylex();

			Parser::BisonParser::semantic_type *tokenValue;
			Parser::BisonParser::location_type *location;
			std::string _currentLine;

		public:

			FlexScanner(std::istream *in) : yyFlexLexer(in), tokenValue(nullptr), location(nullptr), _currentLine("") {}

			string currentLine() const { return this->_currentLine; }

			int yylex(Parser::BisonParser::semantic_type *_tokenValue, Parser::BisonParser::location_type *_location) {
				tokenValue = _tokenValue;
				location = _location;
				return yylex();
			}
	};

}

#endif

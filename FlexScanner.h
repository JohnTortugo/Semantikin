#ifndef __FlexScanner__
#define __FlexScanner__ 1

#ifndef yyFlexLexerOnce
	#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL int Parser::FlexScanner::yylex()

#include "Parsin.tab.hh"
#include "AbstractSyntaxTree.h"

namespace Parser {
	class FlexScanner : public yyFlexLexer {
		private:
			int yylex();

			Parser::BisonParser::semantic_type *tokenValue;

		public:
			FlexScanner(std::istream *in) : yyFlexLexer(in), tokenValue(nullptr) {}

			int yylex(Parser::BisonParser::semantic_type *_tokenValue) {
				tokenValue = _tokenValue;
				return yylex();
			}
	};
}

#endif

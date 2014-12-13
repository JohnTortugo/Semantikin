%option c++
%option yyclass="FlexScanner"
%option noyywrap 
%option nodefault 
%option yylineno

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "FlexScanner.h"
	#include "Parsin.tab.hh"
%}

%%
[ \t]*			{ }
[\n]*			{ }
[\/][\/].*		{ }

"if"			{ return Parser::BisonParser::token::TK_IF; }
"else"			{ return Parser::BisonParser::token::TK_ELSE; }
"elseif"		{ return Parser::BisonParser::token::TK_ELSEIF; }
"while"			{ return Parser::BisonParser::token::TK_WHILE; }
"return"		{ return Parser::BisonParser::token::TK_RETURN; }

";"				{ return Parser::BisonParser::token::TK_SEMICOLON; }
","				{ return Parser::BisonParser::token::TK_COMMA; }
"\""			{ return Parser::BisonParser::token::TK_DOUBLE_QUOTE; }
"\'"			{ return Parser::BisonParser::token::TK_SINGLE_QUOTE; }
"&"				{ return Parser::BisonParser::token::TK_AMPERSAND; }
"|"				{ return Parser::BisonParser::token::TK_BIT_OR; }
"^"				{ return Parser::BisonParser::token::TK_BIT_XOR; }
"~"				{ return Parser::BisonParser::token::TK_BIT_NOT; }

"("				{ return Parser::BisonParser::token::TK_L_PAREN; }
")" 			{ return Parser::BisonParser::token::TK_R_PAREN; }
"{" 			{ return Parser::BisonParser::token::TK_L_BRACE; }
"}" 			{ return Parser::BisonParser::token::TK_R_BRACE; }
"[" 			{ return Parser::BisonParser::token::TK_L_SQBRACE; }
"]" 			{ return Parser::BisonParser::token::TK_R_SQBRACE; }


"&&"			{ return Parser::BisonParser::token::TK_AND; }
"||"			{ return Parser::BisonParser::token::TK_OR; }
"!"				{ return Parser::BisonParser::token::TK_NOT; }
"="				{ return Parser::BisonParser::token::TK_ASSIGN; }
"=="			{ return Parser::BisonParser::token::TK_COMPARE; }
"<"				{ return Parser::BisonParser::token::TK_LT; }
"<="			{ return Parser::BisonParser::token::TK_LTE; }
">"				{ return Parser::BisonParser::token::TK_GT; }
">="			{ return Parser::BisonParser::token::TK_GTE; }

"+"				{ return Parser::BisonParser::token::TK_PLUS; }
"-"				{ return Parser::BisonParser::token::TK_MINUS; }
"*"				{ return Parser::BisonParser::token::TK_TIMES; }
"/"				{ return Parser::BisonParser::token::TK_DIV; }
"%"				{ return Parser::BisonParser::token::TK_MOD; }
"++"			{ return Parser::BisonParser::token::TK_DPLUS; }
"--"			{ return Parser::BisonParser::token::TK_DMINUS; }
"+="			{ return Parser::BisonParser::token::TK_PLUS_EQUAL; }
"-="			{ return Parser::BisonParser::token::TK_MINUS_EQUAL; }
"*="			{ return Parser::BisonParser::token::TK_TIMES_EQUAL; }
"/="			{ return Parser::BisonParser::token::TK_DIV_EQUAL; }
"%="			{ return Parser::BisonParser::token::TK_MOD_EQUAL; }

\"[^\"]*\"		{ tokenValue->str = strdup(yytext); 	return Parser::BisonParser::token::TK_STRING;	}
[0-9][0-9]*		{ tokenValue->integer = atoi(yytext); 	return Parser::BisonParser::token::TK_INTEGER; }
[0-9]+\.[0-9]*	{ tokenValue->floating = atof(yytext); 	return Parser::BisonParser::token::TK_FLOAT; 	}
[0-9a-zA-Z]*	{ tokenValue->str = strdup(yytext); 	return Parser::BisonParser::token::TK_ID; 		}

.				{ printf("Unrecognized token '%s'\n", yytext); }
<<EOF>>			{ return 0; }


%%
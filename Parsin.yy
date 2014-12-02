%skeleton "lalr1.cc"
%require "3.0"
%defines
%locations
%define api.namespace {Parser}
%define parser_class_name {BisonParser}

%code requires{
   namespace Parser {
      class Driver;
      class FlexScanner;
   }
}

%lex-param {FlexScanner &scanner}
%lex-param {Driver &driver}

%parse-param {FlexScanner &scanner}
%parse-param {Driver &driver}
%parse-param {CompilationUnit*& module}

%code requires {
	#include "AbstractSyntaxTree.h"
}

%{
	#include <cstdio>
	#include <iostream>
	#include <memory>
	#include <string>
	#include "Driver.h"
	#include "FlexScanner.h"

	void Parser::BisonParser::error(const location_type& loc, const std::string& msg) {
		std::cerr << "Location was <Line, Col> = <" << loc.begin.line << "," << loc.begin.column << ">\n";
	}
	
	static int yylex(Parser::BisonParser::semantic_type *tokenVal, Parser::BisonParser::location_type *location, Parser::FlexScanner &scanner, Parser::Driver &driver) {
		return scanner.yylex(tokenVal);	
	}
%}


%union {
	int integer;
	float floating;
	double doubling;
	char* str;
	Expression* expr;
	list<std::shared_ptr<Expression>>* listExpr;
	FunctionCall* funCall;
	ReturnStmt* ret;
	list<std::shared_ptr<ElseIfStmt>>* listElseIf;
	list<std::shared_ptr<Statement>>* listStmts;
	CodeBlock* codeBlock;
	IfStmt* ifStmt;
	LoopStmt* loopStmt;
	list<std::shared_ptr<VarSpec>>* listVarSpec;
	VarDecl* varDecl;
	list<std::shared_ptr<ParamDecl>>* formParams;
	Function* function;
	list<std::shared_ptr<Function>>* listFuncs;
	CompilationUnit* module;
}

%type <expr> EXPR;
%type <funCall> FUN_CALL;
%type <listExpr> FORM_PARAMS MFORM_PARAMS MAT_DIMS;
%type <ret> RETURN;
%type <listElseIf> COND_ELSES;
%type <listStmts> CODE;
%type <codeBlock> CODE_BLOCK;
%type <ifStmt> COND;
%type <loopStmt> LOOP;
%type <listVarSpec> MULT_VARS_DECL;
%type <varDecl> VAR_DECL;
%type <formParams> MARGS ARGS;
%type <function> FUNCTION;
%type <listFuncs> FUNCTIONS;
%type <module> MODULE;

%token TK_IF
%token TK_ELSE			
%token TK_ELSEIF		
%token TK_WHILE			
%token TK_RETURN		

%token <str> TK_STRING
%token <integer> TK_INTEGER
%token <floating> TK_FLOAT
%token <str> TK_ID

%token TK_SEMICOLON
%token TK_COMMA			
%token TK_DOUBLE_QUOTE	
%token TK_SINGLE_QUOTE	
%token TK_AMPERSAND
%token TK_BIT_OR
%token TK_BIT_XOR

%token TK_L_PAREN		
%token TK_R_PAREN		
%token TK_L_BRACE		
%token TK_R_BRACE		
%token TK_L_SQBRACE		
%token TK_R_SQBRACE		

%token TK_AND
%token TK_OR
%token TK_NOT
%token TK_ASSIGN		
%token TK_COMPARE		
%token TK_LT			
%token TK_LTE			
%token TK_GT			
%token TK_GTE			

%token TK_PLUS			
%token TK_MINUS			
%token TK_TIMES			
%token TK_DIV		
%token TK_MOD			
%token TK_DPLUS			
%token TK_DMINUS		
%token TK_PLUS_EQUAL	
%token TK_MINUS_EQUAL	
%token TK_TIMES_EQUAL	
%token TK_DIV_EQUAL	
%token TK_MOD_EQUAL		

%left TK_COMPARE TK_ASSIGN
%left TK_LT TK_LTE TK_GT TK_GTE
%left TK_MINUS_EQUAL TK_PLUS_EQUAL
%left TK_DIV_EQUAL TK_TIMES_EQUAL
%left TK_MOD_EQUAL
%left TK_AND TK_OR TK_BIT_OR TK_BIT_XOR
%left TK_PLUS TK_MINUS
%left TK_DIV TK_TIMES
%left TK_MOD
%left TK_DPLUS TK_DMINUS
%left TK_NOT
%left TK_AMPERSAND


%start MODULE

%%

MODULE			: FUNCTIONS																	{ module = new CompilationUnit($1); }
				;

FUNCTIONS		: FUNCTION FUNCTIONS														{ ($2)->push_front(std::shared_ptr<Function>($1)); $$ = $2; }
				|																			{ $$ = new list<std::shared_ptr<Function>>(); }
				;

FUNCTION		: TK_ID TK_ID TK_L_PAREN ARGS TK_R_PAREN CODE_BLOCK							{ $$ = new Function($1, $2, shared_ptr<list<shared_ptr<ParamDecl>>>($4), $6); }
				;

ARGS			: TK_ID TK_ID MAT_DIMS MARGS												{ ($4)->push_front(std::shared_ptr<ParamDecl>(new ParamDecl($1, $2, shared_ptr<list<shared_ptr<Expression>>>($3)))); $$ = $4; }
				|																			{ $$ = new list<std::shared_ptr<ParamDecl>>(); }
				;

MARGS			: TK_COMMA TK_ID TK_ID MAT_DIMS MARGS										{ ($5)->push_front(std::shared_ptr<ParamDecl>(new ParamDecl($2, $3, shared_ptr<list<shared_ptr<Expression>>>($4)))); $$ = $5; }
				|																			{ $$ = new list<std::shared_ptr<ParamDecl>>(); }
				;

CODE_BLOCK		: TK_L_BRACE CODE TK_R_BRACE												{ $$ = new CodeBlock($2); };

CODE			: VAR_DECL TK_SEMICOLON CODE												{ ($3)->push_front(std::shared_ptr<Statement>($1)); $$ = $3; }
				| LOOP CODE																	{ ($2)->push_front(std::shared_ptr<Statement>($1)); $$ = $2; }
				| COND CODE																	{ ($2)->push_front(std::shared_ptr<Statement>($1)); $$ = $2; }
				| RETURN TK_SEMICOLON CODE													{ ($3)->push_front(std::shared_ptr<Statement>($1)); $$ = $3; }
				| EXPR TK_SEMICOLON CODE													{ ($3)->push_front(std::shared_ptr<Statement>($1)); $$ = $3; }
				|																			{ $$ = new list<std::shared_ptr<Statement>>(); }
				;

VAR_DECL		: TK_ID TK_ID MAT_DIMS MULT_VARS_DECL										{ ($4)->push_front(std::shared_ptr<VarSpec>(new VarSpec($2, $3, nullptr))); $$ = new VarDecl($1, $4); }
				| TK_ID TK_ID TK_ASSIGN EXPR MULT_VARS_DECL									{ ($5)->push_front(std::shared_ptr<VarSpec>(new VarSpec($2, nullptr, $4))); $$ = new VarDecl($1, $5); }
				;

MULT_VARS_DECL	: TK_COMMA TK_ID MAT_DIMS MULT_VARS_DECL									{ ($4)->push_front(std::shared_ptr<VarSpec>(new VarSpec($2, $3, nullptr))); $$ = $4; }
				| TK_COMMA TK_ID TK_ASSIGN EXPR MULT_VARS_DECL								{ ($5)->push_front(std::shared_ptr<VarSpec>(new VarSpec($2, nullptr, $4))); $$ = $5; }
				|																			{ $$ = new list<std::shared_ptr<VarSpec>>(); }
				;

MAT_DIMS		: TK_L_SQBRACE EXPR TK_R_SQBRACE MAT_DIMS									{ ($4)->push_front(std::shared_ptr<Expression>($2)); $$ = $4; }
				|																			{ $$ = new list<std::shared_ptr<Expression>>(); }
				;

FUN_CALL		: TK_ID TK_L_PAREN FORM_PARAMS TK_R_PAREN									{ $$ = new FunctionCall($1, $3); }
				;

LOOP			: TK_WHILE TK_L_PAREN EXPR TK_R_PAREN CODE_BLOCK							{ $$ = new LoopStmt($3, $5); }
				;

COND			: TK_IF TK_L_PAREN EXPR TK_R_PAREN CODE_BLOCK COND_ELSES					{ $$ = new IfStmt($3, $5, nullptr, $6); }
				| TK_IF TK_L_PAREN EXPR TK_R_PAREN CODE_BLOCK COND_ELSES TK_ELSE CODE_BLOCK	{ $$ = new IfStmt($3, $5, $8, $6); }
				;

COND_ELSES		: TK_ELSEIF TK_L_PAREN EXPR TK_R_PAREN CODE_BLOCK COND_ELSES				{ ($6)->push_front(std::shared_ptr<ElseIfStmt>(new ElseIfStmt($3, $5))); $$ = $6; }
				|																			{ $$ = new list<std::shared_ptr<ElseIfStmt>>(); }
				;

RETURN			: TK_RETURN EXPR															{ $$ = new ReturnStmt($2); }
				;

FORM_PARAMS		: EXPR MFORM_PARAMS															{ ($2)->push_front(std::shared_ptr<Expression>($1)); $$ = $2; }
				|																			{ $$ = new list<std::shared_ptr<Expression>>(); }
				;

MFORM_PARAMS	: TK_COMMA EXPR MFORM_PARAMS												{ ($3)->push_front(std::shared_ptr<Expression>($2)); $$ = $3; }
				|																			{ $$ = new list<std::shared_ptr<Expression>>(); }
				;

EXPR			: EXPR TK_COMPARE EXPR														{ $$ = new BinaryExpr(BinaryExpr::COMPARE, $1, $3); }
				| EXPR TK_ASSIGN EXPR														{ $$ = new BinaryExpr(BinaryExpr::ASSIGN, $1, $3); }
				| EXPR TK_AND EXPR															{ $$ = new BinaryExpr(BinaryExpr::LOG_AND, $1, $3); }
				| EXPR TK_OR EXPR															{ $$ = new BinaryExpr(BinaryExpr::LOG_OR, $1, $3); }
				| EXPR TK_AMPERSAND EXPR													{ $$ = new BinaryExpr(BinaryExpr::BIT_AND, $1, $3); }
				| EXPR TK_BIT_OR EXPR														{ $$ = new BinaryExpr(BinaryExpr::BIT_OR, $1, $3); }
				| EXPR TK_BIT_XOR EXPR														{ $$ = new BinaryExpr(BinaryExpr::BIT_XOR, $1, $3); }
				| EXPR TK_LT EXPR															{ $$ = new BinaryExpr(BinaryExpr::LT, $1, $3); }
				| EXPR TK_LTE EXPR															{ $$ = new BinaryExpr(BinaryExpr::LTE, $1, $3); }
				| EXPR TK_GT EXPR															{ $$ = new BinaryExpr(BinaryExpr::GT, $1, $3); }
				| EXPR TK_GTE EXPR															{ $$ = new BinaryExpr(BinaryExpr::GTE, $1, $3); }
				| EXPR TK_PLUS EXPR															{ $$ = new BinaryExpr(BinaryExpr::ADDITION, $1, $3); }
				| EXPR TK_MINUS EXPR														{ $$ = new BinaryExpr(BinaryExpr::SUBTRACTION, $1, $3); }
				| EXPR TK_TIMES EXPR														{ $$ = new BinaryExpr(BinaryExpr::TIMES, $1, $3); }
				| EXPR TK_DIV EXPR															{ $$ = new BinaryExpr(BinaryExpr::DIV, $1, $3); }
				| EXPR TK_MOD EXPR															{ $$ = new BinaryExpr(BinaryExpr::MOD, $1, $3); }
				| EXPR TK_PLUS_EQUAL EXPR													{ $$ = new BinaryExpr(BinaryExpr::PLUS_EQUAL, $1, $3); }
				| EXPR TK_MINUS_EQUAL EXPR													{ $$ = new BinaryExpr(BinaryExpr::MINUS_EQUAL, $1, $3); }
				| EXPR TK_TIMES_EQUAL EXPR													{ $$ = new BinaryExpr(BinaryExpr::TIMES_EQUAL, $1, $3); }
				| EXPR TK_DIV_EQUAL EXPR													{ $$ = new BinaryExpr(BinaryExpr::DIV_EQUAL, $1, $3); }
				| EXPR TK_MOD_EQUAL EXPR													{ $$ = new BinaryExpr(BinaryExpr::MOD_EQUAL, $1, $3); }
				| TK_L_PAREN EXPR TK_R_PAREN												{ $$ = $2; }
				| TK_NOT EXPR 																{ $$ = new UnaryExpr(UnaryExpr::NOT, $2); }
				| TK_MINUS EXPR 															{ $$ = new UnaryExpr(UnaryExpr::MINUS, $2); }
				| TK_PLUS EXPR 																{ $$ = new UnaryExpr(UnaryExpr::PLUS, $2); }
				| TK_AMPERSAND EXPR 														{ $$ = new UnaryExpr(UnaryExpr::ADDR, $2); }
				| EXPR TK_DPLUS																{ $$ = new UnaryExpr(UnaryExpr::INCREMENT, $1); }
				| EXPR TK_DMINUS															{ $$ = new UnaryExpr(UnaryExpr::DECREMENT, $1); }
				| TK_INTEGER																{ $$ = new IntegerExpr($1); }
				| TK_FLOAT																	{ $$ = new FloatExpr($1); }
				| TK_STRING																	{ $$ = new StringExpr($1); }
				| TK_ID MAT_DIMS															{ $$ = new IdentifierExpr($1, $2); }
				| FUN_CALL																	{ $$ = $1; }
				;

%%



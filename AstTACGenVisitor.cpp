#include "AstVisitors.h"
#include "SymbolTable.h"
#include <memory>
#include <list>

using namespace Parser;

void AstTACGenVisitor::visit(Parser::CompilationUnit* module) {
	if (module->getSymbTable() == nullptr) {
		cout << "IRGen error:" << endl;
		cout << "\tThe SymbolTable for this module has not been computed!" << endl;
		return ;
	}

	/* Create a new TAC IR module. */
	this->_module = shared_ptr<IR::Module>(new IR::Module());

	/* The IR module owns the symbol table. */
	this->_module->symbolTable(module->getSymbTable());

	/* Continue visiting children. */
	for (auto function : *module->getFunctions()) {
		/* Something like an inherited attribute. */
		this->_currentFunction = shared_ptr<IR::Function>(new IR::Function(function->getSymbTable()));

		/* Produce IR instructions for the components of function. */
		function->accept(this);

		/* Add the function with its IR instructions to the module. */
		this->_module->addFunction( shared_ptr<IR::Function>( this->_currentFunction ) );
	}
}

void AstTACGenVisitor::visit(Parser::Function* function) {
	if (function->getSymbTable() == nullptr) {
		cout << "IRGen error:" << endl;
		cout << "\tThe SymbolTable for this function has not been computed!" << endl;
		return ;
	}

	/* The IR function must assume the ownership of the symbol table. */
	this->_currentFunction->symbolTable(function->getSymbTable());

	/* Produce IR instructions for the statements in the function's code block. */
	function->getBody()->accept(this);
}

void AstTACGenVisitor::visit(const Parser::ParamDecl* param) { /* Nothing. */ }

void AstTACGenVisitor::visit(const Parser::VarSpec* spec) {
}

void AstTACGenVisitor::visit(const Parser::VarDecl* varDec) {
}

void AstTACGenVisitor::visit(const Parser::LoopStmt* loop) {

}

void AstTACGenVisitor::visit(const Parser::IfStmt* ifStmt) {

}

void AstTACGenVisitor::visit(const Parser::ElseIfStmt* elseIfStmt) {

}

void AstTACGenVisitor::visit(const Parser::ReturnStmt* ret) {

}

void AstTACGenVisitor::visit(Parser::CodeBlock* block) {
	if (block->getSymbTable() == nullptr) {
		cout << "IRGen error:" << endl;
		cout << "\tThe SymbolTable for this block has not been computed!" << endl;
		return ;
	}

	/* The IR function symbol table has all definitions of the function's blocks. */
	this->_currentFunction->addSymbolTable(block->getSymbTable());

	/* Produce IR for each statement in the code block. */
	for (auto statement : *block->getStatements())
		statement->accept(this);
}

void AstTACGenVisitor::visit(Parser::StringExpr* str) {

}

void AstTACGenVisitor::visit(Parser::FloatExpr* flt) {

}

void AstTACGenVisitor::visit(Parser::IntegerExpr* integer) {

}

void AstTACGenVisitor::visit(Parser::IdentifierExpr* id) {

}

void AstTACGenVisitor::visit(Parser::FunctionCall* funCall) {

}

void AstTACGenVisitor::visit(Parser::UnaryExpr* unary) {

}

void AstTACGenVisitor::visit(Parser::BinaryExpr* binop) {

}

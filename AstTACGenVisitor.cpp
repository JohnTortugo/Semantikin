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

		/* Reset the counters for temps/labels/constants. */
		this->constCounter = 1;
		this->labelCounter = 1;
		this->tempCounter = 1;

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

	/* We need to keep a pointer to the current function definition, right? */
	this->_currentFunction->addr( function->getSymbTable()->getParent()->lookup(function->getName()) );

	/* Produce IR instructions for the statements in the function's code block. */
	function->getBody()->accept(this);
}

void AstTACGenVisitor::visit(const Parser::ParamDecl* param) { /* Nothing. */ }

void AstTACGenVisitor::visit(const Parser::VarSpec* spec) {
}

void AstTACGenVisitor::visit(const Parser::VarDecl* varDec) {
	shared_ptr<SymbolTable> st = this->_currentFunction->symbolTable();

	for (auto spec : *varDec->getVars()) {
		Expression* initializer = spec->getInitializer();

		if (initializer != nullptr) {
			initializer->accept(this);

			this->_currentFunction->appendInstruction(shared_ptr<IR::ScalarCopy>(new IR::ScalarCopy(st->lookup(spec->getName()), initializer->addr())));
		}
	}
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
	shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), str->getValue()));

	this->_currentFunction->symbolTable()->add(cttEntry);

	str->addr(cttEntry);
}

void AstTACGenVisitor::visit(Parser::FloatExpr* flt) {
	shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), flt->getValue()));

	this->_currentFunction->symbolTable()->add(cttEntry);

	flt->addr(cttEntry);
}

void AstTACGenVisitor::visit(Parser::IntegerExpr* integer) {
	shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), integer->getValue()));

	this->_currentFunction->symbolTable()->add(cttEntry);

	integer->addr(cttEntry);
}

void AstTACGenVisitor::visit(Parser::IdentifierExpr* id) {
	/* No need to code gen. */
	/* Address was already computed in Sema. */
}

void AstTACGenVisitor::visit(Parser::FunctionCall* funCall) {

}

void AstTACGenVisitor::visit(Parser::UnaryExpr* unary) {

}

void AstTACGenVisitor::visit(Parser::BinaryExpr* binop) {
	Expression* exp1 = binop->getExp1();
	Expression* exp2 = binop->getExp2();
	IR::Instruction* newInstruction = nullptr;
	NativeType tgtType = binop->exprType();

	exp1->accept(this);
	exp2->accept(this);

	if (binop->exprType() == Parser::INT) {
		switch (binop->getType()) {
			case BinaryExpr::ASSIGN:
				newInstruction = new IR::ScalarCopy(exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::LOG_AND:
				newInstruction = new IR::LogAnd(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::LOG_OR:
				newInstruction = new IR::LogOr(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::BIT_AND:
				newInstruction = new IR::BinAnd(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::BIT_OR:
				newInstruction = new IR::BinOr(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::BIT_XOR:
				newInstruction = new IR::BinXor(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::COMPARE:
				newInstruction = new IR::REqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::LT:
				newInstruction = new IR::RLesThan(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::LTE:
				newInstruction = new IR::RLesThanEqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::GT:
				newInstruction = new IR::RGreaterThan(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::GTE:
				newInstruction = new IR::RGreaterThanEqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::ADDITION:
				newInstruction = new IR::IAdd(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::SUBTRACTION:
				newInstruction = new IR::ISub(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::TIMES:
				newInstruction = new IR::IMul(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::DIV:
				newInstruction = new IR::IDiv(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::MOD:
				newInstruction = new IR::IMod(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::PLUS_EQUAL:
				newInstruction = new IR::IAdd(exp1->addr(), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::MINUS_EQUAL:
				newInstruction = new IR::ISub(exp1->addr(), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::TIMES_EQUAL:
				newInstruction = new IR::IMul(exp1->addr(), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::DIV_EQUAL:
				newInstruction = new IR::IDiv(exp1->addr(), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::MOD_EQUAL:
				newInstruction = new IR::IMod(exp1->addr(), exp1->addr(), exp2->addr());
				break;
		}
	}

	binop->addr( newInstruction->tgt() );

	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
}

shared_ptr<STTempVar> AstTACGenVisitor::newTemporary(NativeType type) {
	int width  = AstSemaVisitor::typeWidth(type);
	int tempId = this->tempCounter++;
	int offset = this->_currentOffset += width;

	return shared_ptr<STTempVar>(new STTempVar("_t" + std::to_string(tempId), type, width, offset));
}

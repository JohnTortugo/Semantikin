#include "Semantikin.h"
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

	/* This label is just a sentinel pointer to convey the
	 * information that the expression should consider a fallthrough
	 * path. */
	this->_fallLabel = this->newLabel("fallthrough");

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

	/* The first instruction of every function needs to have a label. */
	this->_currentFunction->appendLabel( this->newLabel("entry") );

	/* Produce IR instructions for the statements in the function's code block. */
	function->getBody()->accept(this);
}

void AstTACGenVisitor::visit(const Parser::ParamDecl* param)
{ /* Nothing. */ }

void AstTACGenVisitor::visit(const Parser::VarSpec* spec) {
}

void AstTACGenVisitor::visit(const Parser::VarDecl* varDec) {
	shared_ptr<SymbolTable> st = this->_currentFunction->symbolTable();

	for (auto spec : *varDec->getVars()) {
		Expression* initializer = spec->getInitializer();

		if (initializer != nullptr) {
			initializer->accept(this);

			if (initializer->type() == Parser::INT || initializer->type() == Parser::FLOAT) {
				this->_currentFunction->appendInstruction(shared_ptr<IR::ScalarCopy>(new IR::ScalarCopy(st->lookup(spec->getName()), initializer->addr())));
			}
			else if (initializer->type() == Parser::STRING) {
				shared_ptr<IR::Call> newInstruction = shared_ptr<IR::Call>( new IR::Call(st->lookup(System::NAT_FUN_STRCPY), nullptr) );

				newInstruction->addArgument(st->lookup(spec->getName()));
				newInstruction->addArgument(initializer->addr());

				this->_currentFunction->appendInstruction(shared_ptr<IR::Call>(newInstruction));
			}
			else {
				throw System::EXCEPTION_UNEXPECTED_TYPE;
			}
		}
	}
}

void AstTACGenVisitor::visit(Parser::LoopStmt* loop) {
	auto begin 		= this->newLabel();
	auto condition 	= loop->getCondition();
	auto codeBlock 	= loop->getBody();

	condition->tLabel( this->_fallLabel );
	condition->fLabel( loop->next() );
	codeBlock->next( begin );

	this->_currentFunction->appendLabel(begin);
	condition->accept(this);
//	this->_currentFunction->appendLabel(condition->tLabel());
	codeBlock->accept(this);
	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(begin) ) );
}

void AstTACGenVisitor::visit(Parser::IfStmt* ifStmt) {
	auto condition = ifStmt->getCondition();
	auto thenBlock = ifStmt->getThenBlock();
	auto elseBlock = ifStmt->getElseBlock();
	list<shared_ptr<Parser::ElseIfStmt>>* elseIfChain = ifStmt->getElseIfChain();

	if ((elseIfChain == nullptr || elseIfChain->size() == 0) && elseBlock == nullptr) {
		/* Does not have else-if nor else blocks. */

		condition->tLabel( this->_fallLabel );
		condition->fLabel( ifStmt->next() );
		thenBlock->next( ifStmt->next() );

		condition->accept(this);
		thenBlock->accept(this);
	}
	else if (elseIfChain == nullptr || elseIfChain->size() == 0) {
		/* Only have else block. */

		condition->tLabel( this->_fallLabel );
		condition->fLabel( this->newLabel() );

		thenBlock->next( ifStmt->next() );
		elseBlock->next( ifStmt->next() );

		condition->accept(this);

		this->_currentFunction->appendLabel(condition->tLabel());
		thenBlock->accept(this);
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );
		this->_currentFunction->appendLabel(condition->fLabel());
		elseBlock->accept(this);
	}
	else if (elseBlock == nullptr) {
		/* Only have else-if blocks. */
		auto nextCondLabel = this->newLabel();

		condition->tLabel( this->_fallLabel );
		condition->fLabel( nextCondLabel );

		/* Process the condition. */
		condition->accept(this);

		/* Then block appended. */
		//this->_currentFunction->appendLabel(condition->tLabel());
		thenBlock->accept(this);
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );

		unsigned int size = elseIfChain->size() - 1;
		unsigned int indx = 0;
		for (auto elseif : *elseIfChain) {
			/* Label to begin of the condition check. */
			this->_currentFunction->appendLabel(nextCondLabel);

			/* In the last else-if we jump off of the if it's false. */
			nextCondLabel = (indx < size) ? this->newLabel() : ifStmt->next();

			/* As usual.. */
			elseif->getCondition()->tLabel( this->_fallLabel );
			elseif->getCondition()->fLabel( nextCondLabel );

			/* Process the condition. */
			elseif->getCondition()->accept(this);

			/* Then block appended. */
			//this->_currentFunction->appendLabel( elseif->getCondition()->tLabel() );
			elseif->getElseIfBlock()->accept(this);
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );

			/* We need to know when we are at the last position. */
			indx++;
		}
	}
	else {
		/* Have else and else-if blocks. */

		/* Only have else-if blocks. */
		auto nextCondLabel = this->newLabel();
		auto elseLabel = this->newLabel();

		condition->tLabel( this->_fallLabel );
		condition->fLabel( nextCondLabel );

		/* Process the condition. */
		condition->accept(this);

		/* Then block appended. */
		//this->_currentFunction->appendLabel(condition->tLabel());
		thenBlock->accept(this);
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );

		unsigned int size = elseIfChain->size() - 1;
		unsigned int indx = 0;
		for (auto elseif : *elseIfChain) {
			/* Label to begin of the condition check. */
			this->_currentFunction->appendLabel(nextCondLabel);

			/* In the last else-if we jump off of the if it's false. */
			nextCondLabel = (indx < size) ? this->newLabel() : elseLabel;

			/* As usual.. */
			elseif->getCondition()->tLabel( this->_fallLabel );
			elseif->getCondition()->fLabel( nextCondLabel );

			/* Process the condition. */
			elseif->getCondition()->accept(this);

			/* Then block appended. */
			//this->_currentFunction->appendLabel( elseif->getCondition()->tLabel() );
			elseif->getElseIfBlock()->accept(this);
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );

			/* We need to know when we are at the last position. */
			indx++;
		}

		/* The else part. */
		this->_currentFunction->appendLabel(elseLabel);
		elseBlock->accept(this);
	}
}

void AstTACGenVisitor::visit(Parser::ElseIfStmt* elseIfStmt) {

}

void AstTACGenVisitor::visit(const Parser::ReturnStmt* ret) {
	Expression* exp = ret->getExpression();

	exp->isExpLeftHand(false);
	exp->accept(this);

	IR::Instruction* newInstruction = new IR::Return(exp->addr());

	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
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
	for (auto statement : *block->getStatements()) {
		statement->next( this->newLabel() );
		statement->accept(this);

		/* If in fact happened to be some instruction branching
		 * to statement->next() label then we append it. */
		if (statement->next()->usageCounter() > 0)
			this->_currentFunction->appendLabel( statement->next() );
	}
}

void AstTACGenVisitor::visit(Parser::StringExpr* str) {
	/* If we are not inside a conditional expression. */
	if ( str->tLabel() == nullptr || str->fLabel() == nullptr ) {
		shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), str->value()));
		this->_currentFunction->symbolTable()->add(cttEntry);
		str->addr(cttEntry);
	}
	else {
		/* We are inside conditional expression. We evaluate the parameters
		 * and goes to the correct direction. */
		if (str->value() == "") {
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(str->fLabel()) ) );
		}
		else {
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(str->tLabel()) ) );
		}
	}
}

void AstTACGenVisitor::visit(Parser::FloatExpr* flt) {
	/* If we are not inside a conditional expression. */
	if ( flt->tLabel() == nullptr || flt->fLabel() == nullptr ) {
		shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), flt->value()));
		this->_currentFunction->symbolTable()->add(cttEntry);
		flt->addr(cttEntry);
	}
	else {
		/* We are inside conditional expression. We evaluate the parameters
		 * and goes to the correct direction. */
		if (flt->value() == 0) {
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(flt->fLabel()) ) );
		}
		else {
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(flt->tLabel()) ) );
		}
	}
}

void AstTACGenVisitor::visit(Parser::IntegerExpr* integer) {
	/* If we are not inside a conditional expression. */
	if ( integer->tLabel() == nullptr || integer->fLabel() == nullptr ) {
		shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), integer->value()));
		this->_currentFunction->symbolTable()->add(cttEntry);
		integer->addr(cttEntry);
	}
	else {
		/* We are inside conditional expression. We evaluate the parameters
		 * and goes to the correct direction. */
		if (integer->value() == 0) {
			if (integer->fLabel() != this->_fallLabel)
				this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(integer->fLabel()) ) );
		}
		else {
			if (integer->tLabel() != this->_fallLabel)
				this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(integer->tLabel()) ) );
		}
	}
}

void AstTACGenVisitor::visit(Parser::IdentifierExpr* id) {
	if (id->dimsExprs() != nullptr && id->dimsExprs()->size() > 0) {
		/* Tells parent "visiting" methods that this is an array access. */
		id->isArrayAccess(true);

		/* We look into the symbol table to find the dimensions sizes. */
		shared_ptr<SymbolTableEntry> entry = this->_currentFunction->symbolTable()->lookup(id->value());
		STVariableDeclaration* decl = dynamic_cast<STVariableDeclaration*>( entry.get() );

		/* Here we are going to store the factors for each index. */
		vector<shared_ptr<SymbolTableEntry>> indExpsFacts;

		/* Here we are going to store the accumulated (from right to left) size of each dimension. */
		vector<int> sizes(decl->dims());

		/* Computes the size of each complete dimension. */
		for (int i=sizes.size()-1, prev=1; i>=0; i--) {
			sizes[i] = prev;
			prev *= decl->dims()[i];
		}

		/* Iterate computing the expressions for each index. */
		int factIndex = 0;
		for (auto dimExpr : *id->dimsExprs()) {
			/* Compute the index expression. */
			dimExpr->accept(this);

			/* This represent the index used to make the access and the dimension size. */
			shared_ptr<SymbolTableEntry> ind = dimExpr->addr();
			shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), sizes[factIndex]));

			/* multiply index*DIM_SIZE and store it. */
			IR::Instruction* newInstruction = new IR::IMul(this->newTemporary(Parser::INT), cttEntry, ind);

			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( newInstruction ) );

			/* We store back the target because at the end we need to sum up the partial factors. */
			indExpsFacts.push_back( newInstruction->tgt() );

			factIndex++;
		}

		/* Accumulate the value of the factors. */
		shared_ptr<SymbolTableEntry> prev = indExpsFacts[0];
		for (factIndex=1; factIndex < indExpsFacts.size(); factIndex++) {
			/* Sum prev factor with current. */
			IR::Instruction* newInstruction = new IR::IAdd(this->newTemporary(Parser::INT), prev, indExpsFacts[factIndex]);

			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( newInstruction ) );

			prev = newInstruction->tgt();
		}

		/* Now we need to consider the base type size. */
		shared_ptr<Parser::STConstantDef> cttBaseType(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), AstSemaVisitor::typeWidth(decl->type())));
		IR::Instruction* indAccess = new IR::IMul(this->newTemporary(Parser::INT), cttBaseType, prev);
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( indAccess ) );

		/* Now we have to sum with the base pointer (id). */
		IR::Instruction* arrAccess = new IR::IAdd(this->newTemporary(Parser::INT), entry, indAccess->tgt());
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( arrAccess ) );

		/* If we are parsing a right-hand side expression we will de-refer the pointer and load
		 * the array value in a temporary. Otherwise, we will just return the address to the previous
		 * expression, and it will take care of dereferencing. 									   */
		if (!id->isExpLeftHand()) {
			IR::Instruction* derefer = new IR::CopyFromArray(this->newTemporary(id->type()), arrAccess->tgt());
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( derefer ) );

			id->addr( derefer->tgt() );
		}
		else {
			id->addr( arrAccess->tgt() );
		}
	}
	else {
		id->isArrayAccess(false);
		id->addr( this->_currentFunction->symbolTable()->lookup(id->value()) );
		/* No need to code gen. */
		/* Address was already computed in Sema. */
	}

	if (id->tLabel() != nullptr && id->fLabel() != nullptr) {
		auto entry 	= this->_currentFunction->symbolTable()->lookup(id->value());
		auto type 	= entry->type();

		/* Only numerical values are acceptable in boolean comparissons. */
		if (type == Parser::INT || type == Parser::FLOAT) {
			this->emitBranchesBasedOnExpValue(id->addr(), id->tLabel(), id->fLabel());
		}
		else {
			throw System::EXCEPTION_UNEXPECTED_TYPE;
		}
	}
}

void AstTACGenVisitor::visit(Parser::FunctionCall* funCall) {
	auto decl 	 = this->_currentFunction->symbolTable()->lookup(funCall->name());
	auto tmpRes = this->newTemporary( decl->type() );

	shared_ptr<IR::Call> newInstruction = shared_ptr<IR::Call>( new IR::Call(decl, tmpRes) );

	for (auto argument : *funCall->arguments()) {
		/* Visit the argument, produce address for them.*/
		argument->accept(this);

		/* Add the argument to the function call. */
		newInstruction->addArgument(argument->addr());
	}

	this->_currentFunction->appendInstruction(shared_ptr<IR::Call>(newInstruction));

	funCall->addr(tmpRes);
}

void AstTACGenVisitor::visit(Parser::UnaryExpr* unary) {
	if (unary->opr() == UnaryExpr::NOT) {
		translateBooleanExp(unary);
	}
	else if (unary->opr() == UnaryExpr::ADDR) {
		unary->exp()->isExpLeftHand(false);
		unary->exp()->accept(this);

		auto newInstruction = new IR::Addr(this->newTemporary(unary->type()), unary->exp()->addr());

		unary->addr( newInstruction->tgt() );

		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
	}
	else if (unary->opr() == UnaryExpr::BIT_NOT) {
		unary->exp()->isExpLeftHand(false);
		unary->exp()->accept(this);

		auto newInstruction = new IR::BinNot(this->newTemporary(unary->type()), unary->exp()->addr());

		unary->addr( newInstruction->tgt() );

		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
	}
	else {
		translateArithmeticExpr(unary);
	}
}

void AstTACGenVisitor::translateBooleanExp(Parser::UnaryExpr* unary) {
	auto notCond = false;
	auto nextNull = false;

	/* If the jump labels are nullptr then we are not decoding a boolean
	 * expression that is inside a if statement, thus it is an expression
	 * and we need to take care to create the appropriate labels to storing
	 * the true and false values. */
	if (unary->tLabel() == nullptr && unary->fLabel() == nullptr) {
		notCond = true;
		unary->tLabel( this->newLabel("ult") );
		unary->fLabel( this->newLabel("ulf") );

		/* We need to make sure that there is a target so we can jump over the
		 * true/false blocks. */
		if (unary->next() == nullptr) {
			nextNull = true;
			unary->next( this->newLabel("ulExit") );
		}
	}

	unary->exp()->isExpLeftHand(false);
	unary->exp()->tLabel( unary->fLabel() );
	unary->exp()->fLabel( unary->tLabel() );
	unary->exp()->accept(this);

	if (notCond) {
		shared_ptr<STTempVar> res = this->newTemporary(Parser::INT);

		this->_currentFunction->appendLabel(unary->tLabel());
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::ScalarCopy(res, this->newConstant<int>(1)) ) );
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(unary->next()) ) );
		this->_currentFunction->appendLabel(unary->fLabel());
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::ScalarCopy(res, this->newConstant<int>(0)) ) );

		if (nextNull)
			this->_currentFunction->appendLabel(unary->next());

		unary->addr(res);
	}
}

void AstTACGenVisitor::translateArithmeticExpr(Parser::UnaryExpr* unary) {
	IR::Instruction* newInstruction = nullptr;
	Expression* exp 				= unary->exp();
	NativeType tgtType 				= unary->type();

	exp->isExpLeftHand(false);
	exp->accept(this);

	if (tgtType == Parser::INT) {
		switch (unary->opr()) {
			case UnaryExpr::MINUS:
				newInstruction = new IR::IMinus(this->newTemporary(tgtType), exp->addr());
				break;

			case UnaryExpr::PLUS:
				newInstruction = new IR::IPlus(this->newTemporary(tgtType), exp->addr());
				break;

			case UnaryExpr::INCREMENT:
				newInstruction = new IR::IInc(this->newTemporary(tgtType), exp->addr());
				break;

			case UnaryExpr::DECREMENT:
				newInstruction = new IR::IDec(this->newTemporary(tgtType), exp->addr());
				break;
		}
	}
	else if (tgtType == Parser::FLOAT) {
		switch (unary->opr()) {
			case UnaryExpr::MINUS:
				newInstruction = new IR::FMinus(this->newTemporary(tgtType), exp->addr());
				break;

			case UnaryExpr::PLUS:
				newInstruction = new IR::FPlus(this->newTemporary(tgtType), exp->addr());
				break;

			case UnaryExpr::INCREMENT:
				newInstruction = new IR::FInc(this->newTemporary(tgtType), exp->addr());
				break;

			case UnaryExpr::DECREMENT:
				newInstruction = new IR::FDec(this->newTemporary(tgtType), exp->addr());
				break;
		}
	}

	unary->addr( newInstruction->tgt() );

	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
}

void AstTACGenVisitor::visit(Parser::BinaryExpr* binop) {
	auto opr = binop->opr();

	if (opr == BinaryExpr::LOG_AND || opr == BinaryExpr::LOG_OR) {
		translateBooleanExp(binop);
	}
	else if (opr == BinaryExpr::LT || opr == BinaryExpr::LTE || opr == BinaryExpr::GT || opr == BinaryExpr::GTE || opr == BinaryExpr::COMPARE || opr == BinaryExpr::DIFFERENCE) {
		translateRelationalExp(binop);
	}
	else {
		translateArithmeticExp(binop);
	}
}

void AstTACGenVisitor::translateBooleanExp(Parser::BinaryExpr* binop) {
	auto exp1 	  = binop->getExp1();
	auto exp2 	  = binop->getExp2();
	auto tgtType  = binop->type();
	auto notCond  = false;
	auto nextNull = false;

	/* If the jump labels are nullptr then we are not decoding a boolean
	 * expression that is inside a if statement, thus it is an expression
	 * and we need to take care to create the appropriate labels to storing
	 * the true and false values. */
	if (binop->tLabel() == nullptr && binop->fLabel() == nullptr) {
		notCond = true;
		binop->tLabel( this->_fallLabel );
		binop->fLabel( this->newLabel() );

		/* We need to make sure that there is a target so we can jump over the
		 * true/false blocks. */
		if (binop->next() == nullptr) {
			nextNull = true;
			binop->next( this->newLabel() );
		}
	}

	exp1->isExpLeftHand(false);
	exp2->isExpLeftHand(false);

	if (binop->opr() == BinaryExpr::LOG_AND) {
		auto newLabel = (binop->fLabel() != this->_fallLabel) ? binop->fLabel() : this->newLabel();

		exp1->tLabel( this->_fallLabel );
		exp1->fLabel( newLabel );

		exp2->tLabel( binop->tLabel() );
		exp2->fLabel( binop->fLabel() );

		exp1->accept(this);
		exp2->accept(this);

		if (binop->fLabel() == this->_fallLabel) {
			this->_currentFunction->appendLabel(newLabel);
		}
	}
	else if (binop->opr() == BinaryExpr::LOG_OR) {
		auto newLabel = (binop->tLabel() != this->_fallLabel) ? binop->tLabel() : this->newLabel();

		exp1->tLabel( newLabel );
		exp1->fLabel( this->_fallLabel );
		exp2->tLabel( binop->tLabel() );
		exp2->fLabel( binop->fLabel() );

		exp1->accept(this);
		exp2->accept(this);

		if (binop->tLabel() == this->_fallLabel) {
			this->_currentFunction->appendLabel(newLabel);
		}
	}

	/* We aren't inside a conditional, then we insert the labels
	 * were we create the result of the conditionals. */
	if (notCond) {
		shared_ptr<STTempVar> res = this->newTemporary(Parser::INT);

		//this->_currentFunction->appendLabel(binop->tLabel());
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::ScalarCopy(res, this->newConstant<int>(1)) ) );
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(binop->next()) ) );
		this->_currentFunction->appendLabel(binop->fLabel());
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::ScalarCopy(res, this->newConstant<int>(0)) ) );

		if (nextNull)
			this->_currentFunction->appendLabel(binop->next());

		binop->addr(res);
	}
}

void AstTACGenVisitor::translateArithmeticExp(Parser::BinaryExpr* binop) {
	IR::Instruction* newInstruction = nullptr;
	auto exp1 	 = binop->getExp1();
	auto exp2 	 = binop->getExp2();
	auto tgtType = binop->type();

	exp1->isExpLeftHand(binop->opr() == BinaryExpr::ASSIGN);
	exp1->accept(this);

	exp2->isExpLeftHand(false);
	exp2->accept(this);

	if (tgtType == Parser::INT) {
		switch (binop->opr()) {
			case BinaryExpr::ASSIGN:
				if (exp1->isArrayAccess())
					newInstruction = new IR::CopyToArray(exp1->addr(), exp2->addr());
				else
					newInstruction = new IR::ScalarCopy(exp1->addr(), exp2->addr());
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
	else if (tgtType == Parser::FLOAT) {
		switch (binop->opr()) {
			case BinaryExpr::ASSIGN:
				if (exp1->isArrayAccess())
					newInstruction = new IR::CopyToArray(exp1->addr(), exp2->addr());
				else
					newInstruction = new IR::ScalarCopy(exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::ADDITION:
				newInstruction = new IR::FAdd(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::SUBTRACTION:
				newInstruction = new IR::FSub(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::TIMES:
				newInstruction = new IR::FMul(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::DIV:
				newInstruction = new IR::FDiv(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::PLUS_EQUAL:
				newInstruction = new IR::FAdd(exp1->addr(), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::MINUS_EQUAL:
				newInstruction = new IR::FSub(exp1->addr(), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::TIMES_EQUAL:
				newInstruction = new IR::FMul(exp1->addr(), exp1->addr(), exp2->addr());
				break;

			case BinaryExpr::DIV_EQUAL:
				newInstruction = new IR::FDiv(exp1->addr(), exp1->addr(), exp2->addr());
				break;
		}
	}
	else if (tgtType == Parser::STRING) {
		if (binop->opr() == BinaryExpr::ASSIGN) {
			auto st 	   = this->_currentFunction->symbolTable();
			newInstruction = new IR::Call(st->lookup(System::NAT_FUN_STRCPY), this->newTemporary(Parser::STRING));

			newInstruction->addArgument( exp1->addr() );
			newInstruction->addArgument( exp2->addr() );
		}
	}

	binop->addr( newInstruction->tgt() );

	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );

	/* If we are inside a conditional we need to use the result
	 * of this expression to consider the jump targets. */
	if (binop->tLabel() != nullptr || binop->fLabel() != nullptr) {
		this->emitBranchesBasedOnExpValue(binop->addr(), binop->tLabel(), binop->fLabel());
	}
}

void AstTACGenVisitor::translateRelationalExp(Parser::BinaryExpr* binop) {
	IR::Instruction* newInstruction = nullptr;
	auto exp1 	 = binop->getExp1();
	auto exp2 	 = binop->getExp2();
	auto tgtType = binop->type();

	exp1->isExpLeftHand(false);
	exp1->tLabel(nullptr);
	exp1->fLabel(nullptr);
	exp1->accept(this);

	exp2->isExpLeftHand(false);
	exp2->tLabel(nullptr);
	exp2->fLabel(nullptr);
	exp2->accept(this);

	switch (binop->opr()) {
		case BinaryExpr::COMPARE:
			newInstruction = new IR::REqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
			break;

		case BinaryExpr::DIFFERENCE:
			newInstruction = new IR::RNotEqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
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
	}

	binop->addr( newInstruction->tgt() );
	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );

	auto tLabel = binop->tLabel();
	auto fLabel = binop->fLabel();
	auto fall = this->_fallLabel;

	/* If neither targets were informed then it is because this
	 * expression is part of a larger expression and in this case
	 * we cannot have short-circuit code yet. */
	if (tLabel != nullptr && fLabel != nullptr) {
		/* In this case jumps were informed for both targets, this
		 * may happen when this expression is part of a larger expression
		 * that is inside conditionals. */
		if (tLabel != fall && fLabel != fall) {
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondTrueJump(binop->addr(), tLabel) ) );
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(fLabel) ) );
		}
		else if (tLabel != fall) {
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondTrueJump(binop->addr(), tLabel) ) );
		}
		else if (fLabel != fall) {
			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondFalseJump(binop->addr(), fLabel) ) );
		}
		else {
			/* I do not expect this to happen, but better be
			 * safe than sorry. */
			throw System::EXCEPTION_UNREACHABLE_CODE;
		}
	}
}

void AstTACGenVisitor::emitBranchesBasedOnExpValue(shared_ptr<SymbolTableEntry> result, shared_ptr<STLabelDef> lTrue, shared_ptr<STLabelDef> lFalse) {
	auto cmpInstr = new IR::REqual(this->newTemporary(Parser::INT), result, this->newConstant<int>(0));
	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( cmpInstr ) );

	if (lTrue != this->_fallLabel && lFalse != this->_fallLabel) {
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondTrueJump(cmpInstr->tgt(), lFalse) ) );
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(lTrue) ) );
	}
	else if (lTrue == this->_fallLabel) {
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondTrueJump(cmpInstr->tgt(), lFalse) ) );
	}
	else if (lFalse == this->_fallLabel) {
		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondFalseJump(cmpInstr->tgt(), lTrue) ) );
	}
	else
		throw System::EXCEPTION_UNREACHABLE_CODE;
}

template<typename T>
shared_ptr<STConstantDef> AstTACGenVisitor::newConstant(T value) {
	int tempId = this->constCounter++;

	shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("_ct" + std::to_string(tempId), value));

	this->_currentFunction->symbolTable()->add(cttEntry);

	return cttEntry;
}

shared_ptr<STLabelDef> AstTACGenVisitor::newLabel(string suffix) {
	int tempId = this->labelCounter++;

	return shared_ptr<STLabelDef>(new STLabelDef("_L" + suffix + std::to_string(tempId)));
}

shared_ptr<STTempVar> AstTACGenVisitor::newTemporary(NativeType type) {
	int width  = AstSemaVisitor::typeWidth(type);
	int tempId = this->tempCounter++;
	int offset = this->_currentOffset += width;

	return shared_ptr<STTempVar>(new STTempVar("_t" + std::to_string(tempId), type, width, offset));
}

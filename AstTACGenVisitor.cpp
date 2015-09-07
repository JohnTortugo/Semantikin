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

	/* Continue visiting children. */
	for (auto function : *module->getFunctions()) {
		/* This label is just a sentinel pointer to convey the
		 * information that the expression should consider a fallthrough
		 * path. */
		this->_fallLabel = this->newLabel(function->getName(), "fallthrough");

		/* Something like an inherited attribute. */
		this->_currentFunction = shared_ptr<IR::Function>(new IR::Function(function->getSymbTable()));

		/* Reset the counters for temps/labels/constants. */
		this->constCounter = 1;
		this->labelCounter = 1;
		this->tempCounter = 1;
		this->_currentOffset = function->currentOffset();

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
	this->_currentFunction->appendLabel( this->newLabel(function->getName(), "entry") );

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
//			initializer->accept(this);
//
//			if (initializer->type() == Parser::INT || initializer->type() == Parser::FLOAT) {
//				this->_currentFunction->appendInstruction(shared_ptr<IR::ScalarCopy>(new IR::ScalarCopy(st->lookup(spec->getName()), initializer->addr())));
//			}
//			else if (initializer->type() == Parser::STRING) {
//				shared_ptr<IR::Call> newInstruction = shared_ptr<IR::Call>( new IR::Call(st->lookup(System::NAT_FUN_STRCPY), nullptr) );
//
//				newInstruction->addArgument(st->lookup(spec->getName()));
//				newInstruction->addArgument(initializer->addr());
//
//				this->_currentFunction->appendInstruction(shared_ptr<IR::Call>(newInstruction));
//			}
//			else {
//				throw System::EXCEPTION_UNEXPECTED_TYPE;
//			}
		}
	}
}

void AstTACGenVisitor::visit(Parser::LoopStmt* loop) {
//	auto begin 		= this->newLabel(this->_currentFunction->addr()->getName());
//	auto condition 	= loop->getCondition();
//	auto codeBlock 	= loop->getBody();
//
//	condition->tLabel( this->_fallLabel );
//	condition->fLabel( loop->next() );
//	codeBlock->next( begin );
//
//	this->_currentFunction->appendLabel(begin);
//	condition->accept(this);
////	this->_currentFunction->appendLabel(condition->tLabel());
//	codeBlock->accept(this);
//	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(begin) ) );
}

void AstTACGenVisitor::visit(Parser::IfStmt* ifStmt) {
//	auto condition = ifStmt->getCondition();
//	auto thenBlock = ifStmt->getThenBlock();
//	auto elseBlock = ifStmt->getElseBlock();
//	list<shared_ptr<Parser::ElseIfStmt>>* elseIfChain = ifStmt->getElseIfChain();
//
//	if ((elseIfChain == nullptr || elseIfChain->size() == 0) && elseBlock == nullptr) {
//		/* Does not have else-if nor else blocks. */
//
//		condition->tLabel( this->_fallLabel );
//		condition->fLabel( ifStmt->next() );
//		thenBlock->next( ifStmt->next() );
//
//		condition->accept(this);
//		thenBlock->accept(this);
//	}
//	else if (elseIfChain == nullptr || elseIfChain->size() == 0) {
//		/* Only have else block. */
//
//		condition->tLabel( this->_fallLabel );
//		condition->fLabel( this->newLabel(this->_currentFunction->addr()->getName()) );
//
//		thenBlock->next( ifStmt->next() );
//		elseBlock->next( ifStmt->next() );
//
//		condition->accept(this);
//
//		this->_currentFunction->appendLabel(condition->tLabel());
//		thenBlock->accept(this);
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );
//		this->_currentFunction->appendLabel(condition->fLabel());
//		elseBlock->accept(this);
//	}
//	else if (elseBlock == nullptr) {
//		/* Only have else-if blocks. */
//		auto nextCondLabel = this->newLabel(this->_currentFunction->addr()->getName());
//
//		condition->tLabel( this->_fallLabel );
//		condition->fLabel( nextCondLabel );
//
//		/* Process the condition. */
//		condition->accept(this);
//
//		/* Then block appended. */
//		//this->_currentFunction->appendLabel(condition->tLabel());
//		thenBlock->accept(this);
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );
//
//		unsigned int size = elseIfChain->size() - 1;
//		unsigned int indx = 0;
//		for (auto elseif : *elseIfChain) {
//			/* Label to begin of the condition check. */
//			this->_currentFunction->appendLabel(nextCondLabel);
//
//			/* In the last else-if we jump off of the if it's false. */
//			nextCondLabel = (indx < size) ? this->newLabel(this->_currentFunction->addr()->getName()) : ifStmt->next();
//
//			/* As usual.. */
//			elseif->getCondition()->tLabel( this->_fallLabel );
//			elseif->getCondition()->fLabel( nextCondLabel );
//
//			/* Process the condition. */
//			elseif->getCondition()->accept(this);
//
//			/* Then block appended. */
//			//this->_currentFunction->appendLabel( elseif->getCondition()->tLabel() );
//			elseif->getElseIfBlock()->accept(this);
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );
//
//			/* We need to know when we are at the last position. */
//			indx++;
//		}
//	}
//	else {
//		/* Have else and else-if blocks. */
//
//		/* Only have else-if blocks. */
//		auto nextCondLabel = this->newLabel(this->_currentFunction->addr()->getName());
//		auto elseLabel = this->newLabel(this->_currentFunction->addr()->getName());
//
//		condition->tLabel( this->_fallLabel );
//		condition->fLabel( nextCondLabel );
//
//		/* Process the condition. */
//		condition->accept(this);
//
//		/* Then block appended. */
//		//this->_currentFunction->appendLabel(condition->tLabel());
//		thenBlock->accept(this);
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );
//
//		unsigned int size = elseIfChain->size() - 1;
//		unsigned int indx = 0;
//		for (auto elseif : *elseIfChain) {
//			/* Label to begin of the condition check. */
//			this->_currentFunction->appendLabel(nextCondLabel);
//
//			/* In the last else-if we jump off of the if it's false. */
//			nextCondLabel = (indx < size) ? this->newLabel(this->_currentFunction->addr()->getName()) : elseLabel;
//
//			/* As usual.. */
//			elseif->getCondition()->tLabel( this->_fallLabel );
//			elseif->getCondition()->fLabel( nextCondLabel );
//
//			/* Process the condition. */
//			elseif->getCondition()->accept(this);
//
//			/* Then block appended. */
//			//this->_currentFunction->appendLabel( elseif->getCondition()->tLabel() );
//			elseif->getElseIfBlock()->accept(this);
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(ifStmt->next()) ) );
//
//			/* We need to know when we are at the last position. */
//			indx++;
//		}
//
//		/* The else part. */
//		this->_currentFunction->appendLabel(elseLabel);
//		elseBlock->accept(this);
//	}
}

void AstTACGenVisitor::visit(Parser::ElseIfStmt* elseIfStmt) {

}

void AstTACGenVisitor::visit(const Parser::ReturnStmt* ret) {
//	Expression* exp = ret->getExpression();
//
//	exp->isExpLeftHand(false);
//	exp->accept(this);
//
//	IR::Instruction* newInstruction = new IR::Return(exp->addr());
//
//	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
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
		statement->next( this->newLabel(this->_currentFunction->addr()->getName()) );
		statement->accept(this);

		/* If in fact happened to be some instruction branching
		 * to statement->next() label then we append it. */
		if (statement->next()->usageCounter() > 0)
			this->_currentFunction->appendLabel( statement->next() );
	}
}

void AstTACGenVisitor::visit(Parser::StringExpr* str) {
//	/* If we are not inside a conditional expression. */
//	if ( str->tLabel() == nullptr || str->fLabel() == nullptr ) {
//		shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), str->value()));
//		this->_currentFunction->symbolTable()->add(cttEntry);
//		str->addr(cttEntry);
//	}
//	else {
//		/* We are inside conditional expression. We evaluate the parameters
//		 * and goes to the correct direction. */
//		if (str->value() == "") {
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(str->fLabel()) ) );
//		}
//		else {
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(str->tLabel()) ) );
//		}
//	}
}

void AstTACGenVisitor::visit(Parser::FloatExpr* flt) {
//	/* If we are not inside a conditional expression. */
//	if ( flt->tLabel() == nullptr || flt->fLabel() == nullptr ) {
//		shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("ctt" + std::to_string(constCounter++), flt->value()));
//		this->_currentFunction->symbolTable()->add(cttEntry);
//		flt->addr(cttEntry);
//	}
//	else {
//		/* We are inside conditional expression. We evaluate the parameters
//		 * and goes to the correct direction. */
//		if (flt->value() == 0) {
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(flt->fLabel()) ) );
//		}
//		else {
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(flt->tLabel()) ) );
//		}
//	}
}

void AstTACGenVisitor::visit(Parser::IntegerExpr* integer) {
	/* If we are not inside a conditional expression. */
	if ( integer->tLabel() == nullptr || integer->fLabel() == nullptr ) {
		auto cttEntry = make_shared<Parser::STConstantDef>("ctt" + std::to_string(constCounter++), integer->value());

		this->_currentFunction->symbolTable()->add(cttEntry);

		this->_lastInstruction = make_shared<IR::Immediate>(cttEntry);
	}
	else {
//		/* We are inside conditional expression. We evaluate the parameters
//		 * and goes to the correct direction. */
//		if (integer->value() == 0) {
//			if (integer->fLabel() != this->_fallLabel)
//				this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(integer->fLabel()) ) );
//		}
//		else {
//			if (integer->tLabel() != this->_fallLabel)
//				this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(integer->tLabel()) ) );
//		}
	}
}

void AstTACGenVisitor::visit(Parser::IdentifierExpr* id) {
	/* We look into the symbol table to find the dimensions sizes. */
	shared_ptr<SymbolTableEntry> entry 	= this->_currentFunction->symbolTable()->lookup(id->value());
	auto decl 							= dynamic_cast<STVariableDeclaration*>( entry.get() );

	if (decl->dims().size() > 0) {
		/* Tells parent "visiting" methods that this is an array access. */
		id->isArrayAccess(true);

//		/* If no dimensions were specified then we just return the base addr of the array. */
//		if (id->dimsExprs()->size() != decl->dims().size()) {
//			IR::Instruction* entryAddr = new IR::Addr(this->newTemporary(Parser::INT), entry);
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( entryAddr ) );
//
//			id->addr( entryAddr->tgt() );
//			return ;
//		}

		/* Here we are going to store the factors for each index. */
		vector<shared_ptr<IR::Instruction>> indExpsInstructions;

		/* Here we are going to store the accumulated (from right to left) size of each dimension. */
		vector<int> sizes(decl->dims());

		/* Computes the size of each complete dimension. */
		for (int i=sizes.size()-1, prev=1; i>=0; i--) {
			sizes[i]  = prev;
			prev 	 *= decl->dims()[i];
		}

		/* Iterate computing the expressions for each index. */
		int factIndex = 0;
		for (auto dimExpr : *id->dimsExprs()) {
			/* Compute the index expression. */
			dimExpr->accept(this);
			auto idxExprInstr = this->_lastInstruction;
			this->_lastInstruction = nullptr;

			/** This represents the dimension size. */
			auto dimSizeCtt   = make_shared<Parser::STConstantDef>("ctt" + std::to_string(constCounter++), sizes[factIndex]);
			auto dimSizeInstr = make_shared<IR::Immediate>(dimSizeCtt);

			/** This is where the partial result will be stored. */
			auto tempResult = this->newTemporary(Parser::INT);

			/* multiply index*DIM_SIZE and store it. */
			auto newInstruction = make_shared<IR::IMul>(tempResult, idxExprInstr, dimSizeInstr);

			///this->_currentFunction->appendInstruction(newInstruction);

			/* We store back the target because at the end we need to sum up the partial factors. */
			indExpsInstructions.push_back(newInstruction);

			factIndex++;
		}

		/* Accumulate the value of the factors. */
		auto prev = indExpsInstructions[0];
		for (factIndex=1; factIndex < indExpsInstructions.size(); factIndex++) {
			/* In each iteration we are going to sum prev factor with current. */

			/** This is where the partial result will be stored. */
			auto tempResult = this->newTemporary(Parser::INT);

			auto newInstruction = make_shared<IR::IAdd>(tempResult, prev, indExpsInstructions[factIndex]);

			//this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( newInstruction ) );

			prev = newInstruction;
		}

		/**
		 * Above we computed the *index* of the element we are trying to access. Now we need
		 * to multiplicate the index by the size of the array's type so we can access a byte
		 * inside the array.
		 */
		auto cttBaseType  = make_shared<Parser::STConstantDef>("ctt" + std::to_string(constCounter++), AstSemaVisitor::typeWidth(decl->type()));
		auto immBaseType  = make_shared<IR::Immediate>(cttBaseType);
		auto tempResult   = this->newTemporary(Parser::INT);
		auto indAccess 	  = make_shared<IR::IMul>(tempResult, immBaseType, prev);

		/* Find the array base *address* (which is a pointer) */
		tempResult		   = this->newTemporary(Parser::INT);
		auto varEntryInstr = make_shared<IR::Memory>( shared_ptr<Parser::STVariableDeclaration>(decl) );
		auto entryAddr 	   = make_shared<IR::Addr>(tempResult, varEntryInstr);
		//this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( entryAddr ) );

		/**
		 * Now we sum the address of the base of the array with the number of bytes we
		 * computed from the index*base_type.
		 */
		tempResult	= this->newTemporary(Parser::INT);
		auto arrAccess 	= make_shared<IR::AddrDispl>(tempResult, entryAddr, indAccess);
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( arrAccess ) );

		/* If we are parsing a right-hand side expression we will de-refer the pointer and load
		 * the array value in a temporary. Otherwise, we will just return the address to the previous
		 * expression, and it will take care of dereferencing. 									   */
		if (id->isExpLeftHand()) {
			this->_lastInstruction = arrAccess;
//			id->addr( arrAccess->tgt() );
		}
		else {
			tempResult	 = this->newTemporary(id->type());
			auto derefer = make_shared<IR::CopyFromArray>(tempResult, arrAccess);
			this->_lastInstruction = derefer;

			///this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( derefer ) );
			///id->addr( derefer->tgt() );
		}
	}
	else {
		id->isArrayAccess(false);
		this->_lastInstruction = make_shared<IR::Memory>( shared_ptr<STVariableDeclaration>(decl) );
	}

	if (id->tLabel() != nullptr && id->fLabel() != nullptr) {
		auto entry 	= this->_currentFunction->symbolTable()->lookup(id->value());
		auto type 	= entry->type();

		/* Only numerical values are acceptable in boolean comparissons. */
		if (type == Parser::INT || type == Parser::FLOAT) {
			this->emitBranchesBasedOnExpValue(this->_lastInstruction, id->tLabel(), id->fLabel());
		}
		else {
			throw System::EXCEPTION_UNEXPECTED_TYPE;
		}
	}
}

void AstTACGenVisitor::visit(Parser::FunctionCall* funCall) {
//	auto decl 	 = this->_currentFunction->symbolTable()->lookup(funCall->name());
//	auto tmpRes = this->newTemporary( decl->type() );
//
//	shared_ptr<IR::Call> newInstruction = shared_ptr<IR::Call>( new IR::Call(decl, tmpRes) );
//
//	for (auto argument : *funCall->arguments()) {
//		/* Visit the argument, produce address for them.*/
//		argument->accept(this);
//
//		/* Add the argument to the function call. */
//		newInstruction->addArgument(argument->addr());
//	}
//
//	this->_currentFunction->appendInstruction(shared_ptr<IR::Call>(newInstruction));
//
//	funCall->addr(tmpRes);
}

void AstTACGenVisitor::visit(Parser::UnaryExpr* unary) {
//	if (unary->opr() == UnaryExpr::NOT) {
//		translateBooleanExp(unary);
//	}
//	else if (unary->opr() == UnaryExpr::ADDR) {
//		unary->exp()->isExpLeftHand(false);
//		unary->exp()->accept(this);
//
//		auto newInstruction = new IR::Addr(this->newTemporary(unary->type()), unary->exp()->addr());
//
//		unary->addr( newInstruction->tgt() );
//
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
//	}
//	else if (unary->opr() == UnaryExpr::BIT_NOT) {
//		unary->exp()->isExpLeftHand(false);
//		unary->exp()->accept(this);
//
//		auto newInstruction = new IR::BinNot(this->newTemporary(unary->type()), unary->exp()->addr());
//
//		unary->addr( newInstruction->tgt() );
//
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
//	}
//	else {
//		translateArithmeticExpr(unary);
//	}
}

void AstTACGenVisitor::translateBooleanExp(Parser::UnaryExpr* unary) {
//	auto notCond = false;
//	auto nextNull = false;
//
//	/* If the jump labels are nullptr then we are not decoding a boolean
//	 * expression that is inside a if statement, thus it is an expression
//	 * and we need to take care to create the appropriate labels to storing
//	 * the true and false values. */
//	if (unary->tLabel() == nullptr && unary->fLabel() == nullptr) {
//		notCond = true;
//		unary->tLabel( this->newLabel("ult") );
//		unary->fLabel( this->newLabel("ulf") );
//
//		/* We need to make sure that there is a target so we can jump over the
//		 * true/false blocks. */
//		if (unary->next() == nullptr) {
//			nextNull = true;
//			unary->next( this->newLabel("ulExit") );
//		}
//	}
//
//	unary->exp()->isExpLeftHand(false);
//	unary->exp()->tLabel( unary->fLabel() );
//	unary->exp()->fLabel( unary->tLabel() );
//	unary->exp()->accept(this);
//
//	if (notCond) {
//		shared_ptr<STTempVar> res = this->newTemporary(Parser::INT);
//
//		this->_currentFunction->appendLabel(unary->tLabel());
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::ScalarCopy(res, this->newConstant<int>(1)) ) );
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(unary->next()) ) );
//		this->_currentFunction->appendLabel(unary->fLabel());
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::ScalarCopy(res, this->newConstant<int>(0)) ) );
//
//		if (nextNull)
//			this->_currentFunction->appendLabel(unary->next());
//
//		unary->addr(res);
//	}
}

void AstTACGenVisitor::translateArithmeticExpr(Parser::UnaryExpr* unary) {
//	IR::Instruction* newInstruction = nullptr;
//	Expression* exp 				= unary->exp();
//	NativeType tgtType 				= unary->type();
//
//	exp->isExpLeftHand(false);
//	exp->accept(this);
//
//	if (tgtType == Parser::INT) {
//		switch (unary->opr()) {
//			case UnaryExpr::MINUS:
//				newInstruction = new IR::IMinus(this->newTemporary(tgtType), exp->addr());
//				break;
//
//			case UnaryExpr::INCREMENT:
//				newInstruction = new IR::IInc(exp->addr(), exp->addr());
//				break;
//
//			case UnaryExpr::DECREMENT:
//				newInstruction = new IR::IDec(exp->addr(), exp->addr());
//				break;
//		}
//	}
//	else if (tgtType == Parser::FLOAT) {
//		switch (unary->opr()) {
//			case UnaryExpr::MINUS:
//				newInstruction = new IR::FMinus(this->newTemporary(tgtType), exp->addr());
//				break;
//
//			case UnaryExpr::INCREMENT:
//				newInstruction = new IR::FInc(exp->addr(), exp->addr());
//				break;
//
//			case UnaryExpr::DECREMENT:
//				newInstruction = new IR::FDec(exp->addr(), exp->addr());
//				break;
//		}
//	}
//
//	unary->addr( newInstruction->tgt() );
//
//	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
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
//	auto exp1 	  = binop->getExp1();
//	auto exp2 	  = binop->getExp2();
//	auto tgtType  = binop->type();
//	auto notCond  = false;
//	auto nextNull = false;
//
//	/* If the jump labels are nullptr then we are not decoding a boolean
//	 * expression that is inside a if statement, thus it is an expression
//	 * and we need to take care to create the appropriate labels to storing
//	 * the true and false values. */
//	if (binop->tLabel() == nullptr && binop->fLabel() == nullptr) {
//		notCond = true;
//		binop->tLabel( this->_fallLabel );
//		binop->fLabel( this->newLabel(this->_currentFunction->addr()->getName()) );
//
//		/* We need to make sure that there is a target so we can jump over the
//		 * true/false blocks. */
//		if (binop->next() == nullptr) {
//			nextNull = true;
//			binop->next( this->newLabel(this->_currentFunction->addr()->getName()) );
//		}
//	}
//
//	exp1->isExpLeftHand(false);
//	exp2->isExpLeftHand(false);
//
//	if (binop->opr() == BinaryExpr::LOG_AND) {
//		auto newLabel = (binop->fLabel() != this->_fallLabel) ? binop->fLabel() : this->newLabel(this->_currentFunction->addr()->getName());
//
//		exp1->tLabel( this->_fallLabel );
//		exp1->fLabel( newLabel );
//
//		exp2->tLabel( binop->tLabel() );
//		exp2->fLabel( binop->fLabel() );
//
//		exp1->accept(this);
//		exp2->accept(this);
//
//		if (binop->fLabel() == this->_fallLabel) {
//			this->_currentFunction->appendLabel(newLabel);
//		}
//	}
//	else if (binop->opr() == BinaryExpr::LOG_OR) {
//		auto newLabel = (binop->tLabel() != this->_fallLabel) ? binop->tLabel() : this->newLabel(this->_currentFunction->addr()->getName());
//
//		exp1->tLabel( newLabel );
//		exp1->fLabel( this->_fallLabel );
//		exp2->tLabel( binop->tLabel() );
//		exp2->fLabel( binop->fLabel() );
//
//		exp1->accept(this);
//		exp2->accept(this);
//
//		if (binop->tLabel() == this->_fallLabel) {
//			this->_currentFunction->appendLabel(newLabel);
//		}
//	}
//
//	/* We aren't inside a conditional, then we insert the labels
//	 * were we create the result of the conditionals. */
//	if (notCond) {
//		shared_ptr<STTempVar> res = this->newTemporary(Parser::INT);
//
//		//this->_currentFunction->appendLabel(binop->tLabel());
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::ScalarCopy(res, this->newConstant<int>(1)) ) );
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(binop->next()) ) );
//		this->_currentFunction->appendLabel(binop->fLabel());
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::ScalarCopy(res, this->newConstant<int>(0)) ) );
//
//		if (nextNull)
//			this->_currentFunction->appendLabel(binop->next());
//
//		binop->addr(res);
//	}
}

void AstTACGenVisitor::translateArithmeticExp(Parser::BinaryExpr* binop) {
	auto exp1 	 = binop->getExp1();
	auto exp2 	 = binop->getExp2();
	auto tgtType = binop->type();

	exp1->isExpLeftHand(binop->opr() == BinaryExpr::ASSIGN);
	exp1->accept(this);
	auto leftInstruction = this->_lastInstruction;

	exp2->isExpLeftHand(false);
	exp2->accept(this);
	auto rightInstruction = this->_lastInstruction;

	if (tgtType == Parser::INT) {
		switch (binop->opr()) {
			case BinaryExpr::ASSIGN: {
				if (exp1->isArrayAccess()) {
					this->_currentFunction->appendInstruction( make_shared<IR::CopyToArray>(leftInstruction, rightInstruction) );
					this->_lastInstruction = nullptr;
				}
				else {
					this->_currentFunction->appendInstruction( make_shared<IR::ScalarCopy>(leftInstruction, rightInstruction) );
					this->_lastInstruction = nullptr;
				}
				break;
			}
			case BinaryExpr::BIT_AND:
				this->_lastInstruction = make_shared<IR::BinAnd>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::BIT_OR:
				this->_lastInstruction = make_shared<IR::BinOr>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::BIT_XOR:
				this->_lastInstruction = make_shared<IR::BinXor>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::ADDITION:
				this->_lastInstruction = make_shared<IR::IAdd>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::SUBTRACTION:
				this->_lastInstruction = make_shared<IR::ISub>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::TIMES:
				this->_lastInstruction = make_shared<IR::IMul>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::DIV:
				this->_lastInstruction = make_shared<IR::IDiv>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::MOD:
				this->_lastInstruction = make_shared<IR::IMod>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::PLUS_EQUAL:
				this->_lastInstruction = make_shared<IR::IAdd>(leftInstruction, leftInstruction, rightInstruction);
				break;
			case BinaryExpr::MINUS_EQUAL:
				this->_lastInstruction = make_shared<IR::ISub>(leftInstruction, leftInstruction, rightInstruction);
				break;
			case BinaryExpr::TIMES_EQUAL:
				this->_lastInstruction = make_shared<IR::IMul>(leftInstruction, leftInstruction, rightInstruction);
				break;
			case BinaryExpr::DIV_EQUAL:
				this->_lastInstruction = make_shared<IR::IDiv>(leftInstruction, leftInstruction, rightInstruction);
				break;
			case BinaryExpr::MOD_EQUAL:
				this->_lastInstruction = make_shared<IR::IMod>(leftInstruction, leftInstruction, rightInstruction);
				break;
		}
	}
	else if (tgtType == Parser::FLOAT) {
		switch (binop->opr()) {
			case BinaryExpr::ASSIGN: {
				if (exp1->isArrayAccess()) {
					this->_currentFunction->appendInstruction( make_shared<IR::CopyToArray>(leftInstruction, rightInstruction) );
					this->_lastInstruction = nullptr;
				}
				else {
					this->_currentFunction->appendInstruction( make_shared<IR::ScalarCopy>(leftInstruction, rightInstruction) );
					this->_lastInstruction = nullptr;
				}
				break;
			}
			case BinaryExpr::ADDITION:
				this->_lastInstruction = make_shared<IR::FAdd>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::SUBTRACTION:
				this->_lastInstruction = make_shared<IR::FSub>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::TIMES:
				this->_lastInstruction = make_shared<IR::FMul>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::DIV:
				this->_lastInstruction = make_shared<IR::FDiv>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::PLUS_EQUAL:
				this->_lastInstruction = make_shared<IR::FAdd>(leftInstruction, leftInstruction, rightInstruction);
				break;
			case BinaryExpr::MINUS_EQUAL:
				this->_lastInstruction = make_shared<IR::FSub>(leftInstruction, leftInstruction, rightInstruction);
				break;
			case BinaryExpr::TIMES_EQUAL:
				this->_lastInstruction = make_shared<IR::FMul>(leftInstruction, leftInstruction, rightInstruction);
				break;
			case BinaryExpr::DIV_EQUAL:
				this->_lastInstruction = make_shared<IR::FDiv>(leftInstruction, leftInstruction, rightInstruction);
				break;
		}
	}
//	else if (tgtType == Parser::STRING) {
//		if (binop->opr() == BinaryExpr::ASSIGN) {
//			auto st 	   = this->_currentFunction->symbolTable();
//			newInstruction = new IR::Call(st->lookup(System::NAT_FUN_STRCPY), this->newTemporary(Parser::STRING));
//
//			newInstruction->addArgument( exp1->addr() );
//			newInstruction->addArgument( exp2->addr() );
//		}
//	}

	/* If we are inside a conditional we need to use the result
	 * of this expression to consider the jump targets. */
	if (binop->tLabel() != nullptr || binop->fLabel() != nullptr) {
		this->emitBranchesBasedOnExpValue(this->_lastInstruction, binop->tLabel(), binop->fLabel());
	}
}

void AstTACGenVisitor::translateRelationalExp(Parser::BinaryExpr* binop) {
//	IR::Instruction* newInstruction = nullptr;
//	auto exp1 	 = binop->getExp1();
//	auto exp2 	 = binop->getExp2();
//	auto tgtType = binop->type();
//
//	exp1->isExpLeftHand(false);
//	exp1->tLabel(nullptr);
//	exp1->fLabel(nullptr);
//	exp1->accept(this);
//
//	exp2->isExpLeftHand(false);
//	exp2->tLabel(nullptr);
//	exp2->fLabel(nullptr);
//	exp2->accept(this);
//
//	switch (binop->opr()) {
//		case BinaryExpr::COMPARE:
//			newInstruction = new IR::REqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
//			break;
//
//		case BinaryExpr::DIFFERENCE:
//			newInstruction = new IR::RNotEqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
//			break;
//
//		case BinaryExpr::LT:
//			newInstruction = new IR::RLesThan(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
//			break;
//
//		case BinaryExpr::LTE:
//			newInstruction = new IR::RLesThanEqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
//			break;
//
//		case BinaryExpr::GT:
//			newInstruction = new IR::RGreaterThan(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
//			break;
//
//		case BinaryExpr::GTE:
//			newInstruction = new IR::RGreaterThanEqual(this->newTemporary(tgtType), exp1->addr(), exp2->addr());
//			break;
//	}
//
//	binop->addr( newInstruction->tgt() );
//	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>(newInstruction) );
//
//	auto tLabel = binop->tLabel();
//	auto fLabel = binop->fLabel();
//	auto fall = this->_fallLabel;
//
//	/* If neither targets were informed then it is because this
//	 * expression is part of a larger expression and in this case
//	 * we cannot have short-circuit code yet. */
//	if (tLabel != nullptr && fLabel != nullptr) {
//		/* In this case jumps were informed for both targets, this
//		 * may happen when this expression is part of a larger expression
//		 * that is inside conditionals. */
//		if (tLabel != fall && fLabel != fall) {
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondTrueJump(binop->addr(), tLabel) ) );
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(fLabel) ) );
//		}
//		else if (tLabel != fall) {
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondTrueJump(binop->addr(), tLabel) ) );
//		}
//		else if (fLabel != fall) {
//			this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondFalseJump(binop->addr(), fLabel) ) );
//		}
//		else {
//			/* I do not expect this to happen, but better be
//			 * safe than sorry. */
//			throw System::EXCEPTION_UNREACHABLE_CODE;
//		}
//	}
}



void AstTACGenVisitor::emitBranchesBasedOnExpValue(Instruction_sptr result, shared_ptr<STLabelDef> lTrue, shared_ptr<STLabelDef> lFalse) {
//	auto cmpInstr = new IR::REqual(this->newTemporary(Parser::INT), result, this->newConstant<int>(0));
//	this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( cmpInstr ) );
//
//	if (lTrue != this->_fallLabel && lFalse != this->_fallLabel) {
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondTrueJump(cmpInstr->tgt(), lFalse) ) );
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::Jump(lTrue) ) );
//	}
//	else if (lTrue == this->_fallLabel) {
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondTrueJump(cmpInstr->tgt(), lFalse) ) );
//	}
//	else if (lFalse == this->_fallLabel) {
//		this->_currentFunction->appendInstruction( shared_ptr<IR::Instruction>( new IR::CondFalseJump(cmpInstr->tgt(), lTrue) ) );
//	}
//	else
//		throw System::EXCEPTION_UNREACHABLE_CODE;
}



template<typename T>
shared_ptr<STConstantDef> AstTACGenVisitor::newConstant(T value) {
	shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("_ct" + std::to_string(this->constCounter), value));

	this->_currentFunction->symbolTable()->add(cttEntry);
	this->constCounter++;

	return cttEntry;
}

shared_ptr<STLabelDef> AstTACGenVisitor::newLabel(string scope, string suffix) {
	int tempId = this->labelCounter++;

	return make_shared<STLabelDef>("_L" + scope + "." + suffix + std::to_string(tempId));
}

shared_ptr<IR::Register> AstTACGenVisitor::newTemporary(NativeType type) {
	int width  = AstSemaVisitor::typeWidth(type);

	auto tempVar = make_shared<STRegister>("_t" + std::to_string(this->tempCounter), type, width);

	this->_currentFunction->symbolTable()->add(tempVar);
	this->tempCounter++;

	return make_shared<IR::Register>(tempVar);
}

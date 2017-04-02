#include "AstVisitors.h"
#include "SymbolTable.h"

#include <memory>
#include <list>

using namespace Parser;

void AstTACGenVisitor::visit(Parser::CompilationUnit* astModule) {
	if (astModule->getSymbTable() == nullptr) {
		cout << "IRGen error:" << endl;
		cout << "\tThe SymbolTable for this module has not been computed!" << endl;
		return ;
	}

	/* Create a new TAC IR module. */
	this->_module = make_shared<IR::Module>();

	/* The IR module owns the symbol table. */
	this->_module->symbolTable(astModule->getSymbTable());

    cout << "Creating IR for module " << endl;

	/* Continue visiting children. */
	for (auto& astFunction : *astModule->getFunctions()) {
		/* An object representing the function that we are currently generating code for. */
		this->_currentFunction = make_shared<IR::Function>( astFunction->getSymbTable() );

		/* Reset the counters for temps/constants. */
		this->constCounter = 1;
		this->tempCounter = 1;
		this->_currentOffset = astFunction->currentOffset();
		this->_exitBasicBlock = this->newBasicBlock();

		/// Produce IR instructions for the function's components.
		astFunction->accept(this);

		// If there were any return instruction it points to the exitBasicBlock
		if (this->_exitBasicBlock->preds()->size() > 0)
			this->_currentFunction->appendBasicBlock( this->_exitBasicBlock );

		/* Add the function with its IR instructions to the module. */
		this->_module->addFunction(this->_currentFunction);
	}
}

void AstTACGenVisitor::visit(Parser::Function* astFunction) {
	if (astFunction->getSymbTable() == nullptr) {
		cout << "IRGen error:" << endl;
		cout << "\tThe SymbolTable for this function has not been computed!" << endl;
		return ;
	}

	/* The IR function should assume the ownership of the symbol table. */
	this->_currentFunction->symbolTable( astFunction->getSymbTable() );

	/* We need to keep a pointer to the current function definition, right? */
	this->_currentFunction->addr( astFunction->getSymbTable()->getParent()->lookup(astFunction->getName()) );

    cout << "Creating IR for function " << astFunction->getName() << endl;

	/* Produce IR instructions for the statements in the function's code block. */
	astFunction->getBody()->accept(this);
}

void AstTACGenVisitor::visit(const Parser::ParamDecl* param)
{ /* Nothing. */ }

void AstTACGenVisitor::visit(const Parser::VarSpec* spec) 
{ /* Nothing. */ }

void AstTACGenVisitor::visit(const Parser::VarDecl* astVarDec) {
	auto st = this->_currentFunction->symbolTable();

	for (auto& spec : *astVarDec->getVars()) {
		auto initializer = spec->getInitializer();

		/** If there is an initializer for the variable we produce instruction for it */
		if (initializer != nullptr) {
			auto varDeclRaw = dynamic_cast<STVariableDeclaration*>( st->lookup(spec->getName()).get() );
			auto memInstr 	= make_shared<IR::Memory>( shared_ptr<STVariableDeclaration>( varDeclRaw ) );

			initializer->accept(this);

			assert(this->_lastInstruction != nullptr && "Empty initializer for variable.");

			if (initializer->type() == Parser::INT) {
				this->_currentFunction->appendInstruction( make_shared<IR::ScalarCopy>(memInstr, this->_lastInstruction) );
				this->_lastInstruction = nullptr;
			}
			else {
				throw System::EXCEPTION_UNEXPECTED_TYPE;
			}
		}
	}
}

void AstTACGenVisitor::visit(Parser::LoopStmt* astLoopStmt) {
	auto begin 		= this->newBasicBlock();
	auto condition 	= astLoopStmt->getCondition();
	auto codeBlock 	= astLoopStmt->getBody();

	condition->tLabel( this->newBasicBlock() );
	condition->fLabel( astLoopStmt->next() );
	codeBlock->next( begin );

	/// For now we are using jumps in these cases where the code also fallthrough
	this->_currentFunction->appendInstruction( make_shared<IR::Jump>(begin) );

	/// The start of the basic block that computes the condition
	this->_currentFunction->appendBasicBlock(begin);

	/// The code to compute the condition
	condition->accept(this);

	/// The basic block that comprises the astLoopStmt body
	this->_currentFunction->appendBasicBlock(condition->tLabel());

	/// The code of the astLoopStmt body
	codeBlock->accept(this);

	/// We jump back to the condition computation
	this->_currentFunction->appendInstruction( make_shared<IR::Jump>(begin) );
}

void AstTACGenVisitor::visit(Parser::IfStmt* ifStmt) {
	auto condition   = ifStmt->getCondition();
	auto thenBlock   = ifStmt->getThenBlock();
	auto elseBlock 	 = ifStmt->getElseBlock();
	auto elseIfChain = ifStmt->getElseIfChain();

	/* Does not have else-if nor else blocks. */
	if ((elseIfChain == nullptr || elseIfChain->size() == 0) && elseBlock == nullptr) {
		condition->tLabel( this->newBasicBlock() );
		condition->fLabel( ifStmt->next() );
		thenBlock->next( ifStmt->next() );

		/// Emit the instructions for calculating the condition
		condition->accept(this);

		/// Basic block representing the then block
		this->_currentFunction->appendBasicBlock(condition->tLabel());
	
		/// Instructions of the "then block"
		thenBlock->accept(this);

		/// Jump from the "then block" to the basic block after the "if statement"
		this->_currentFunction->appendInstruction(make_shared<IR::Jump>(thenBlock->next()));
	}
	else if (elseIfChain == nullptr || elseIfChain->size() == 0) {
		/* Only have else block. */
		condition->tLabel( this->newBasicBlock() );
		condition->fLabel( this->newBasicBlock() );

		thenBlock->next( ifStmt->next() );
		elseBlock->next( ifStmt->next() );

		condition->accept(this);

		this->_currentFunction->appendBasicBlock(condition->tLabel());
		thenBlock->accept(this);
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(ifStmt->next()) );

		this->_currentFunction->appendBasicBlock(condition->fLabel());
		elseBlock->accept(this);
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(ifStmt->next()) );
	}
	else if (elseBlock == nullptr) {
		/* Only have else-if blocks. */
		auto nextCondLabel = this->newBasicBlock();

		condition->tLabel( this->newBasicBlock() );
		condition->fLabel( nextCondLabel );

		/* Append the first condition check */
		condition->accept(this);

		/* Append the instructions to the first "then-block" */
		this->_currentFunction->appendBasicBlock(condition->tLabel());
		thenBlock->accept(this);
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(ifStmt->next()) );

		unsigned int size = elseIfChain->size() - 1;
		unsigned int indx = 0;
		for (auto elseif : *elseIfChain) {
			/* Label to begin of the condition check. */
			this->_currentFunction->appendBasicBlock(nextCondLabel);

			/* In the last else-if we jump off of the if it's false. */
			nextCondLabel = (indx < size) ? this->newBasicBlock() : ifStmt->next();

			/* As usual.. */
			elseif->getCondition()->tLabel( this->newBasicBlock() );
			elseif->getCondition()->fLabel( nextCondLabel );

			/* Emit instructions to check the condition */
			elseif->getCondition()->accept(this);

			/* Then block appended. */
			this->_currentFunction->appendBasicBlock( elseif->getCondition()->tLabel() );
			elseif->getElseIfBlock()->accept(this);
			this->_currentFunction->appendInstruction( make_shared<IR::Jump>(ifStmt->next()) );

			/* We need to know when we are at the last position. */
			indx++;
		}
	}
	else {
		/* Has else and else-if blocks. */

		/* Only have else-if blocks. */
		auto nextCondLabel = this->newBasicBlock();
		auto elseLabel = this->newBasicBlock();

		condition->tLabel( this->newBasicBlock() );
		condition->fLabel( nextCondLabel );

		/* Append the first condition check. */
		condition->accept(this);

		/* Then block appended. */
		this->_currentFunction->appendBasicBlock(condition->tLabel());
		thenBlock->accept(this);
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(ifStmt->next()) );

		unsigned int size = elseIfChain->size() - 1;
		unsigned int indx = 0;
		for (auto elseif : *elseIfChain) {
			/* Label to begin of the condition check. */
			this->_currentFunction->appendBasicBlock(nextCondLabel);

			/* In the last else-if we jump off of the if it's false. */
			nextCondLabel = (indx < size) ? this->newBasicBlock() : elseLabel;

			/* As usual.. */
			elseif->getCondition()->tLabel( this->newBasicBlock() );
			elseif->getCondition()->fLabel( nextCondLabel );

			/* Emit instructions to check the condition */
			elseif->getCondition()->accept(this);

			/* Then block appended. */
			this->_currentFunction->appendBasicBlock( elseif->getCondition()->tLabel() );
			elseif->getElseIfBlock()->accept(this);
			this->_currentFunction->appendInstruction( make_shared<IR::Jump>(ifStmt->next()) );

			/* We need to know when we are at the last position. */
			indx++;
		}

		/* The else part. */
		this->_currentFunction->appendBasicBlock(elseLabel);
		elseBlock->accept(this);
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(ifStmt->next()) );
	}
}

void AstTACGenVisitor::visit(Parser::ElseIfStmt* elseIfStmt) { }

void AstTACGenVisitor::visit(const Parser::ReturnStmt* ret) {
	Expression* exp = ret->getExpression();

	exp->isExpLeftHand(false);
	exp->accept(this);

	assert(this->_lastInstruction && "Null instruction at return statement.");

	this->_currentFunction->appendInstruction( make_shared<IR::Return>(this->_lastInstruction, this->_exitBasicBlock) );
	this->_currentFunction->appendBasicBlock( this->newBasicBlock() );

	this->_lastInstruction = nullptr;
}

void AstTACGenVisitor::visit(Parser::CodeBlock* astCodeBlock) {
	if (astCodeBlock->getSymbTable() == nullptr) {
		cout << "IRGen error:" << endl;
		cout << "\tThe SymbolTable for this block has not been computed!" << endl;
		return ;
	}

	// The IR function symbol table has all definitions of the function's blocks.
	this->_currentFunction->addSymbolTable(astCodeBlock->getSymbTable());

    cout << "Creating IR for codeblock " << endl;

	// Produce IR for each statement in the code block.
	for (auto& statement : *astCodeBlock->getStatements()) {
		// This label is used in situations that is necessary to jump directoy to the 
		// next instruction. E.g., when are parsing a boolean expression. 
		statement->next( this->newBasicBlock() );

		// Produce code for the expression/statement 
		statement->accept(this);

		// If we parsed an instruction that could be part of a larger expressionn, but
		// in fact it was not inside one then we add that instruction as a subtree.
		if (this->_lastInstruction != nullptr) {
			this->_currentFunction->appendInstruction(this->_lastInstruction);
			this->_lastInstruction = nullptr;
		}

		// If some instruction branch to statement->next() then we append it. 
		if (statement->next()->usageCounter() > 0)
			this->_currentFunction->appendBasicBlock( statement->next() );
	}
}

void AstTACGenVisitor::visit(Parser::IntegerExpr* integer) {
	/* If we are not inside a conditional expression. */
	if ( integer->tLabel() == nullptr || integer->fLabel() == nullptr ) {
		auto immInst = make_shared<IR::Immediate>( newConstant(integer->value()) );
		auto regDest = newTemporary(integer->type());

		this->_lastInstruction = make_shared<IR::ScalarCopy>(regDest, immInst);
	}
	else {
		auto cttEntry = newConstant(integer->value());
		auto immInstr = make_shared<IR::Immediate>(cttEntry);

		this->_currentFunction->appendInstruction( make_shared<IR::Conditional>(immInstr, integer->tLabel(), integer->fLabel()) );
		this->_lastInstruction = nullptr;
	}
}

void AstTACGenVisitor::visit(Parser::IdentifierExpr* id) {
	/* We look into the symbol table to find the dimensions sizes. */
	auto entry 	= this->_currentFunction->symbolTable()->lookup(id->value());
	auto decl 	= dynamic_cast<STVariableDeclaration*>( entry.get() );

	if (decl->dims().size() > 0) {
		/* Tells parent "visiting" methods that this is an array access. */
		id->isArrayAccess(true);

		/* If no dimensions were specified then we just return the base addr of the array. */
		if (id->dimsExprs()->size() != decl->dims().size()) {
			auto varEntryInstr 		= make_shared<IR::Memory>( shared_ptr<Parser::STVariableDeclaration>(decl) );
			this->_lastInstruction 	= make_shared<IR::Addr>(this->newTemporary(Parser::INT), varEntryInstr);
			return ;
		}

		/* Here we are going to store the factors for each index. */
		vector<Instruction_sptr> indExpsInstructions;

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

		/**
		 * Now we sum the address of the base of the array with the number of bytes we
		 * computed from the index*base_type.
		 */
		tempResult	= this->newTemporary(Parser::INT);
		auto arrAccess 	= make_shared<IR::AddrDispl>(tempResult, entryAddr, indAccess);

		/* If we are parsing a right-hand side expression we will de-refer the pointer and load
		 * the array value in a temporary. Otherwise, we will just return the address to the previous
		 * expression, and it will take care of dereferencing. 									   */
		if (id->isExpLeftHand()) {
			this->_lastInstruction = arrAccess;
		}
		else {
			tempResult	 = this->newTemporary(id->type());
			auto derefer = make_shared<IR::CopyFromArray>(tempResult, arrAccess);
			this->_lastInstruction = derefer;
		}
	}
	else {
		id->isArrayAccess(false);

		if (id->isExpLeftHand()) {
			this->_lastInstruction = make_shared<IR::Memory>( shared_ptr<STVariableDeclaration>(decl) );
		}
		else {
			auto memAccInstr = make_shared<IR::Memory>( shared_ptr<STVariableDeclaration>(decl) );
			auto destReg = this->newTemporary(id->type());

			this->_lastInstruction = make_shared<IR::ScalarCopy>(destReg, memAccInstr);
		}
	}

	if (id->tLabel() != nullptr && id->fLabel() != nullptr) {
		auto entry 	= this->_currentFunction->symbolTable()->lookup(id->value());
		auto type 	= entry->type();

		/* Only numerical values are acceptable in boolean comparissons. */
		if (type == Parser::INT) {
			this->emitBranchesBasedOnExpValue(this->_lastInstruction, id->tLabel(), id->fLabel());
		}
		else {
			throw System::EXCEPTION_UNEXPECTED_TYPE;
		}
	}
}

void AstTACGenVisitor::visit(Parser::FunctionCall* funCall) {
	auto decl 		= this->_currentFunction->symbolTable()->lookup(funCall->name());
	auto funDecPtr 	= shared_ptr<STFunctionDeclaration>( dynamic_cast<STFunctionDeclaration*>(decl.get()) );
	auto irFun  	= make_shared<IR::Func>(funDecPtr);
	auto tmpRes 	= this->newTemporary( decl->type() );

	auto funCallInstr = make_shared<IR::Call>(tmpRes, irFun);

	for (auto argument : *funCall->arguments()) {
		/* Visit the argument, produce address for them.*/
		argument->accept(this);

		/* Add the argument to the function call. */
		funCallInstr->addArgument(this->_lastInstruction);
		this->_lastInstruction = nullptr;
	}

	this->_lastInstruction = funCallInstr;
}

void AstTACGenVisitor::visit(Parser::UnaryExpr* unary) {
	if (unary->opr() == UnaryExpr::NOT) {
		translateBooleanExp(unary);
	}
	else if (unary->opr() == UnaryExpr::ADDR) {
		unary->exp()->isExpLeftHand(false);
		unary->exp()->accept(this);
		auto rightInstruction = this->_lastInstruction;

		this->_lastInstruction = make_shared<IR::Addr>(this->newTemporary(unary->type()), rightInstruction);
	}
	else if (unary->opr() == UnaryExpr::BIT_NOT) {
		unary->exp()->isExpLeftHand(false);
		unary->exp()->accept(this);
		auto rightInstruction = this->_lastInstruction;

		this->_lastInstruction = make_shared<IR::BinNot>(this->newTemporary(unary->type()), rightInstruction);
	}
	else {
		translateArithmeticExpr(unary);
	}
}

void AstTACGenVisitor::translateBooleanExp(Parser::UnaryExpr* unary) {
	auto topLevel = false;
	auto nextNull = false;
	BasicBlock_sptr endBb = nullptr;

	/* If the jump labels are nullptr then we are not decoding a boolean
	 * expression that is inside a if statement, thus it is an expression
	 * and we need to take care to create the appropriate labels to storing
	 * the true and false values. */
	if (unary->tLabel() == nullptr && unary->fLabel() == nullptr) {
		topLevel = true;
		endBb = this->newBasicBlock();

		unary->tLabel( this->newBasicBlock() );
		unary->fLabel( this->newBasicBlock() );
	}

	unary->exp()->isExpLeftHand(false);
	unary->exp()->tLabel( unary->fLabel() );
	unary->exp()->fLabel( unary->tLabel() );
	unary->exp()->accept(this);
	this->_lastInstruction = nullptr;

	if (topLevel) {
		auto regResT = this->newTemporary(Parser::INT);
		auto regResF = make_shared<IR::Register>( regResT->value() );
		auto regResE = make_shared<IR::Register>( regResT->value() );
		auto zero 	 = make_shared<IR::Immediate>( this->newConstant(0) );
		auto one 	 = make_shared<IR::Immediate>( this->newConstant(1) );

		this->_currentFunction->appendBasicBlock(unary->tLabel());
		this->_currentFunction->appendInstruction( make_shared<IR::ScalarCopy>(regResT, one) );
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(endBb) );
		this->_currentFunction->appendBasicBlock(unary->fLabel());
		this->_currentFunction->appendInstruction( make_shared<IR::ScalarCopy>(regResF, zero) );
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(endBb) );
		this->_currentFunction->appendBasicBlock(endBb);
		this->_lastInstruction = regResE;
	}
}

void AstTACGenVisitor::translateArithmeticExpr(Parser::UnaryExpr* unary) {
	Expression* exp 	= unary->exp();
	NativeType tgtType 	= unary->type();

	exp->isExpLeftHand(false);
	exp->accept(this);
	auto rightInstruction = this->_lastInstruction;

	if (tgtType == Parser::INT) {
		switch (unary->opr()) {
			case UnaryExpr::MINUS:
				this->_lastInstruction = make_shared<IR::IMinus>(this->newTemporary(tgtType), rightInstruction);
				break;

			case UnaryExpr::INCREMENT:
				this->_lastInstruction = make_shared<IR::IInc>(rightInstruction->tgt(), rightInstruction);
				break;

			case UnaryExpr::DECREMENT:
				this->_lastInstruction = make_shared<IR::IDec>(rightInstruction->tgt(), rightInstruction);
				break;
		}
	}
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
	auto topLevel  = false;
	auto nextNull  = false;
	BasicBlock_sptr end = nullptr;

	exp1->isExpLeftHand(false);
	exp2->isExpLeftHand(false);

	/* If the jump labels are nullptr then we are not decoding a boolean
	 * expression that is inside a if statement, thus it is an expression
	 * and we need to take care to create the appropriate labels to storing
	 * the true and false values. */
	if (binop->tLabel() == nullptr && binop->fLabel() == nullptr) {
		topLevel = true;

		binop->tLabel( this->newBasicBlock() );
		binop->fLabel( this->newBasicBlock() );

		end = this->newBasicBlock();
	}

	if (binop->opr() == BinaryExpr::LOG_OR) {
		exp1->tLabel( binop->tLabel() );
		exp1->fLabel( this->newBasicBlock() );
		exp1->accept(this);

		exp2->tLabel( binop->tLabel() );
		exp2->fLabel( binop->fLabel() );

		this->_currentFunction->appendBasicBlock( exp1->fLabel() );

		exp2->accept(this);
	 }
	 else if (binop->opr() == BinaryExpr::LOG_AND) {
		exp1->tLabel( this->newBasicBlock() );
		exp1->fLabel( binop->fLabel() );
		exp1->accept(this);

		exp2->tLabel( binop->tLabel() );
		exp2->fLabel( binop->fLabel() );

		this->_currentFunction->appendBasicBlock( exp1->tLabel() );

		exp2->accept(this);
	}

	/* We aren't inside a conditional, then we insert the labels
	 * were we create the result of the conditionals. */
	if (topLevel) {
		auto resT  = this->newTemporary(Parser::INT);
		auto resF  = make_shared<IR::Register>( resT->value() );
		auto resE  = make_shared<IR::Register>( resT->value() );
		auto zero = make_shared<IR::Immediate>( this->newConstant(0) );
		auto one  = make_shared<IR::Immediate>( this->newConstant(1) );

		this->_currentFunction->appendBasicBlock(binop->tLabel());
		this->_currentFunction->appendInstruction( make_shared<IR::ScalarCopy>(resT, one) );
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(end) );

		this->_currentFunction->appendBasicBlock(binop->fLabel());
		this->_currentFunction->appendInstruction( make_shared<IR::ScalarCopy>(resF, zero) );
		this->_currentFunction->appendInstruction( make_shared<IR::Jump>(end) );

		this->_currentFunction->appendBasicBlock(end);
		this->_lastInstruction = resE;
	}
}

void AstTACGenVisitor::translateArithmeticExp(Parser::BinaryExpr* binop) {
	auto exp1 	 = binop->getExp1();
	auto exp2 	 = binop->getExp2();
	auto tgtType = binop->type();

	exp1->isExpLeftHand(binop->opr() == BinaryExpr::ASSIGN);
	exp1->next(binop->next());
	exp1->accept(this);
	auto leftInstruction = this->_lastInstruction;

	exp2->isExpLeftHand(false);
	exp2->next(binop->next());
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
				this->_lastInstruction = make_shared<IR::IAdd>(leftInstruction->tgt(), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::MINUS_EQUAL:
				this->_lastInstruction = make_shared<IR::ISub>(leftInstruction->tgt(), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::TIMES_EQUAL:
				this->_lastInstruction = make_shared<IR::IMul>(leftInstruction->tgt(), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::DIV_EQUAL:
				this->_lastInstruction = make_shared<IR::IDiv>(leftInstruction->tgt(), leftInstruction, rightInstruction);
				break;
			case BinaryExpr::MOD_EQUAL:
				this->_lastInstruction = make_shared<IR::IMod>(leftInstruction->tgt(), leftInstruction, rightInstruction);
				break;
		}
	}

	/* If we are inside a conditional we need to use the result
	 * of this expression to consider the jump targets. */
	if (binop->tLabel() != nullptr || binop->fLabel() != nullptr) {
		this->emitBranchesBasedOnExpValue(this->_lastInstruction, binop->tLabel(), binop->fLabel());
	}
}

void AstTACGenVisitor::translateRelationalExp(Parser::BinaryExpr* binop) {
	auto exp1 	 = binop->getExp1();
	auto exp2 	 = binop->getExp2();
	auto tgtType = binop->type();

	exp1->isExpLeftHand(false);
	exp1->tLabel(nullptr);
	exp1->fLabel(nullptr);
	exp1->accept(this);
	auto leftInstruction = this->_lastInstruction;

	exp2->isExpLeftHand(false);
	exp2->tLabel(nullptr);
	exp2->fLabel(nullptr);
	exp2->accept(this);
	auto rightInstruction = this->_lastInstruction;

	switch (binop->opr()) {
		case BinaryExpr::COMPARE:
			this->_lastInstruction = make_shared<IR::REqual>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
			break;

		case BinaryExpr::DIFFERENCE:
			this->_lastInstruction = make_shared<IR::RNotEqual>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
			break;

		case BinaryExpr::LT:
			this->_lastInstruction = make_shared<IR::RLesThan>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
			break;

		case BinaryExpr::LTE:
			this->_lastInstruction = make_shared<IR::RLesThanEqual>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
			break;

		case BinaryExpr::GT:
			this->_lastInstruction = make_shared<IR::RGreaterThan>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
			break;

		case BinaryExpr::GTE:
			this->_lastInstruction = make_shared<IR::RGreaterThanEqual>(this->newTemporary(tgtType), leftInstruction, rightInstruction);
			break;
	}

	auto tLabel = binop->tLabel();
	auto fLabel = binop->fLabel();

	// If neither targets were informed then it is because this
	// expression is part of a larger expression and in this case
	// we cannot have short-circuit code yet.
	if (tLabel != nullptr && fLabel != nullptr) {
		this->_currentFunction->appendInstruction( make_shared<IR::Conditional>(this->_lastInstruction, tLabel, fLabel) );
		this->_lastInstruction = nullptr;
	}
}



void AstTACGenVisitor::emitBranchesBasedOnExpValue(Instruction_sptr result, BasicBlock_sptr lTrue, BasicBlock_sptr lFalse) {
	auto immediate = make_shared<IR::Immediate>( this->newConstant(0) );
	auto cmpInstr = make_shared<IR::RNotEqual>(this->newTemporary(Parser::INT), result, immediate);

	this->_currentFunction->appendInstruction( make_shared<IR::Conditional>(cmpInstr, lTrue, lFalse) );
}

shared_ptr<STConstantDef> AstTACGenVisitor::newConstant(int value) {
	shared_ptr<Parser::STConstantDef> cttEntry(new Parser::STConstantDef("_ct" + std::to_string(this->constCounter), value));

	this->_currentFunction->symbolTable()->add(cttEntry);
	this->constCounter++;

	return cttEntry;
}

shared_ptr<IR::Register> AstTACGenVisitor::newTemporary(NativeType type) {
	int width  = AstSemaVisitor::typeWidth(type);

	auto tempVar = make_shared<STRegister>("_t" + std::to_string(this->tempCounter), type, width);

	this->_currentFunction->symbolTable()->add(tempVar);
	this->tempCounter++;

	return make_shared<IR::Register>(tempVar);
}


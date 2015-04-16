#include "Semantikin.h"
#include "AstVisitors.h"
#include "SymbolTable.h"
#include <memory>
#include <list>

/*
 * These are the checks performed:
 * 	 1. adicionar declarações.										// OK
 * 	 2. verificar uso de simbolos inexistentes.						// ok
 * 	 3. checagem de tipos.											// ok
 * 	 4. indexacao com ponto flutuante.								// ok
 * 	 5. usando variável como chamada de função.						// ok
 * 	 6. usando função como variável.								// ok
 * 	 7. tipo de argumentos											// ok
 * 	 8. quantidade correta de argumentos.							// ok
 * 	 9. size of array dimensions must be integers contants.			// ok
 * 	10. check (non-)existence of return statements and its type.	// ok
 *	11. Check if we aren't taking the address of a non-lefthand exp.// ok
 */

using namespace Parser;

#ifndef CHECK_TYPE
	#define CHECK_TYPE(_exp_, _tp_) ((_exp_)->type() == (Parser::_tp_))
#endif

#ifndef IS_SUBTYPE
	#define IS_SUBTYPE(tp1, tp2)	((tp1 == tp2) ||										\
									((tp1 != Parser::STRING && tp2 != Parser::STRING) && 	\
									(tp1 != Parser::VOID && tp2 != Parser::VOID) && 		\
									(tp1 <= tp2)))
#endif

void AstSemaVisitor::visit(Parser::CompilationUnit* module) {
	if (module->getSymbTable() != nullptr) {
		cout << "The SymbolTable for this module has already been computed. " << endl;
		return ;
	}

	/* Create new symbol table for this scope. */
	shared_ptr<SymbolTable> table(new SymbolTable(nullptr));

	/* Set the symbol table in the module. */
	module->setSymbTable(table);

	/* Add all native global variables and functions. */
	addNativeFunctions(table);

	/* Update pointer to new current symbol table. */
	this->currentSymbTable = table;

	/* Continue visiting children. */
	list<shared_ptr<Parser::Function>>::iterator it = module->getFunctions()->begin();
	while (it != module->getFunctions()->end()) {
		(*it)->accept(this);
		it++;
	}

	/* Reset pointer to previous symbol table. */
	this->currentSymbTable = table->getParent();
}

void AstSemaVisitor::visit(Parser::Function* function) {
	if (function->getSymbTable() != nullptr) {
		cout << "The SymbolTable for this function has already been computed. " << endl;
		return ;
	}

	string type = function->getReturnType();
	string name = function->getName();

	/* Check if there isn't already a symbol table entry with this name. */
	shared_ptr<SymbolTableEntry> prev = this->currentSymbTable->lookup(name);

	if (prev != nullptr) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tRedefinition of the symbol \"" << name << "\"." << endl;
		exit(-1);
	}

	if (translateType(type) == Parser::NOT_A_TYPE) {
		cout << "Error in semantic analysis." << endl;
		cout << "\t The function \"" << name << "\" has a invalid return type (\"" << type << "\")." << endl;
		exit(-1);
	}

	/* Insert the function definition in the symbol table. */
	shared_ptr<Parser::STFunctionDeclaration> funDecl(new Parser::STFunctionDeclaration(name, name, translateType(type)));
	this->currentSymbTable->add(funDecl);

	/* Save pointer to declaration of current function.
	 * This will be handy for debugging. */
	this->_curFunDecl = funDecl;

	/* Offset is always relative to the function frame,
	 * so we reset when we enter a function. */
	this->_currentOffset = 0;

	/* Reset the counter of number of return statements of this function. */
	this->currentFunReturns = 0;

	/* Set the (inherited attribute) representing the return type of the
	 * current function. The analyze of return statements with check this
	 * type.												  		   */
	this->currentFunRetType = translateType(type);

	/* Create new symbol table for this scope. */
	shared_ptr<SymbolTable> table(new SymbolTable(currentSymbTable));

	/* Set the symbol table in the function. */
	function->setSymbTable(table);

	/* Update pointer to new current symbol table. */
	this->currentSymbTable = table;

	/* Add the node of parameters if actually there is parameters. */
	list<shared_ptr<ParamDecl>>::iterator itt = function->getParams()->begin();
	for (itt = function->getParams()->begin(); itt != function->getParams()->end(); itt++) {
		/* The parameter is added to the symbol table. */
		(*itt)->accept(this);

		/* Now set set in the Function entry in the ST a pointer to its parameter entry in the ST. */
		funDecl->addParam( this->currentSymbTable->lookup((*itt)->getName()) );
	}

	/* Continue the visiting. */
	function->getBody()->accept(this);

	/* Check if return type and return statements make sense. */
	if (this->currentFunRetType != Parser::VOID && this->currentFunReturns == 0) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tMissing return statement in function (\"" << name << "\") returning non-void." << endl;
		exit(-1);
	}

	/* We save this because when we will emit temporaries we want their indices continue
	 * after the indices of local vars and parameters. */
	function->currentOffset(this->_currentOffset);

	/* Reset pointer to previous symbol table. */
	this->currentSymbTable = table->getParent();
}

void AstSemaVisitor::visit(const Parser::ParamDecl* param) {
	list<shared_ptr<Parser::Expression>>::iterator it;

	string type = param->getType();
	string name = param->getName();

	if (!isValidType(type)) {
		cout << "Error in semantic analysis." << endl;
		cout << "\t\"" << type << "\" is not a valid type." << endl;
		exit(-1);
	}

	shared_ptr<SymbolTableEntry> prev = this->currentSymbTable->lookup(name);

	if (prev != nullptr) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tThere is a previous definition of the symbol \"" << name << "\"." << endl;
		exit(-1);
	}

	/* Calculate the size of the variable and check if the array
	 * dimensions were specified with constants. 			  */
	int varSize = variableSize(typeWidth(translateType(type)), param->getDims());

	if (varSize == 0) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tThe size of array dimensions should be integer constants." << endl;
		exit(-1);
	}

	/* Multidimentional parameters are passed only as pointers, so we just need a pointer.*/
	if (param->getDims() != nullptr && param->getDims()->size() > 0)
		varSize = System::POINTER_SIZE;

	/* Create the parameter definition. */
	auto parDecl = make_shared<Parser::STParamDecl>(name, translateType(type), varSize, this->_currentOffset);

	/* Collect the number and size of each dimension. */
	for (auto _par : *param->getDims()) {
		IntegerExpr *expr = dynamic_cast<IntegerExpr*>(_par.get());
		parDecl->addDim(expr->value());
	}

	/* Include the parameter definition in the ST. */
	this->currentSymbTable->add(parDecl);

	this->_currentOffset += varSize;
}

void AstSemaVisitor::visit(const Parser::VarSpec* spec) {
}

void AstSemaVisitor::visit(const Parser::VarDecl* varDec) {
	string type 									= varDec->getType();
	list<shared_ptr<Parser::VarSpec>>* specs 		= varDec->getVars();
	list<shared_ptr<Parser::VarSpec>>::iterator it 	= specs->begin();

	while (it != specs->end()) {
		string name = (*it)->getName();
		shared_ptr<SymbolTableEntry> prev = this->currentSymbTable->lookup(name);

		if (!isValidType(type)) {
			cout << "Error in semantic analysis." << endl;
			cout << "\t\"" << type << "\" is not a valid type." << endl;
			exit(-1);
		}

		if (prev != nullptr) {
			cout << "Error in semantic analysis." << endl;
			cout << "\tThere is a previous definition of the symbol \"" << name << "\"." << endl;
			exit(-1);
		}

		int dims = 0, varSize = typeWidth(translateType(type));

		if ((*it)->getDimsExpr() != nullptr) {
			dims = (*it)->getDimsExpr()->size();
			varSize = variableSize(typeWidth(translateType(type)), (*it)->getDimsExpr());
		}

		if ((*it)->getInitializer() != nullptr) {
			Expression* initializer = (*it)->getInitializer();

			initializer->accept(this);

			if ( !IS_SUBTYPE(initializer->type(), translateType(type)) ) {
				cout << "Error in semantic analysis." << endl;
				cout << "\tThe initializer of the variable \"" << name << "\" is not a subtype of \"" << type << "\"." << endl;
				exit(-1);
			}
		}

		/* Create the parameter */
		shared_ptr<Parser::STLocalVarDecl> param(new Parser::STLocalVarDecl(name, translateType(type), varSize, this->_currentOffset));

		/* Collect the number and size of each dimension. */
		if ((*it)->getDimsExpr() != nullptr) {
			for (auto _par : *(*it)->getDimsExpr()) {
				IntegerExpr *expr = dynamic_cast<IntegerExpr*>(_par.get());

				if (expr == nullptr) {
					cout << "Error in semantic analysis." << endl;
					cout << "\tDimensions size must be integer constants." << endl;
					exit(-1);
				}

				param->addDim(expr->value());
			}
		}

		/* Add the parameter to the current ST. */
		this->currentSymbTable->add(param);

		this->_currentOffset += varSize;

		it++;
	}
}

void AstSemaVisitor::visit(Parser::LoopStmt* loop) {
	/* Continue visiting. */
	loop->getCondition()->accept(this);

	if (loop->getBody() != nullptr)
		loop->getBody()->accept(this);
}

void AstSemaVisitor::visit(Parser::IfStmt* ifStmt) {
	Parser::Expression* condition 						= ifStmt->getCondition();
	Parser::CodeBlock* thenBlock 						= ifStmt->getThenBlock();
	list<shared_ptr<Parser::ElseIfStmt>>* elseIfChain 	= ifStmt->getElseIfChain();
	Parser::CodeBlock* elseBlock 						= ifStmt->getElseBlock();

	/* Continue visiting. */
	condition->accept(this);

	if (thenBlock != nullptr)
		thenBlock->accept(this);

	if (elseIfChain != nullptr) {
		list<shared_ptr<Parser::ElseIfStmt>>::iterator it = elseIfChain->begin();
		while (it != elseIfChain->end()) {
			(*it)->accept(this);
			it++;
		}
	}

	if (elseBlock != nullptr)
		elseBlock->accept(this);
}

void AstSemaVisitor::visit(Parser::ElseIfStmt* elseIfStmt) {
	/* Continue visiting. */
	elseIfStmt->getCondition()->accept(this);

	if (elseIfStmt->getElseIfBlock() != nullptr)
		elseIfStmt->getElseIfBlock()->accept(this);
}

void AstSemaVisitor::visit(const Parser::ReturnStmt* ret) {
	if (this->currentFunRetType == Parser::VOID) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tReturn statement present in function with void return type." << endl;
		exit(-1);
	}

	/* Continue visiting */
	ret->getExpression()->accept(this);

	if ( ! IS_SUBTYPE(ret->getExpression()->type(), this->currentFunRetType) ) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tA return-statement's expression in function (\"" << this->_curFunDecl->getName()
				<< "\") is not a subtype of the function return type." << endl;
		exit(-1);
	}

	/* Increment number of valid return statements. */
	this->currentFunReturns++;
}

void AstSemaVisitor::visit(Parser::CodeBlock* block) {
	if (block->getSymbTable() != nullptr) {
		cout << "The SymbolTable for this block has already been computed. " << endl;
		return ;
	}

	/* Create new symbol table for this scope. */
	shared_ptr<SymbolTable> table(new SymbolTable(this->currentSymbTable));

	/* Set the symbol table in the function. */
	block->setSymbTable(table);

	/* Update pointer to new current symbol table. */
	this->currentSymbTable = table;

	/* Continue the visiting. */
	list<shared_ptr<Parser::Statement>>* statements = block->getStatements();

	if (statements != nullptr) {
		list<shared_ptr<Parser::Statement>>::iterator it = statements->begin();
		while (it != statements->end()) {
			(*it)->accept(this);
			it++;
		}
	}

	/* Reset pointer to previous symbol table. */
	this->currentSymbTable = table->getParent();
}

void AstSemaVisitor::visit(Parser::StringExpr* str) {
	str->type(Parser::STRING);
}

void AstSemaVisitor::visit(Parser::FloatExpr* flt) {
	flt->type(Parser::FLOAT);
}

void AstSemaVisitor::visit(Parser::IntegerExpr* integer) {
	integer->type(Parser::INT);
}

void AstSemaVisitor::visit(Parser::IdentifierExpr* id) {
	string name = id->value();
	shared_ptr<SymbolTableEntry> decl = this->currentSymbTable->lookup(name);
	list<shared_ptr<Parser::Expression>>* dims = id->dimsExprs();

	/* If the symbol was not declared is a error. */
	if (decl == nullptr) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tThere is not a definition of the symbol \"" << name << "\"." << endl;
		exit(-1);
	}

	/* If the symbol is of the wrong type is not a variable is a error. */
	if (dynamic_cast<STFunctionDeclaration*>(decl.get())) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tThe symbol \"" << name << "\" is a function, a variable was expected." << endl;
		exit(-1);
	}

	/* Begin: Check matrix and variables indexing constraints. */
	STVariableDeclaration* varDecl = dynamic_cast<STVariableDeclaration*>(decl.get());

	/* Specified indices for acessing scalar variable. */
	if (varDecl->getNumDims() == 0 && dims != nullptr && dims->size() > 0) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tVariable \"" << name << "\" cannot be indexed as an array." << endl;
		exit(-1);
	}

	/* It was specified more dimensions than necessary. */
	if (varDecl->getNumDims() < dims->size()) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tMismatch in the number of dimensions used to access the symbol \"" << name << "\"." << endl;
		exit(-1);
	}

	/* Just the initial indices were specified, fill the remainder wit [0]. */
//	if ((varDecl->getNumDims() > 0 && dims == nullptr) || (varDecl->getNumDims() > dims->size())) {
//		int remainder = (dims == nullptr) ? varDecl->getNumDims() : varDecl->getNumDims() - dims->size();
//
//		for (int i=0; i<remainder; i++)
//			dims->push_back(shared_ptr<Parser::IntegerExpr>(new Parser::IntegerExpr(location(), 0)));
//	}

	/* Continue visiting. */
	if (dims != nullptr) {
		list<shared_ptr<Parser::Expression>>::iterator it = dims->begin();
		while (it != dims->end()) {
			(*it)->accept(this);
			it++;
		}

		/* Check if the expressions used to index the array are all constants. */
		for (it = dims->begin(); it != dims->end(); it++) {
			if ( ! CHECK_TYPE((*it), INT) ) {
				cout << "Error in semantic analysis." << endl;
				cout << "\tYou must index arrays using integer values.." << endl;
				exit(-1);
			}
		}
	}

	/* Register to which symbol table entry this 'id' refers to. This will
	 * be helpfull when generating intermediate representation. 		*/
	id->addr(decl);

	/* This reference to a variable has the type of the variable. */
	id->type(varDecl->type());
}

void AstSemaVisitor::visit(Parser::FunctionCall* funCall) {
	string name = funCall->name();
	shared_ptr<SymbolTableEntry> decl 					= this->currentSymbTable->lookup(name);
	list<shared_ptr<Parser::Expression>>* arguments 	= funCall->arguments();
	list<shared_ptr<Parser::Expression>>::iterator it 	= arguments->begin();

	/* If the symbol was not declared is a error. */
	if (decl == nullptr) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tThere is not a definition of the symbol \"" << name << "\"." << endl;
		exit(-1);
	}

	/* Check if there is a *function* definition. */
	STFunctionDeclaration *funDecl = dynamic_cast<STFunctionDeclaration*>(decl.get());
	if (funDecl == nullptr) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tThere is an attempt to use the symbol \"" << name << "\" as a function but it's declared type is variable." << endl;
		exit(-1);
	}

	/* Check number of parameters. */
	if (funDecl->params().size() != funCall->arguments()->size()) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tIncorrect number of parameters were passed to function \"" << name << "\"." << endl;
		cout << "\t" << funDecl->params().size() << " were expected, but " << arguments->size() << " was passed." << endl;
		exit(-1);
	}

	/* Continue visiting. */
	if (arguments != nullptr) {
		list<shared_ptr<Parser::Expression>>::iterator it = arguments->begin();
		while (it != arguments->end()) {
			(*it)->accept(this);
			it++;
		}
	}

	/* Need to check the type of the parameters. */
	if (arguments != nullptr) {
		list<shared_ptr<Parser::Expression>>::iterator it = arguments->begin();
		const vector<shared_ptr<SymbolTableEntry>> parameters = funDecl->params();

		for (int i=0; it != arguments->end(); it++, i++) {
			Parser::Expression* exp = it->get();
			STParamDecl* param = dynamic_cast<STParamDecl*>(parameters[i].get());

			/* If the argument is not a subtype of the parameter type it is an error. */
			if ( ! IS_SUBTYPE(exp->type(), param->type()) ) {
				cout << "Error in semantic analysis." << endl;
				cout << "\tThe " << (i+1) << "'th parameter of the function \"" << funDecl->getName() << "\" is a \"" << Util::typeName(param->type()) << "\"";
				cout << " but an \"" << Util::typeName(exp->type()) << "\" was informed." << endl;
				exit(-1);
			}
		}
	}

	/* Register to which symbol table entry this 'funCall' refers to.
	 * This will be helpfull when generating intermediate representation. */
	funCall->addr(decl);

	/* The type of the function call is the function return type. */
	funCall->type(funDecl->type());
}

void AstSemaVisitor::visit(Parser::UnaryExpr* unary) {
	/* Continue visiting */
	unary->exp()->accept(this);

	/* Arithmetic expressions are allowed only between numerical types. */
	if (unary->exp()->type() == Parser::STRING) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tArithmetic expressions are allowed only between numerical types." << endl;
		exit(-1);
	}

	/* We can only take the address of left-hand operands. */
	if (unary->opr() == UnaryExpr::ADDR && !dynamic_cast<Parser::IdentifierExpr*>(unary->exp())) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tIt is only allowed to take the address of variables." << endl;
		exit(-1);
	}

	unary->type(unary->exp()->type());
}

void AstSemaVisitor::visit(Parser::BinaryExpr* binop) {
	/* Continue visiting. */
	binop->getExp1()->accept(this);
	binop->getExp2()->accept(this);

	Expression* exp1 = binop->getExp1();
	Expression* exp2 = binop->getExp2();

	/* Arithmetic expressions are allowed only between numerical types. */
	if (exp1->type() == Parser::STRING || exp2->type() == Parser::STRING) {
		auto decl  = exp1->addr();

		if (binop->opr() != BinaryExpr::ASSIGN || exp2->type() != Parser::STRING || decl == NULL || decl->type() != Parser::STRING) {
			cout << "Error in semantic analysis." << endl;
			cout << "\tArithmetic expressions are allowed only between numerical types." << endl;
			exit(-1);
		}

		binop->type(Parser::STRING);
	}
	else if (exp1->type() == exp2->type()) {
		binop->type(exp1->type());
	}
	else if ((CHECK_TYPE(exp1, INT) && CHECK_TYPE(exp2, FLOAT)) || (CHECK_TYPE(exp1, FLOAT) && CHECK_TYPE(exp2, INT))) {
		binop->type(Parser::FLOAT);
	}
}

void AstSemaVisitor::addNativeFunctions(shared_ptr<Parser::SymbolTable> table) {
	/* Global functions for outputing data. */
	shared_ptr<STFunctionDeclaration> printInt(new STFunctionDeclaration("printInt", "_printInt", Parser::VOID));
	shared_ptr<STFunctionDeclaration> printFlt(new STFunctionDeclaration("printFlt", "_printFlt", Parser::VOID));
	shared_ptr<STFunctionDeclaration> printStr(new STFunctionDeclaration("printStr", "_printStr", Parser::VOID));

	/* We need to add the function_formal parameters. */
	shared_ptr<STParamDecl> printIntParam = shared_ptr<STParamDecl>( new STParamDecl("_printInt_1", Parser::INT, typeWidth(Parser::INT), this->_currentOffset+=typeWidth(Parser::INT)));
	shared_ptr<STParamDecl> printFltParam = shared_ptr<STParamDecl>( new STParamDecl("_printFlt_1", Parser::FLOAT, typeWidth(Parser::FLOAT), this->_currentOffset+=typeWidth(Parser::FLOAT)));
	shared_ptr<STParamDecl> printStrParam = shared_ptr<STParamDecl>( new STParamDecl("_printStr_1", Parser::STRING, typeWidth(Parser::STRING), this->_currentOffset+=typeWidth(Parser::STRING)));

	table->add(printIntParam);
	printInt->addParam(printIntParam);

	table->add(printFltParam);
	printFlt->addParam(printFltParam);

	table->add(printStrParam);
	printStr->addParam(printStrParam);

	table->add(printInt);
	table->add(printFlt);
	table->add(printStr);



	/* Global functions for reading data. */
	shared_ptr<STFunctionDeclaration> readInt(new STFunctionDeclaration("readInt", "_readInt", Parser::INT));
	shared_ptr<STFunctionDeclaration> readFlt(new STFunctionDeclaration("readFlt", "_readFlt", Parser::FLOAT));
	shared_ptr<STFunctionDeclaration> readStr(new STFunctionDeclaration("readStr", "_readStr", Parser::STRING));

	table->add(readInt);
	table->add(readFlt);
	table->add(readStr);



	/* Global functions for copying strings. */
	shared_ptr<STFunctionDeclaration> fstrcpy(new STFunctionDeclaration(System::NAT_FUN_STRCPY, "_" + System::NAT_FUN_STRCPY, Parser::INT));
	shared_ptr<STParamDecl> fstrcpyPar1 = shared_ptr<STParamDecl>( new STParamDecl("_" + System::NAT_FUN_STRCPY + "_1", Parser::STRING, typeWidth(Parser::STRING), this->_currentOffset+=typeWidth(Parser::STRING)));
	shared_ptr<STParamDecl> fstrcpyPar2 = shared_ptr<STParamDecl>( new STParamDecl("_" + System::NAT_FUN_STRCPY + "_2", Parser::STRING, typeWidth(Parser::STRING), this->_currentOffset+=typeWidth(Parser::STRING)));

	table->add(fstrcpyPar1);
	table->add(fstrcpyPar2);

	fstrcpy->addParam(fstrcpyPar1);
	fstrcpy->addParam(fstrcpyPar2);

	table->add(fstrcpy);
}

bool AstSemaVisitor::isValidType(string name) {
	if (name == "int") return true;
	else if (name == "float") return true;
	else if (name == "string") return true;
	else if (name == "void") return true;
	else return false;
}

string Util::typeName(Parser::NativeType type) {
	if (type == Parser::INT) return "int";
	else if (type == Parser::FLOAT) return "float";
	else if (type == Parser::STRING) return "string";
	else if (type == Parser::VOID) return "void";
	else throw -1;
}

NativeType AstSemaVisitor::translateType(string name) {
	if (name == "int") return Parser::INT;
	else if (name == "float") return Parser::FLOAT;
	else if (name == "string") return Parser::STRING;
	else if (name == "void") return Parser::VOID;
	else return Parser::NOT_A_TYPE;
}

TypeWidth AstSemaVisitor::typeWidth(NativeType name) {
	if (name == Parser::INT) return Parser::INT_WIDTH;
	else if (name == Parser::FLOAT) return Parser::FLOAT_WIDTH;
	else if (name == Parser::STRING) return Parser::STRING_WIDTH;
	else if (name == Parser::VOID) return Parser::VOID_WIDTH;
	else throw -1;
}

int AstSemaVisitor::variableSize(int typeSize, list<shared_ptr<Expression>>* dims) {
	int units = 1;

	list<shared_ptr<Expression>>::iterator it;
	for (it = dims->begin(); it != dims->end(); it++) {
		IntegerExpr *expr = dynamic_cast<IntegerExpr*>((*it).get());

		if (expr == nullptr) return 0;

		units *= expr->value();
	}

	return typeSize * units;
}

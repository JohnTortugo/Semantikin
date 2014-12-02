#include "AstVisitors.h"
#include "SymbolTable.h"
#include <memory>
#include <list>

/* 1. adicionar declarações.										// OK
 * 2. verificar uso de simbolos inexistentes.						// ok
 * 3. checagem de tipos.
 * 4. checagem de semantica:
 * 		4.1 indexacao com ponto flutuante.
 * 		4.2 usando variável como chamada de função.					// ok
 * 		4.3 usando função como variável.							// ok
 * 		4.4 tipo de argumentos
 * 		4.5 quantidade correta de argumentos.						// ok
 */

using namespace Parser;

void AstSemaVisitor::visit(Parser::CompilationUnit* module) {
	cout << "Going to carry semantic analyze brow." << endl;

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

	/* Insert the function definition in the symbol table. */
	vector<pair<NativeType, int>> params;
	list<shared_ptr<ParamDecl>>::iterator it;
	for (it = function->getParams()->begin(); it != function->getParams()->end(); it++) {
		NativeType ptype = translateType((*it)->getType());
		int pdims = (*it)->getDims()->size();

		params.push_back(make_pair(ptype, pdims));
	}

	shared_ptr<Parser::STFunctionDeclaration> funDecl(new Parser::STFunctionDeclaration(name, name, translateType(type), params));
	this->currentSymbTable->add(funDecl);

	/* Offset is always relative to the function frame,
	 * so we reset when we enter a function. */
	this->currentOffset = 0;

	/* Create new symbol table for this scope. */
	shared_ptr<SymbolTable> table(new SymbolTable(currentSymbTable));

	/* Set the symbol table in the function. */
	function->setSymbTable(table);

	/* Update pointer to new current symbol table. */
	this->currentSymbTable = table;

	/* Add the node of parameters if actually there is parameters. */
	list<shared_ptr<ParamDecl>>::iterator itt = function->getParams()->begin();
	for (itt = function->getParams()->begin(); itt != function->getParams()->end(); itt++)
		(*itt)->accept(this);

	/* Continue the visiting. */
	function->getBody()->accept(this);

	/* Reset pointer to previous symbol table. */
	this->currentSymbTable = table->getParent();
}

void AstSemaVisitor::visit(const Parser::ParamDecl* param) {
	list<shared_ptr<Parser::Expression>>* dims = param->getDims();
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
	int varSize = variableSize(typeWidth(type), param->getDims());

	if (varSize == 0) {
		cout << "The dimension of a array should be constant." << endl;
		exit(-1);
	}

	shared_ptr<Parser::STParamDecl> parDecl(new Parser::STParamDecl(name, translateType(type), varSize, this->currentOffset, param->getDims()->size()));
	this->currentSymbTable->add(parDecl);

	this->currentOffset += varSize;
}

void AstSemaVisitor::visit(const Parser::VarSpec* spec) {
}

void AstSemaVisitor::visit(const Parser::VarDecl* varDec) {
	string type 									= varDec->getType();
	list<shared_ptr<Parser::VarSpec>>* specs 		= varDec->getVars();
	list<shared_ptr<Parser::VarSpec>>::iterator it 	= specs->begin();

	while (it != specs->end()) {
		string name = (*it)->getName();
		shared_ptr<SymbolTableEntry> prev 	= this->currentSymbTable->lookup(name);

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

		int dims = 0, varSize = typeWidth(type);

		if ((*it)->getDimsExpr() != nullptr) {
			dims = (*it)->getDimsExpr()->size();
			varSize = variableSize(typeWidth(type), (*it)->getDimsExpr());
		}

		shared_ptr<Parser::STLocalVarDecl> param(new Parser::STLocalVarDecl(name, translateType(type), varSize, this->currentOffset, dims));
		this->currentSymbTable->add(param);

		this->currentOffset += varSize;

		it++;
	}
}

void AstSemaVisitor::visit(const Parser::LoopStmt* loop) {
	/* Continue visiting. */
	loop->getCondition()->accept(this);

	if (loop->getBody() != nullptr)
		loop->getBody()->accept(this);
}

void AstSemaVisitor::visit(const Parser::IfStmt* ifStmt) {
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

void AstSemaVisitor::visit(const Parser::ElseIfStmt* elseIfStmt) {
	/* Continue visiting. */
	elseIfStmt->getCondition()->accept(this);

	if (elseIfStmt->getElseIfBlock() != nullptr)
		elseIfStmt->getElseIfBlock()->accept(this);
}

void AstSemaVisitor::visit(const Parser::ReturnStmt* ret) {
	/* Continue visiting */
	ret->getExpression()->accept(this);
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

	table->dump();
	cout << endl << endl << endl;
}

void AstSemaVisitor::visit(const Parser::StringExpr* str) {

}

void AstSemaVisitor::visit(const Parser::FloatExpr* flt) {

}

void AstSemaVisitor::visit(const Parser::IntegerExpr* integer) {

}

void AstSemaVisitor::visit(const Parser::IdentifierExpr* id) {
	string name = id->getValue();
	shared_ptr<SymbolTableEntry> decl = this->currentSymbTable->lookup(name);
	list<shared_ptr<Parser::Expression>>* dims = id->getDimsExprs();

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

	/* If the symbol declaration tells it is a matrix then we need the right number of dimensions. */
	STVariableDeclaration* varDecl = dynamic_cast<STVariableDeclaration*>(decl.get());
	if ((varDecl->getNumDims() > 0 && dims == nullptr) || (varDecl->getNumDims() != dims->size())) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tMismatch in the number of dimensions used to access the symbol \"" << name << "\"." << endl;
		exit(-1);
	}

	/* Check if the expressions used to index the array are all constants. */
//	if (dims != nullptr) {
//		list<shared_ptr<Parser::Expression>>::iterator it = dims->begin();
//		IntegerExpr* integer;
//		IdentifierExpr* ident;
//		FunctionCall* func;
//
//		while (it != dims->end()) {
//			if (integer = dynamic_cast<IntegerExpr*>(it->get())) {
//				/* OK. */
//			}
//			else if (ident = dynamic_cast<IdentifierExpr*>(it->get())) {
//				STVariableDeclaration* decl = dynamic_cast<STVariableDeclaration*>(this->currentSymbTable->lookup(ident->getValue()).get());
//
//				if (decl->getType() != Parser::INT) {
//					cout << "Error in semantic analysis." << endl;
//					cout << "\tArray variables (e.g., \"" << name << "\") must be indexed using integer values." << endl;
//					exit(-1);
//				}
//			}
//			else if (func = dynamic_cast<FunctionCall*>(it->get())) {
//				STFunctionDeclaration* decl = dynamic_cast<STFunctionDeclaration*>(this->currentSymbTable->lookup(ident->getValue()).get());
//
//				if (decl->getReturnType() != Parser::INT) {
//					cout << "Error in semantic analysis." << endl;
//					cout << "\tArray variables (e.g., \"" << name << "\") must be indexed using integer values." << endl;
//					exit(-1);
//				}
//			}
//			else {
//				cout << "Error in semantic analysis." << endl;
//				cout << "\tArray variables (e.g., \"" << name << "\") must be indexed using integer values." << endl;
//				exit(-1);
//			}
//
//			it++;
//		}
//	}

	/* Continue visiting. */
	if (dims != nullptr) {
		list<shared_ptr<Parser::Expression>>::iterator it = dims->begin();
		while (it != dims->end()) {
			(*it)->accept(this);
			it++;
		}
	}
}

void AstSemaVisitor::visit(const Parser::FunctionCall* funCall) {
	string name = funCall->getName();
	shared_ptr<SymbolTableEntry> decl 					= this->currentSymbTable->lookup(name);
	list<shared_ptr<Parser::Expression>>* arguments 	= funCall->getArguments();
	list<shared_ptr<Parser::Expression>>::iterator it 	= arguments->begin();

	/* If the symbol was not declared is a error. */
	if (decl == nullptr) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tThere is not a definition of the symbol \"" << name << "\"." << endl;
		exit(-1);
	}

	STFunctionDeclaration *funDecl = dynamic_cast<STFunctionDeclaration*>(decl.get());
	if (funDecl == nullptr) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tThere is an attempt to use the symbol \"" << name << "\" as a function but it's declared type is variable." << endl;
		exit(-1);
	}

	if (funDecl->getParams()->size() != funCall->getArguments()->size()) {
		cout << "Error in semantic analysis." << endl;
		cout << "\tIncorrect number of parameters were passed to function \"" << name << "\"." << endl;
		cout << "\t" << funDecl->getParams()->size() << " were expected, but " << arguments->size() << " was passed." << endl;
		exit(-1);
	}

	/* Need to check the type of the parameters. */

	/* Continue visiting. */
	if (arguments != nullptr) {
		list<shared_ptr<Parser::Expression>>::iterator it = arguments->begin();
		while (it != arguments->end()) {
			(*it)->accept(this);
			it++;
		}
	}
}

void AstSemaVisitor::visit(const Parser::UnaryExpr* unary) {
	/* Continue visiting */
	unary->getExp()->accept(this);
}

void AstSemaVisitor::visit(const Parser::BinaryExpr* binop) {
	/* Continue visiting. */
	binop->getExp1()->accept(this);
	binop->getExp2()->accept(this);
}

void AstSemaVisitor::addNativeFunctions(shared_ptr<Parser::SymbolTable> table) {
	/* Global functions for outputing data. */
	shared_ptr<STFunctionDeclaration> printInt(new STFunctionDeclaration("printInt", "_printInt", Parser::VOID, vector<pair<NativeType, int>>(1, make_pair(Parser::INT, 0))));
	shared_ptr<STFunctionDeclaration> printFlt(new STFunctionDeclaration("printFlt", "_printFlt", Parser::VOID, vector<pair<NativeType, int>>(1, make_pair(Parser::FLOAT, 0))));
	shared_ptr<STFunctionDeclaration> printDbl(new STFunctionDeclaration("printDbl", "_printDbl", Parser::VOID, vector<pair<NativeType, int>>(1, make_pair(Parser::DOUBLE, 0))));
	shared_ptr<STFunctionDeclaration> printStr(new STFunctionDeclaration("printStr", "_printStr", Parser::VOID, vector<pair<NativeType, int>>(1, make_pair(Parser::STRING, 0))));

	table->add(printInt);
	table->add(printFlt);
	table->add(printDbl);
	table->add(printStr);


	/* Global functions for reading data. */
	shared_ptr<STFunctionDeclaration> readInt(new STFunctionDeclaration("readInt", "_readInt", Parser::INT, vector<pair<NativeType, int>>()));
	shared_ptr<STFunctionDeclaration> readFlt(new STFunctionDeclaration("readFlt", "_readFlt", Parser::FLOAT, vector<pair<NativeType, int>>()));
	shared_ptr<STFunctionDeclaration> readDbl(new STFunctionDeclaration("readDbl", "_readDbl", Parser::DOUBLE, vector<pair<NativeType, int>>()));
	shared_ptr<STFunctionDeclaration> readStr(new STFunctionDeclaration("readStr", "_readStr", Parser::STRING, vector<pair<NativeType, int>>()));

	table->add(readInt);
	table->add(readFlt);
	table->add(readDbl);
	table->add(readStr);
}

bool AstSemaVisitor::isValidType(string name) {
	if (name == "int") return true;
	else if (name == "float") return true;
	else if (name == "double") return true;
	else if (name == "string") return true;
	else if (name == "void") return true;
	else return false;
}

NativeType AstSemaVisitor::translateType(string name) {
	if (name == "int") return Parser::INT;
	else if (name == "float") return Parser::FLOAT;
	else if (name == "double") return Parser::DOUBLE;
	else if (name == "string") return Parser::STRING;
	else if (name == "void") return Parser::VOID;
	else throw -1;
}

TypeWidth AstSemaVisitor::typeWidth(string name) {
	if (name == "int") return Parser::INT_WIDTH;
	else if (name == "float") return Parser::FLOAT_WIDTH;
	else if (name == "double") return Parser::DOUBLE_WIDTH;
	else if (name == "string") return Parser::STRING_WIDTH;
	else if (name == "void") return Parser::VOID_WIDTH;
	else throw -1;
}

int AstSemaVisitor::variableSize(int typeSize, list<shared_ptr<Expression>>* dims) {
	int units = 1;

	list<shared_ptr<Expression>>::iterator it;
	for (it = dims->begin(); it != dims->end(); it++) {
		IntegerExpr *expr = dynamic_cast<IntegerExpr*>((*it).get());

		if (expr == nullptr) return 0;

		units *= expr->getValue();
	}

	return typeSize * units;
}

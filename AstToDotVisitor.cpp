#include "AstVisitors.h"
#include <memory>
#include <list>

void AstToDotVisitor::visit(Parser::CompilationUnit* module) {
	list<shared_ptr<Parser::Function>>::iterator it = module->getFunctions()->begin();
	while (it != module->getFunctions()->end()) {
		(*it)->accept(this);
		it++;
	}
}

void AstToDotVisitor::visit(Parser::Function* function) {
	/* The function header node. */
	this->dotfile << "\t\"" << function << "\" [shape=record, label=\"{FuncDecl|Name:" << function->getName() << "|Return:" << function->getReturnType() << "}\"];" << endl;

	/* Add the node of parameters if actually there is parameters. */
	list<shared_ptr<Parser::ParamDecl>>* params = function->getParams();
	if (params != nullptr && params->size() > 0) {
		list<shared_ptr<Parser::ParamDecl>>::iterator it = params->begin();

		this->dotfile << "\t\"" << function << "\" -> \"" << function << "_params\";" << endl;
		this->dotfile << "\t\"" << function << "_params\" [shape=trapezium, label=\"Parameters\"];" << endl;

		for (it = params->begin(); it != params->end(); it++) {
			this->dotfile << "\t\"" << function << "_params\" -> \"" << *it << "\";" << endl;
			(*it)->accept(this);
		}
	}

	/* Edge to Body. */
	this->dotfile << "\t\"" << function << "\" -> \"" << function->getBody() << "\";" << endl;

	/* Continue the visiting. */
	function->getBody()->accept(this);
}

void AstToDotVisitor::visit(const Parser::ParamDecl* param) {
	list<shared_ptr<Parser::Expression>>* dims = param->getDims();

	/* The parameter header node. */
	if (dims->size() > 0) {
		this->dotfile << "\t\"" << param << "\" [shape=record, label=\"{Type:" << param->getType() << "|Name:" << param->getName() << "|Dims:" << dims->size() << "}\"];" << endl;

		/* Create the hub node. */
		this->dotfile << "\t\"" << param << "-dims\" [shape=tripleoctagon, label=\"Sizes\"];" << endl;

		/* Edge to the dimension hub. */
		this->dotfile << "\t\"" << param << "\" -> \"" << param << "-dims\";" << endl;

		/* Edge from the hub to every dimension expr. */
		list<shared_ptr<Parser::Expression>>::iterator it = dims->begin();
		while (it != dims->end()) {
			this->dotfile << "\t\"" << param << "-dims\" -> \"" << it->get() << "\";" << endl;
			(*it)->accept(this);
			it++;
		}
	}
	else {
		this->dotfile << "\t\"" << param << "\" [shape=record, label=\"{Type:" << param->getType() << "|Name:" << param->getName() << "|Scalar}\"];" << endl;
	}
}

void AstToDotVisitor::visit(const Parser::VarSpec* spec) {
	Parser::Expression* initializer = spec->getInitializer();
	list<shared_ptr<Parser::Expression>>* dims = spec->getDimsExpr();

	if (dims != nullptr && dims->size() > 0) {
		this->dotfile << "\t\"" << spec << "\" 		[shape=record, label=\"{Vector|Name:" << spec->getName() << "}\"];" << endl;
		this->dotfile << "\t\"" << spec << "-dims\" [shape=tripleoctagon, label=\"Sizes\"];" << endl;

		/* Edge to the dimension hub. */
		this->dotfile << "\t\"" << spec << "\" -> \"" << spec << "-dims\";" << endl;

		list<shared_ptr<Parser::Expression>>::iterator it 	= dims->begin();

		while (it != dims->end()) {
			this->dotfile << "\t\"" << spec << "-dims\" -> \"" << it->get() << "\";" << endl;
			it++;
		}
	}
	else {
		this->dotfile << "\t\"" << spec << "\" [shape=record, label=\"{Scalar|Name:" << spec->getName() << "}\"];" << endl;

		if (initializer != nullptr)
			this->dotfile << "\t\"" << spec << "\" -> \"" << initializer << "\";" << endl;
	}

	/* Continue the visiting. */
	if (dims != nullptr) {
		list<shared_ptr<Parser::Expression>>::iterator it = dims->begin();
		while (it != dims->end()) {
			(*it)->accept(this);
			it++;
		}
	}

	if (initializer != nullptr) {
		initializer->accept(this);
	}
}

void AstToDotVisitor::visit(const Parser::VarDecl* varDec) {
	/* The block header header node. */
	this->dotfile << "\t\"" << varDec << "\" [shape=record, label=\"{VarDecl|Type:" << varDec->getType() << "}\"];" << endl;

	list<shared_ptr<Parser::VarSpec>>* specs = varDec->getVars();
	list<shared_ptr<Parser::VarSpec>>::iterator it = specs->begin();

	while (it != specs->end()) {
		this->dotfile << "\t\"" << varDec << "\" -> \"" << it->get() << "\";" << endl;
		it++;
	}

	/* Continue the visiting. */
	it = specs->begin();
	while (it != specs->end()) {
		(*it)->accept(this);
		it++;
	}
}

void AstToDotVisitor::visit(Parser::LoopStmt* loop) {
	/* The loop header node. */
	this->dotfile << "\t\"" << loop << "\" [shape=egg, label=\"Loop\"];" << endl;

	/* Edge to Condition. */
	this->dotfile << "\t\"" << loop << "\" -> \"" << loop->getCondition() << "\";" << endl;

	/* Edge to Body. */
	if (loop->getBody() != nullptr)
		this->dotfile << "\t\"" << loop << "\" -> \"" << loop->getBody() << "\";" << endl;

	/* Continue visiting. */
	loop->getCondition()->accept(this);

	if (loop->getBody() != nullptr)
		loop->getBody()->accept(this);
}

void AstToDotVisitor::visit(Parser::IfStmt* ifStmt) {
	/* The block header header node. */
	this->dotfile << "\t\"" << ifStmt << "\" [shape=house, label=\"IfStmt\"];" << endl;

	Parser::Expression* condition = ifStmt->getCondition();
	Parser::CodeBlock* thenBlock = ifStmt->getThenBlock();
	list<shared_ptr<Parser::ElseIfStmt>>* elseIfChain = ifStmt->getElseIfChain();
	Parser::CodeBlock* elseBlock = ifStmt->getElseBlock();

	/* Add the "Then-Block" */
	this->dotfile << "\t\"" << ifStmt << "\" -> \"" << ifStmt << "-then\";" << endl;
	this->dotfile << "\t\"" << ifStmt << "-then\" [shape=house, label=\"Then\"];" << endl;

	/* Edges from "Then-Block" to condition and the block itself. */
	this->dotfile << "\t\"" << ifStmt << "-then\" -> \"" << condition << "\";" << endl;
	if (thenBlock != nullptr)
		this->dotfile << "\t\"" << ifStmt << "-then\" -> \"" << thenBlock << "\";" << endl;

	/* Add the nodes/edges for else-if */
	if (elseIfChain != nullptr) {
		list<shared_ptr<Parser::ElseIfStmt>>::iterator it = elseIfChain->begin();
		while (it != elseIfChain->end()) {
			this->dotfile << "\t\"" << ifStmt << "\" -> \"" << it->get() << "\";" << endl;
			it++;
		}
	}

	/* Add the else branch. */
	if (elseBlock != nullptr) {
		this->dotfile << "\t\"" << ifStmt << "\" -> \"" << ifStmt << "-else\";" << endl;
		this->dotfile << "\t\"" << ifStmt << "-else\" [shape=house, label=\"Else\"];" << endl;
		this->dotfile << "\t\"" << ifStmt << "-else\" -> \"" << elseBlock << "\";" << endl;
	}

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

void AstToDotVisitor::visit(Parser::ElseIfStmt* elseIfStmt) {
	/* The block header header node. */
	this->dotfile << "\t\"" << elseIfStmt << "\" [shape=house, label=\"ElseIf\"];" << endl;

	this->dotfile << "\t\"" << elseIfStmt << "\" -> \"" << elseIfStmt->getCondition() << "\";" << endl;
	if (elseIfStmt->getElseIfBlock()  != nullptr)
		this->dotfile << "\t\"" << elseIfStmt << "\" -> \"" << elseIfStmt->getElseIfBlock() << "\";" << endl;

	/* Continue visiting. */
	elseIfStmt->getCondition()->accept(this);

	if (elseIfStmt->getElseIfBlock() != nullptr)
		elseIfStmt->getElseIfBlock()->accept(this);
}

void AstToDotVisitor::visit(const Parser::ReturnStmt* ret) {
	/* The main node information. */
	this->dotfile << "\t\"" << ret << "\" [shape=diamond, label=\"Return\"];" << endl;

	this->dotfile << "\t\"" << ret << "\" -> \"" << ret->getExpression() << "\";" << endl;

	/* Continue visiting */
	ret->getExpression()->accept(this);
}

void AstToDotVisitor::visit(Parser::CodeBlock* block) {
	/* The block header header node. */
	this->dotfile << "\t\"" << block << "\" [shape=trapezium, label=\"CodeBlock\"];" << endl;

	list<shared_ptr<Parser::Statement>>* statements = block->getStatements();

	if (statements != nullptr) {
		list<shared_ptr<Parser::Statement>>::iterator it = statements->begin();
		while (it != statements->end()) {
			this->dotfile << "\t\"" << block << "\" -> \"" << it->get() << "\";" << endl;
			it++;
		}
	}

	/* Continue visiting */
	if (statements != nullptr) {
		list<shared_ptr<Parser::Statement>>::iterator it = statements->begin();
		while (it != statements->end()) {
			(*it)->accept(this);
			it++;
		}
	}
}

void AstToDotVisitor::visit(Parser::StringExpr* str) {
	/* The main node information. */
	string value = escapeStr(str->value());
	value = (value.length() > 10) ? value.substr(0, 10)  + "...\\\"" : value;

	this->dotfile << "\t\"" << str << "\" [shape=rect, label=\"" << value<< "\"];" << endl;

}

void AstToDotVisitor::visit(Parser::FloatExpr* flt) {
	/* The main node information. */
	this->dotfile << "\t\"" << flt << "\" [shape=rect, label=\"" << flt->value() << "\"];" << endl;
}

void AstToDotVisitor::visit(Parser::IntegerExpr* integer) {
	/* The main node information. */
	this->dotfile << "\t\"" << integer << "\" [shape=rect, label=\"" << integer->value() << "\"];" << endl;
}

void AstToDotVisitor::visit(Parser::IdentifierExpr* id) {
	/* The main node information. */
	this->dotfile << "\t\"" << id << "\" [shape=octagon, label=\"" << id->value() << "\"];" << endl;

	list<shared_ptr<Parser::Expression>>* dims = id->dimsExprs();

	if (dims != nullptr && dims->size() > 0) {
		/* Create the hub node. */
		this->dotfile << "\t\"" << id << "-dims\" [shape=tripleoctagon, label=\"Dims\"];" << endl;

		/* Edge to the dimension hub. */
		this->dotfile << "\t\"" << id << "\" -> \"" << id << "-dims\";" << endl;

		/* Edge from the hub to every dimension expr. */
		list<shared_ptr<Parser::Expression>>::iterator it = dims->begin();
		while (it != dims->end()) {
			this->dotfile << "\t\"" << id << "-dims\" -> \"" << it->get() << "\";" << endl;
			it++;
		}
	}

	/* Continue visiting. */
	if (dims != nullptr) {
		list<shared_ptr<Parser::Expression>>::iterator it = dims->begin();
		while (it != dims->end()) {
			(*it)->accept(this);
			it++;
		}
	}

}

void AstToDotVisitor::visit(Parser::FunctionCall* funCall) {
	/* The main node information. */
	this->dotfile << "\t\"" << funCall << "\" [shape=record, label=\"{FunCall|Name:" << funCall->name() << "|Arguments}\"];" << endl;

	list<shared_ptr<Parser::Expression>>* arguments = funCall->arguments();
	list<shared_ptr<Parser::Expression>>::iterator it = arguments->begin();
	while (it != arguments->end()) {
		this->dotfile << "\t\"" << funCall << "\" -> \"" << it->get() << "\";" << endl;
		it++;
	}

	/* Continue visiting. */
	if (arguments != nullptr) {
		list<shared_ptr<Parser::Expression>>::iterator it = arguments->begin();
		while (it != arguments->end()) {
			(*it)->accept(this);
			it++;
		}
	}
}

void AstToDotVisitor::visit(Parser::UnaryExpr* unary) {
	/* The main node information. */
	this->dotfile << "\t\"" << unary << "\" [shape=triangle, label=\"" << translateUnop(unary->opr()) << "\"];" << endl;

	/* Edge to the first expression. */
	this->dotfile << "\t\"" << unary << "\" -> \"" << unary->exp() << "\";" << endl;

	/* Continue visiting */
	unary->exp()->accept(this);
}

void AstToDotVisitor::visit(Parser::BinaryExpr* binop) {
	/* The main node information. */
	this->dotfile << "\t\"" << binop << "\" [shape=triangle, label=\"" << translateBinop(binop->opr()) << "\"];" << endl;

	/* Edge to the first expression. */
	this->dotfile << "\t\"" << binop << "\" -> \"" << binop->getExp1() << "\";" << endl;

	/* Edge to the second expression. */
	this->dotfile << "\t\"" << binop << "\" -> \"" << binop->getExp2() << "\";" << endl;

	/* Continue visiting. */
	binop->getExp1()->accept(this);
	binop->getExp2()->accept(this);
}

string AstToDotVisitor::translateBinop(Parser::BinaryExpr::ExprType type) {
	switch (type) {
		case Parser::BinaryExpr::COMPARE: return "==";
		case Parser::BinaryExpr::DIFFERENCE: return "!=";
		case Parser::BinaryExpr::ASSIGN: return "=";
		case Parser::BinaryExpr::LOG_AND: return "&&";
		case Parser::BinaryExpr::LOG_OR: return "||";
		case Parser::BinaryExpr::BIT_AND: return "&";
		case Parser::BinaryExpr::BIT_OR: return "|";
		case Parser::BinaryExpr::BIT_XOR: return "^";
		case Parser::BinaryExpr::LT: return "<";
		case Parser::BinaryExpr::LTE: return "<=";
		case Parser::BinaryExpr::GT: return ">";
		case Parser::BinaryExpr::GTE: return ">=";
		case Parser::BinaryExpr::ADDITION: return "+";
		case Parser::BinaryExpr::SUBTRACTION: return "-";
		case Parser::BinaryExpr::TIMES: return "*";
		case Parser::BinaryExpr::DIV: return "/";
		case Parser::BinaryExpr::MOD: return "%";
		case Parser::BinaryExpr::PLUS_EQUAL: return "+=";
		case Parser::BinaryExpr::MINUS_EQUAL: return "-=";
		case Parser::BinaryExpr::TIMES_EQUAL: return "*=";
		case Parser::BinaryExpr::DIV_EQUAL: return "/=";
		case Parser::BinaryExpr::MOD_EQUAL: return "%=";
	}
}

string AstToDotVisitor::translateUnop(Parser::UnaryExpr::ExprType type) {
	switch (type) {
		case Parser::UnaryExpr::BIT_NOT: return "~";
		case Parser::UnaryExpr::NOT: return "!";
		case Parser::UnaryExpr::MINUS: return "-";
		case Parser::UnaryExpr::PLUS: return "+";
		case Parser::UnaryExpr::ADDR: return "&";
		case Parser::UnaryExpr::INCREMENT: return "++";
		case Parser::UnaryExpr::DECREMENT: return "--";
	}
}

string AstToDotVisitor::escapeStr(string str) {
	string ret;
	string::iterator it = str.begin();

	while (it != str.end()) {
		if (*it == '"') ret += "\\\"";
		else if (*it == '\\') ret += "\\\\";
		else ret += *it;

		it++;
	}

	return ret;
}


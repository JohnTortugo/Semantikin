#ifndef ASTVISITORS_H_
#define ASTVISITORS_H_

#include <iostream>
#include "AbstractSyntaxTree.h"

class AstNodeVisitor {
public:
	virtual void visit(Parser::CompilationUnit* module) = 0;
	virtual void visit(Parser::Function* module) = 0;
	virtual void visit(const Parser::ParamDecl* module) = 0;
	virtual void visit(const Parser::VarSpec* module) = 0;
	virtual void visit(const Parser::VarDecl* module) = 0;
	virtual void visit(const Parser::LoopStmt* module) = 0;
	virtual void visit(const Parser::IfStmt* module) = 0;
	virtual void visit(const Parser::ElseIfStmt* module) = 0;
	virtual void visit(const Parser::ReturnStmt* module) = 0;
	virtual void visit(Parser::CodeBlock* module) = 0;
	virtual void visit(Parser::StringExpr* module) = 0;
	virtual void visit(Parser::FloatExpr* module) = 0;
	virtual void visit(Parser::IntegerExpr* module) = 0;
	virtual void visit(Parser::IdentifierExpr* module) = 0;
	virtual void visit(Parser::FunctionCall* module) = 0;
	virtual void visit(Parser::UnaryExpr* module) = 0;
	virtual void visit(Parser::BinaryExpr* module) = 0;

	virtual ~AstNodeVisitor() {};
};

class AstToDotVisitor : public AstNodeVisitor {
private:
	std::ofstream dotfile;

public:
	AstToDotVisitor(string filename) {
		this->dotfile.open(filename);

		this->dotfile << "digraph AstGraph {" << endl;
		this->dotfile << "\ttitle = \"This is the AST of Astzin\";" << endl;
	}

	~AstToDotVisitor() {
		this->dotfile << "}" << endl;

		this->dotfile.close();
	}

	string translateBinop(Parser::BinaryExpr::ExprType type);

	string translateUnop(Parser::UnaryExpr::ExprType type);

	string escapeStr(string str);

	void visit(Parser::CompilationUnit* module);
	void visit(Parser::Function* module);
	void visit(const Parser::ParamDecl* module);
	void visit(const Parser::VarSpec* module);
	void visit(const Parser::VarDecl* module);
	void visit(const Parser::LoopStmt* module);
	void visit(const Parser::IfStmt* module);
	void visit(const Parser::ElseIfStmt* module);
	void visit(const Parser::ReturnStmt* module);
	void visit(Parser::CodeBlock* module);
	void visit(Parser::StringExpr* module);
	void visit(Parser::FloatExpr* module);
	void visit(Parser::IntegerExpr* module);
	void visit(Parser::IdentifierExpr* module);
	void visit(Parser::FunctionCall* node);
	void visit(Parser::UnaryExpr* module);
	void visit(Parser::BinaryExpr* module);
};

class AstSemaVisitor : public AstNodeVisitor {
private:
	/* Offset of the next variable inside the frame of this function. */
	int currentOffset;

	/* Return type of the current function. */
	Parser::NativeType currentFunRetType;

	/* Number of return statements in the current function. */
	unsigned int currentFunReturns;

	std::shared_ptr<Parser::SymbolTable> currentSymbTable;

public:
	AstSemaVisitor() : currentOffset(0), currentFunRetType(Parser::VOID), currentFunReturns(0) { }

	void visit(Parser::CompilationUnit* module);
	void visit(Parser::Function* module);
	void visit(const Parser::ParamDecl* module);
	void visit(const Parser::VarSpec* module);
	void visit(const Parser::VarDecl* module);
	void visit(const Parser::LoopStmt* module);
	void visit(const Parser::IfStmt* module);
	void visit(const Parser::ElseIfStmt* module);
	void visit(const Parser::ReturnStmt* module);
	void visit(Parser::CodeBlock* module);
	void visit(Parser::StringExpr* module);
	void visit(Parser::FloatExpr* module);
	void visit(Parser::IntegerExpr* module);
	void visit(Parser::IdentifierExpr* module);
	void visit(Parser::FunctionCall* node);
	void visit(Parser::UnaryExpr* module);
	void visit(Parser::BinaryExpr* module);

	void addNativeFunctions(shared_ptr<Parser::SymbolTable> table);
	bool isValidType(string name);
	Parser::NativeType translateType(string name);
	string typeName(Parser::NativeType type);
	Parser::TypeWidth typeWidth(string name);
	int variableSize(int typeSize, list<shared_ptr<Parser::Expression>>* param);
};

#endif /* ASTVISITORS_H_ */

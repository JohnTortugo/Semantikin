#ifndef ASTVISITORS_H_
#define ASTVISITORS_H_

#include <iostream>
#include "AbstractSyntaxTree.h"
#include "IR.h"

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
	int _currentOffset;

	/* Return type of the current function. */
	Parser::NativeType currentFunRetType;

	/* Number of return statements in the current function. */
	unsigned int currentFunReturns;

	/* This is the symbol table for the current scope. A pointer to
	 * the enclosing scope symbol table is keept. 				 */
	std::shared_ptr<Parser::SymbolTable> currentSymbTable;

public:
	AstSemaVisitor() : _currentOffset(0), currentFunRetType(Parser::VOID), currentFunReturns(0) { }

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

	static bool isValidType(string name);

	static Parser::NativeType translateType(string name);

	static string typeName(Parser::NativeType type);

	static Parser::TypeWidth typeWidth(Parser::NativeType name);

	static int variableSize(int typeSize, list<shared_ptr<Parser::Expression>>* param);

	int currentOffset() { return this->_currentOffset; }
};

class AstTACGenVisitor : public AstNodeVisitor {
private:
	/* This is a parameter imported from the Semantic Analyzer, it indicates
	 * the function frame index used to store new temporary variables.    */
	int _currentOffset;

	/* Pointer to the module currently being IR generated. */
	shared_ptr<IR::Module> _module;

	/* Pointer to the function currently being IR generated. */
	shared_ptr<IR::Function> _currentFunction;

	/* This indicates if the RelationalExpression being analyzed is a
	 * expression inside a conditional instruction or not. If it is, due to
	 * short-circuit behavior, we may need to insert jumps (if's) inside
	 * the expression IR's instruction. 								 */
	bool _isExpInConditional;

	/* Count the number of constants/labels/tmps found/added to the symbol table of
	 * a function. 													 			 */
	unsigned int constCounter = 1;
	unsigned int labelCounter = 1;
	unsigned int tempCounter = 1;

public:
	AstTACGenVisitor(int currOffset) : _module(nullptr), _currentOffset(currOffset), _isExpInConditional(false), constCounter(1), labelCounter(1), tempCounter(1) { }

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

	shared_ptr<IR::Module> module() const { return this->_module; }

	shared_ptr<STTempVar> newTemporary(NativeType type);
};

#endif /* ASTVISITORS_H_ */

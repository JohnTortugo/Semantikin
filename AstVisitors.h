#ifndef ASTVISITORS_H_
#define ASTVISITORS_H_

#include <iostream>
#include "AbstractSyntaxTree.h"
#include "IR.h"

class AstTreeVisitor {
public:
	virtual void visit(Parser::CompilationUnit* module) = 0;
	virtual void visit(Parser::Function* module) = 0;
	virtual void visit(const Parser::ParamDecl* module) = 0;
	virtual void visit(const Parser::VarSpec* module) = 0;
	virtual void visit(const Parser::VarDecl* module) = 0;
	virtual void visit(Parser::LoopStmt* module) = 0;
	virtual void visit(Parser::IfStmt* module) = 0;
	virtual void visit(Parser::ElseIfStmt* module) = 0;
	virtual void visit(const Parser::ReturnStmt* module) = 0;
	virtual void visit(Parser::CodeBlock* module) = 0;
	virtual void visit(Parser::StringExpr* module) = 0;
	virtual void visit(Parser::FloatExpr* module) = 0;
	virtual void visit(Parser::IntegerExpr* module) = 0;
	virtual void visit(Parser::IdentifierExpr* module) = 0;
	virtual void visit(Parser::FunctionCall* module) = 0;
	virtual void visit(Parser::UnaryExpr* module) = 0;
	virtual void visit(Parser::BinaryExpr* module) = 0;

	virtual ~AstTreeVisitor() {};
};

class AstToDotVisitor : public AstTreeVisitor {
private:
	std::ofstream dotfile;

public:
	AstToDotVisitor(string filename) {
		this->dotfile.open(filename);

		this->dotfile << "digraph AstGraph {" << endl;
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
	void visit(Parser::LoopStmt* module);
	void visit(Parser::IfStmt* module);
	void visit(Parser::ElseIfStmt* module);
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

class AstSemaVisitor : public AstTreeVisitor {
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

	/* Pointer to the STentry for declaration of current function. */
	shared_ptr<Parser::STFunctionDeclaration> _curFunDecl;

public:
	AstSemaVisitor() : _currentOffset(0), currentFunRetType(Parser::VOID), currentFunReturns(0) { }

	void visit(Parser::CompilationUnit* module);
	void visit(Parser::Function* module);
	void visit(const Parser::ParamDecl* module);
	void visit(const Parser::VarSpec* module);
	void visit(const Parser::VarDecl* module);
	void visit(Parser::LoopStmt* module);
	void visit(Parser::IfStmt* module);
	void visit(Parser::ElseIfStmt* module);
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

	static Parser::TypeWidth typeWidth(Parser::NativeType name);

	static int variableSize(int typeSize, list<shared_ptr<Parser::Expression>>* param);

	int currentOffset() { return this->_currentOffset; }
};

class AstTACGenVisitor : public AstTreeVisitor {
private:
	/* Pointer to the module currently being IR generated. */
	shared_ptr<IR::Module> _module;

	/* Pointer to the function currently being IR generated. */
	shared_ptr<IR::Function> _currentFunction;

	/* Count the number of constants/basicblocks/tmps found/added to the symbol table of
	 * a function. 													 			 */
	unsigned int constCounter = 1;
	unsigned int basicBlockCounter = 1;
	unsigned int tempCounter = 1;

	/* This is a parameter imported from the Semantic Analyzer, it indicates
	 * the function frame index used to store new temporary variables.    */
	int _currentOffset;

	/**
	 * This is a pointer for the last IR node (instruction) added to a subtree.
	 * Therefore, essentially this instruction is a root of a subtree. Since we
	 * construct the IR tree bottom up the "upper" nodes need to have a reference
	 * to the "bottom" nodes. This variable is a pointer to the last bottom node
	 * created.
	 */
	Instruction_sptr _lastInstruction = nullptr;

	/*
	 * This is the exit basic block of the function.
	 */
	BasicBlock_sptr _exitBasicBlock = nullptr;

public:
	AstTACGenVisitor(int currOffset) : _module(nullptr), _currentOffset(currOffset), constCounter(1), basicBlockCounter(1), tempCounter(1) { }

	void visit(Parser::CompilationUnit* module);
	void visit(Parser::Function* module);
	void visit(const Parser::ParamDecl* module);
	void visit(const Parser::VarSpec* module);
	void visit(const Parser::VarDecl* module);
	void visit(Parser::LoopStmt* module);
	void visit(Parser::IfStmt* module);
	void visit(Parser::ElseIfStmt* module);
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

	void translateBooleanExp(Parser::BinaryExpr* binop);
	void translateArithmeticExp(Parser::BinaryExpr* binop);
	void translateRelationalExp(Parser::BinaryExpr* binop);
	void translateArithmeticExpr(Parser::UnaryExpr* unary);
	void translateBooleanExp(Parser::UnaryExpr* unary);

	void emitBranchesBasedOnExpValue(Instruction_sptr, BasicBlock_sptr lTrue, BasicBlock_sptr lFalse);

	template<typename T>
	shared_ptr<STConstantDef> newConstant(T value);

	shared_ptr<STLabelDef> newLabel(string scope, string suffix="");

	shared_ptr<IR::Register> newTemporary(NativeType type);

	BasicBlock_sptr newBasicBlock() { return make_shared<IR::BasicBlock>(0); }
};

#endif /* ASTVISITORS_H_ */

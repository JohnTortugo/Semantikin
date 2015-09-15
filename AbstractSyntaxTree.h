#ifndef ABSTRACTSYNTAXTREE_H_
#define ABSTRACTSYNTAXTREE_H_

#include "SymbolTable.h"
#include "location.hh"

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <list>

using std::string;
using std::list;
using std::shared_ptr;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

class AstTreeVisitor;

namespace Parser {
	class AstNode {
	private:
		location _loc;

	public:
		AstNode()
		{ }

		AstNode(location loc) : _loc(loc)
		{ }

		virtual const location& loc() { return this->_loc; }
		virtual void loc(location loct) { this->_loc = loct; }

		virtual void accept(AstTreeVisitor* visitor) = 0;

		virtual ~AstNode() { };
	};

	class Statement : public AstNode {
	protected:
		/* Pointer to a label after the current AST node. */
		BasicBlock_sptr _next = nullptr;

	public:
		Statement()
		{ }

		Statement(location loc) : AstNode(loc)
		{ }

		void next(BasicBlock_sptr label) { this->_next = label; }
		BasicBlock_sptr next() { return this->_next; }
	};

	class Expression : public Statement {
	protected:
		NativeType _type = NOT_A_TYPE;
		shared_ptr<SymbolTableEntry> _addr = nullptr;

		/* This indicate if the current expression being decoded is a left-hand
		 * or right-hand. If it is left-hand then, if the expression is a identifier
		 * array, it is not deferenced instead a pointer to the location is returned.
		 * If it is a right-hand then the value is loaded into a temporary. 	   */
		bool _isExpLeftHand = false;

		/* Tells if the expressions turns out to be an array access. */
		bool _isArrayAccess = false;

		/* These variables indicate labels that are target of the result of
		 * evaluating the current expression. If these targets are null it
		 * means that the current expression is not part of a short circuit
		 * code (e.g., it is an arithmetic/relation expression). */
		BasicBlock_sptr _tLabel = nullptr;
		BasicBlock_sptr _fLabel = nullptr;

	public:
		Expression()
		{ }

		Expression(location loc) : Statement(loc)
		{ }

		NativeType type() const { return _type; }
		void type(NativeType _etype) { _type = _etype; }

		void addr(shared_ptr<SymbolTableEntry> addr) { this->_addr = addr; }
		shared_ptr<SymbolTableEntry> addr() { return this->_addr; }

		void isExpLeftHand(bool isLeft) { this->_isExpLeftHand = isLeft; }
		bool isExpLeftHand() { return this->_isExpLeftHand; }

		void tLabel(BasicBlock_sptr label) { this->_tLabel = label; }
		BasicBlock_sptr tLabel() { return this->_tLabel; }

		void fLabel(BasicBlock_sptr label) { this->_fLabel = label; }
		BasicBlock_sptr fLabel() { return this->_fLabel; }

		void isArrayAccess(bool isArray) { this->_isArrayAccess = isArray; }
		bool isArrayAccess() { return this->_isArrayAccess; }
	};

	class BinaryExpr : public Expression {
	public:
		enum ExprType {
			COMPARE,
			DIFFERENCE,
			ASSIGN,
			LOG_AND,
			LOG_OR,
			BIT_AND,
			BIT_OR,
			BIT_XOR,
			LT,
			LTE,
			GT,
			GTE,
			ADDITION,
			SUBTRACTION,
			TIMES,
			DIV,
			MOD,
			PLUS_EQUAL,
			MINUS_EQUAL,
			TIMES_EQUAL,
			DIV_EQUAL,
			MOD_EQUAL,
		};

	private:
		ExprType _opr;
		shared_ptr<Expression> _exp1;
		shared_ptr<Expression> _exp2;

	public:
		BinaryExpr(ExprType opr, location loc, Expression* exp1, Expression* exp2) :
			Expression(loc),
			_opr(opr),
			_exp1(exp1),
			_exp2(exp2)
		{ }

		void accept(AstTreeVisitor* visitor);

		ExprType opr() const { return this->_opr; }

		Expression* getExp1() const { return this->_exp1.get(); }

		Expression* getExp2() const { return this->_exp2.get(); }
	};

	class UnaryExpr : public Expression {
	public:
		enum ExprType {
			BIT_NOT,
			NOT,
			MINUS,
			ADDR,
			INCREMENT,
			DECREMENT
		};

	private:
		ExprType _opr;
		shared_ptr<Expression> _exp;

	public:
		UnaryExpr(ExprType type, location loc, Expression* exp) :
			Expression(loc),
			_opr(type),
			_exp(shared_ptr<Expression>(exp))
		{ }

		ExprType opr() const { return this->_opr; }

		Expression* exp() const { return this->_exp.get(); }

		void accept(AstTreeVisitor* visitor);
	};

	class FunctionCall : public Expression {
	private:
		string _name;
		shared_ptr<list<shared_ptr<Expression>>> _arguments;

	public:
		FunctionCall(location loc, string name, list<shared_ptr<Expression>>* arguments) :
			Expression(loc),
			_name(name),
			_arguments(shared_ptr<list<shared_ptr<Expression>>>(arguments))
		{ }

		void accept(AstTreeVisitor* visitor);

		string name() const { return this->_name; }

		list<shared_ptr<Expression>>* arguments() const { return this->_arguments.get(); }
	};

	class IdentifierExpr : public Expression {
	private:
		string _value;
		shared_ptr<list<shared_ptr<Expression>>> _dimExprs;

	public:
		IdentifierExpr(location loc, string value,  list<shared_ptr<Expression>>* dimExprs) :
			Expression(loc),
			_value(value),
			_dimExprs(shared_ptr<list<shared_ptr<Expression>>>(dimExprs))
		{ }

		void accept(AstTreeVisitor* visitor);

		string value() const { return this->_value; }

		list<shared_ptr<Expression>>* dimsExprs() const { return this->_dimExprs.get(); }
	};

	class IntegerExpr : public Expression {
	private:
		int _value;

	public:
		IntegerExpr(location loc, int _value) : Expression(loc), _value(_value) {}

		void accept(AstTreeVisitor* visitor);

		int value() const { return this->_value; }
	};

	class FloatExpr : public Expression {
	private:
		float _value;

	public:
		FloatExpr(location loc, float _value) : Expression(loc), _value(_value) {}

		void accept(AstTreeVisitor* visitor);

		float value() const { return this->_value; }
	};

	class StringExpr : public Expression {
	private:
		string _value;

	public:
		StringExpr(location loc, string _value) : Expression(loc), _value(_value) {}

		void accept(AstTreeVisitor* visitor);

		string value() const { return this->_value; }
	};

	class CodeBlock : public Statement {
	private:
		shared_ptr<SymbolTable> _symbTable;
		shared_ptr<list<shared_ptr<Statement>>> _statements;

	public:
		CodeBlock(location loc, list<shared_ptr<Statement>>* _stmts) :
			Statement(loc),
			_statements(shared_ptr<list<shared_ptr<Statement>>>(_stmts))
		{ }

		void accept(AstTreeVisitor* visitor);

		shared_ptr<SymbolTable> getSymbTable() const { return this->_symbTable; }

		void setSymbTable(shared_ptr<SymbolTable> _symbTable) { this->_symbTable = _symbTable; }

		list<shared_ptr<Statement>>* getStatements() const { return this->_statements.get(); }
	};

	class ReturnStmt : public Statement {
	private:
		shared_ptr<Expression> _expression;

	public:
		ReturnStmt(location loc, Expression* _exp) :
			Statement(loc),
			_expression(shared_ptr<Expression>(_exp))
		{ }

		void accept(AstTreeVisitor* visitor);

		Expression* getExpression() const { return this->_expression.get(); }
	};

	class ElseIfStmt : public Statement {
	private:
		shared_ptr<Expression> _condition;
		shared_ptr<CodeBlock> _elseIfBlock;

	public:
		ElseIfStmt(location loc, Expression* _condition, CodeBlock* _elseIfBlock) :
			Statement(loc),
			_condition(shared_ptr<Expression>(_condition)),
			_elseIfBlock(shared_ptr<CodeBlock>(_elseIfBlock))
		{ }

		void accept(AstTreeVisitor* visitor);

		Expression* getCondition() const { return this->_condition.get(); }

		CodeBlock* getElseIfBlock() const { return this->_elseIfBlock.get(); }
	};

	class IfStmt : public Statement {
	private:
		shared_ptr<Expression> _condition;
		shared_ptr<CodeBlock> _thenBlock;
		shared_ptr<list<shared_ptr<ElseIfStmt>>> _elseIfChain;
		shared_ptr<CodeBlock> _elseBlock;

	public:
		IfStmt(location loc, Expression* _condition, CodeBlock* _then, CodeBlock* _else, list<shared_ptr<ElseIfStmt>>* _elseIfChain) :
			Statement(loc),
			_condition(shared_ptr<Expression>(_condition)),
			_thenBlock(shared_ptr<CodeBlock>(_then)),
			_elseBlock(shared_ptr<CodeBlock>(_else)),
			_elseIfChain(shared_ptr<list<shared_ptr<ElseIfStmt>>>(_elseIfChain))
		{ }

		Expression* getCondition() const { return this->_condition.get(); }

		CodeBlock* getThenBlock() const { return this->_thenBlock.get(); }

		list<shared_ptr<ElseIfStmt>>* getElseIfChain() const { return this->_elseIfChain.get(); }

		CodeBlock* getElseBlock() const { return this->_elseBlock.get(); }

		void accept(AstTreeVisitor* visitor);
	};

	class LoopStmt : public Statement {
	private:
		shared_ptr<Expression> _condition;
		shared_ptr<CodeBlock> _body;

	public:
		LoopStmt(location loc, Expression* condition, CodeBlock* body) :
			Statement(loc),
			_condition(shared_ptr<Expression>(condition)),
			_body(shared_ptr<CodeBlock>(body))
		{ }

		void accept(AstTreeVisitor* visitor);

		Expression* getCondition() const { return this->_condition.get(); }

		CodeBlock* getBody() const { return this->_body.get(); }
	};

	class VarSpec : public Statement {
	private:
		string _name;
		shared_ptr<list<shared_ptr<Expression>>> _dimsExprs;
		shared_ptr<Expression> _initializer;

	public:
		VarSpec(location loc, string name, list<shared_ptr<Expression>>* dims, Expression* init) :
			Statement(loc),
			_name(name),
			_dimsExprs(shared_ptr<list<shared_ptr<Expression>>>(dims)),
			_initializer(shared_ptr<Expression>(init))
		{ }

		string getName() const { return this->_name; }

		Expression* getInitializer() const { return this->_initializer.get(); }

		list<shared_ptr<Expression>>* getDimsExpr() const { return this->_dimsExprs.get(); }

		void accept(AstTreeVisitor* visitor);
	};

	class VarDecl : public Statement {
	private:
		string _type;
		shared_ptr<list<shared_ptr<VarSpec>>> _vars;

	public:
		VarDecl(location loc, string type, list<shared_ptr<VarSpec>>* vars) :
			Statement(loc),
			_type(type),
			_vars(shared_ptr<list<shared_ptr<VarSpec>>>(vars))
		{ }

		string getType() const { return this->_type; }

		list<shared_ptr<VarSpec>>* getVars() const { return this->_vars.get(); }

		void accept(AstTreeVisitor* visitor);
	};

	class ParamDecl : public Statement {
	private:
		string _type;
		string _name;
		location _nmLoc;
		shared_ptr<list<shared_ptr<Expression>>> _dims;

	public:
		ParamDecl(location tpLoc, location nmLoc, string type, string name, shared_ptr<list<shared_ptr<Expression>>> dims) :
			Statement(tpLoc),
			_nmLoc(nmLoc),
			_type(type),
			_name(name),
			_dims(dims)
		{ }

		list<shared_ptr<Expression>>* getDims() const { return _dims.get(); }

		const location& tpLoc() { return this->loc(); }
		const location& nmLoc() { return this->_nmLoc; }

		const string& getName() const { return _name; }

		const string& getType() const { return _type; }

		void accept(AstTreeVisitor* visitor);
	};

	class Function : public AstNode {
	private:
		string _returnType;
		string _name;
		location _nmLoc;
		shared_ptr<list<shared_ptr<ParamDecl>>> _params;
		shared_ptr<CodeBlock> _body;
		shared_ptr<SymbolTable> _symbTable;
		int _currentOffset;

	public:
		Function(location tpLoc, location nmLoc, string type, string name, shared_ptr<list<shared_ptr<ParamDecl>>> params, CodeBlock* body) :
			AstNode(tpLoc),
			_nmLoc(nmLoc),
			_returnType(type),
			_name(name),
			_params(params),
			_body(shared_ptr<CodeBlock>(body)),
			_symbTable(nullptr),
			_currentOffset(0)
		{ }

		string getName() const { return this->_name; }

		string getReturnType() const { return this->_returnType; }

		list<shared_ptr<ParamDecl>>* getParams() const { return this->_params.get(); }

		CodeBlock* getBody() const { return this->_body.get(); }

		shared_ptr<SymbolTable> getSymbTable() const { return this->_symbTable; }

		void setSymbTable(shared_ptr<SymbolTable> _symbTable) { this->_symbTable = _symbTable; }

		int currentOffset() const { return this->_currentOffset; }
		void currentOffset(int ofts) { this->_currentOffset = ofts; }

		void accept(AstTreeVisitor* visitor);
	};

	class CompilationUnit : public AstNode {
	private:
		shared_ptr<list<shared_ptr<Function>>> functions;
		shared_ptr<SymbolTable> symbTable;

	public:
		CompilationUnit(list<shared_ptr<Function>>* _funcs) {
			this->functions = shared_ptr<list<shared_ptr<Function>>>( _funcs );
		}

		shared_ptr<list<shared_ptr<Function>>> getFunctions() const { return this->functions; }

		shared_ptr<SymbolTable> getSymbTable() const { return this->symbTable; }

		void setSymbTable(shared_ptr<SymbolTable> _symbTable) { this->symbTable = _symbTable; }

		void accept(AstTreeVisitor* visitor);
	};
}

#endif /* ABSTRACTSYNTAXTREE_H_ */

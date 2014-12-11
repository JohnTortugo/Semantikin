#ifndef ABSTRACTSYNTAXTREE_H_
#define ABSTRACTSYNTAXTREE_H_

#include "SymbolTable.h"

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

class AstNodeVisitor;

namespace Parser {
	class AstNode {
	public:
		virtual void accept(AstNodeVisitor* visitor) = 0;

		virtual ~AstNode() { };
	};

	class Statement : public AstNode {};

	class Expression : public Statement {
	protected:
		NativeType _expType;
		shared_ptr<SymbolTableEntry> _addr;

		/* This indicate if the current expression being decoded is a left-hand
		 * or right-hand. If it is left-hand then, if the expression is a identifier
		 * array, it is not deferenced instead a pointer to the location is returned.
		 * If it is a right-hand then the value is loaded into a temporary. 	   */
		bool _isExpLeftHand = false;

		/* Tells if the expressions turns out to be an array access. */
		bool _isArrayAccess = false;

		/* This indicates if the RelationalExpression being analyzed is a
		 * expression inside a conditional instruction or not. If it is, due to
		 * short-circuit behavior, we may need to insert jumps (if's) inside
		 * the expression IR's instruction. 								 */
		bool _isExpInConditional = false;

	public:
		NativeType exprType() const { return _expType; }
		void exprType(NativeType _etype) { _expType = _etype; }

		void addr(shared_ptr<SymbolTableEntry> addr) { this->_addr = addr; }
		shared_ptr<SymbolTableEntry> addr() { return this->_addr; }

		void isExpLeftHand(bool isLeft) { this->_isExpLeftHand = isLeft; }
		bool isExpLeftHand() { return this->_isExpLeftHand; }

		void isExpInConditional(bool isCond) { this->_isExpInConditional = isCond; }
		bool isExpInConditional() { return this->_isExpInConditional; }

		void isArrayAccess(bool isArray) { this->_isArrayAccess = isArray; }
		bool isArrayAccess() { return this->_isArrayAccess; }
	};

	class BinaryExpr : public Expression {
	public:
		enum ExprType {
			COMPARE,
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
		ExprType type;
		shared_ptr<Expression> exp1;
		shared_ptr<Expression> exp2;

	public:
		BinaryExpr(ExprType _type, Expression* _exp1, Expression* _exp2) {
			this->type = _type;
			this->exp1 = shared_ptr<Expression>(_exp1);
			this->exp2 = shared_ptr<Expression>(_exp2);
		}

		void accept(AstNodeVisitor* visitor);

		ExprType getType() const { return this->type; }

		Expression* getExp1() const { return this->exp1.get(); }

		Expression* getExp2() const { return this->exp2.get(); }
	};

	class UnaryExpr : public Expression {
	public:
		enum ExprType {
			NOT,
			MINUS,
			PLUS,
			ADDR,
			INCREMENT,
			DECREMENT
		};

	private:
		ExprType type;
		shared_ptr<Expression> exp;

	public:
		UnaryExpr(ExprType _type, Expression* _exp) {
			this->type = _type;
			this->exp = shared_ptr<Expression>(_exp);
		}

		ExprType getType() const { return this->type; }

		Expression* getExp() const { return this->exp.get(); }

		void accept(AstNodeVisitor* visitor);
	};

	class FunctionCall : public Expression {
	private:
		string name;
		shared_ptr<list<shared_ptr<Expression>>> arguments;

	public:
		FunctionCall(string _name, list<shared_ptr<Expression>>* _arguments) {
			this->name = _name;
			this->arguments = shared_ptr<list<shared_ptr<Expression>>>(_arguments);
		}

		void accept(AstNodeVisitor* visitor);

		string getName() const { return this->name; }

		list<shared_ptr<Expression>>* getArguments() const { return this->arguments.get(); }
	};

	class IdentifierExpr : public Expression {
	private:
		string value;
		shared_ptr<list<shared_ptr<Expression>>> dimExprs;

	public:
		IdentifierExpr(string _value,  list<shared_ptr<Expression>>* _dimExprs) {
			this->value			= _value;
			this->dimExprs		= shared_ptr<list<shared_ptr<Expression>>>(_dimExprs);
		}

		void accept(AstNodeVisitor* visitor);

		string getValue() const { return this->value; }

		list<shared_ptr<Expression>>* getDimsExprs() const { return this->dimExprs.get(); }
	};

	class IntegerExpr : public Expression {
	private:
		int value;

	public:
		IntegerExpr(int _value) : value(_value) {}

		void accept(AstNodeVisitor* visitor);

		int getValue() const { return this->value; }
	};

	class FloatExpr : public Expression {
	private:
		float value;

	public:
		FloatExpr(float _value) : value(_value) {}

		void accept(AstNodeVisitor* visitor);

		float getValue() const { return this->value; }
	};

	class StringExpr : public Expression {
	private:
		string value;

	public:
		StringExpr(string _value) : value(_value) {}

		void accept(AstNodeVisitor* visitor);

		string getValue() const { return this->value; }
	};

	class CodeBlock : public Statement {
	private:
		shared_ptr<SymbolTable> symbTable;
		shared_ptr<list<shared_ptr<Statement>>> statements;

	public:
		CodeBlock(list<shared_ptr<Statement>>* _stmts) {
			this->statements 	= shared_ptr<list<shared_ptr<Statement>>>(_stmts);
		}

		void accept(AstNodeVisitor* visitor);

		shared_ptr<SymbolTable> getSymbTable() const { return this->symbTable; }

		void setSymbTable(shared_ptr<SymbolTable> _symbTable) { this->symbTable = _symbTable; }

		list<shared_ptr<Statement>>* getStatements() const { return this->statements.get(); }
	};

	class ReturnStmt : public Statement {
	private:
		shared_ptr<Expression> expression;

	public:
		ReturnStmt(Expression* _exp) {
			this->expression = shared_ptr<Expression>(_exp);
		}

		void accept(AstNodeVisitor* visitor);

		Expression* getExpression() const { return this->expression.get(); }
	};

	class ElseIfStmt : public Statement {
	private:
		shared_ptr<Expression> condition;
		shared_ptr<CodeBlock> elseIfBlock;

	public:
		ElseIfStmt(Expression* _condition, CodeBlock* _elseIfBlock) {
			this->condition = shared_ptr<Expression>(_condition);
			this->elseIfBlock = shared_ptr<CodeBlock>(_elseIfBlock);
		}

		void accept(AstNodeVisitor* visitor);

		Expression* getCondition() const { return this->condition.get(); }

		CodeBlock* getElseIfBlock() const { return this->elseIfBlock.get(); }
	};

	class IfStmt : public Statement {
	private:
		shared_ptr<Expression> condition;
		shared_ptr<CodeBlock> thenBlock;
		shared_ptr<list<shared_ptr<ElseIfStmt>>> elseIfChain;
		shared_ptr<CodeBlock> elseBlock;

	public:
		IfStmt(Expression* _condition, CodeBlock* _then, CodeBlock* _else, list<shared_ptr<ElseIfStmt>>* _elseIfChain) {
			this->condition 	= shared_ptr<Expression>(_condition);
			this->thenBlock 	= shared_ptr<CodeBlock>(_then);
			this->elseBlock 	= shared_ptr<CodeBlock>(_else);
			this->elseIfChain 	= shared_ptr<list<shared_ptr<ElseIfStmt>>>(_elseIfChain);
		}

		Expression* getCondition() const { return this->condition.get(); }

		CodeBlock* getThenBlock() const { return this->thenBlock.get(); }

		list<shared_ptr<ElseIfStmt>>* getElseIfChain() const { return this->elseIfChain.get(); }

		CodeBlock* getElseBlock() const { return this->elseBlock.get(); }

		void accept(AstNodeVisitor* visitor);
	};

	class LoopStmt : public Statement {
	private:
		shared_ptr<Expression> condition;
		shared_ptr<CodeBlock> body;

	public:
		LoopStmt(Expression* _condition, CodeBlock* _body) {
			this->condition = shared_ptr<Expression>(_condition);
			this->body 		= shared_ptr<CodeBlock>(_body);
		}

		void accept(AstNodeVisitor* visitor);

		Expression* getCondition() const { return this->condition.get(); }

		CodeBlock* getBody() const { return this->body.get(); }
	};

	class VarSpec : public Statement {
	private:
		string name;
		shared_ptr<list<shared_ptr<Expression>>> dimsExprs;
		shared_ptr<Expression> initializer;

	public:
		VarSpec(string _name, list<shared_ptr<Expression>>* _dims, Expression* _init) {
			this->name 			= _name;
			this->dimsExprs 	= shared_ptr<list<shared_ptr<Expression>>>(_dims);
			this->initializer 	= shared_ptr<Expression>(_init);
		}

		string getName() const { return this->name; }

		Expression* getInitializer() const { return this->initializer.get(); }

		list<shared_ptr<Expression>>* getDimsExpr() const { return this->dimsExprs.get(); }

		void accept(AstNodeVisitor* visitor);
	};

	class VarDecl : public Statement {
	private:
		string type;
		shared_ptr<list<shared_ptr<VarSpec>>> vars;

	public:
		VarDecl(string _type, list<shared_ptr<VarSpec>>* _vars) {
			this->type = _type;
			this->vars = shared_ptr<list<shared_ptr<VarSpec>>>(_vars);
		}

		string getType() const { return this->type; }

		list<shared_ptr<VarSpec>>* getVars() const { return this->vars.get(); }

		void accept(AstNodeVisitor* visitor);
	};

	class ParamDecl : public Statement {
	private:
		string type;
		string name;
		shared_ptr<list<shared_ptr<Expression>>> dims;

	public:
		ParamDecl(string _type, string _name, shared_ptr<list<shared_ptr<Expression>>> _dims) {
			this->type = _type;
			this->name = _name;
			this->dims = _dims;
		}

		list<shared_ptr<Expression>>* getDims() const { return dims.get(); }

		const string& getName() const { return name; }

		const string& getType() const { return type; }

		void accept(AstNodeVisitor* visitor);
	};

	class Function : public AstNode {
	private:
		string returnType;
		string name;
		shared_ptr<list<shared_ptr<ParamDecl>>> params;
		shared_ptr<CodeBlock> body;
		shared_ptr<SymbolTable> symbTable;

	public:
		Function(string _type, string _name, shared_ptr<list<shared_ptr<ParamDecl>>> _params, CodeBlock* _body) {
			this->returnType = _type;
			this->name = _name;
			this->params = _params;
			this->body = shared_ptr<CodeBlock>(_body);
		}

		string getName() const { return this->name; }

		string getReturnType() const { return this->returnType; }

		list<shared_ptr<ParamDecl>>* getParams() const { return this->params.get(); }

		CodeBlock* getBody() const { return this->body.get(); }

		shared_ptr<SymbolTable> getSymbTable() const { return this->symbTable; }

		void setSymbTable(shared_ptr<SymbolTable> _symbTable) { this->symbTable = _symbTable; }

		void accept(AstNodeVisitor* visitor);
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

		void accept(AstNodeVisitor* visitor);
	};
}

#endif /* ABSTRACTSYNTAXTREE_H_ */

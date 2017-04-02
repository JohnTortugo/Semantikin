#include <iostream>
#include <string>
#include <list>
#include "AbstractSyntaxTree.h"
#include "AstVisitors.h"
using namespace std;

namespace Parser {
	void BinaryExpr::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void UnaryExpr::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void FunctionCall::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void IdentifierExpr::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void IntegerExpr::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void CodeBlock::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void ReturnStmt::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void ElseIfStmt::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void IfStmt::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void LoopStmt::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void VarSpec::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void VarDecl::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void ParamDecl::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void Function::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

	void CompilationUnit::accept(AstTreeVisitor* visitor) { visitor->visit(this); }

}

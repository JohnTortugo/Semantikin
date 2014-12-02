#include <iostream>
#include <string>
#include <list>
#include "AbstractSyntaxTree.h"
#include "AstVisitors.h"
using namespace std;

namespace Parser {
	void BinaryExpr::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void UnaryExpr::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void FunctionCall::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void IdentifierExpr::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void IntegerExpr::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void FloatExpr::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void StringExpr::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void CodeBlock::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void ReturnStmt::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void ElseIfStmt::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void IfStmt::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void LoopStmt::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void VarSpec::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void VarDecl::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void ParamDecl::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void Function::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

	void CompilationUnit::accept(AstNodeVisitor* visitor) { visitor->visit(this); }

}

#ifndef IRVISITORS_H_
#define IRVISITORS_H_ 1

#include <iostream>
#include <tuple>
#include "IR.h"

class IRTreeVisitor {
public:
	virtual void visit(IR::Module*) = 0;
	virtual void visit(IR::Function*) = 0;
	virtual void visit(IR::BasicBlock* bb) = 0;

	virtual void visit(IR::ScalarCopy*) = 0;
	virtual void visit(IR::CopyFromArray* visitor) = 0;
	virtual void visit(IR::CopyToArray* visitor) = 0;

	virtual void visit(IR::Register*) = 0;
	virtual void visit(IR::Memory*) = 0;
	virtual void visit(IR::Immediate*) = 0;
	virtual void visit(IR::Func*) = 0;

	virtual void visit(IR::IAdd* visitor) = 0;
	virtual void visit(IR::ISub* visitor) = 0;
	virtual void visit(IR::IMul* visitor) = 0;
	virtual void visit(IR::IDiv* visitor) = 0;
	virtual void visit(IR::IMod* visitor) = 0;
	virtual void visit(IR::IMinus* visitor) = 0;
	virtual void visit(IR::IInc* visitor) = 0;
	virtual void visit(IR::IDec* visitor) = 0;

	virtual void visit(IR::FAdd* visitor) = 0;
	virtual void visit(IR::FSub* visitor) = 0;
	virtual void visit(IR::FMul* visitor) = 0;
	virtual void visit(IR::FDiv* visitor) = 0;
	virtual void visit(IR::FMinus* visitor) = 0;
	virtual void visit(IR::FInc* visitor) = 0;
	virtual void visit(IR::FDec* visitor) = 0;

	virtual void visit(IR::BinAnd* visitor) = 0;
	virtual void visit(IR::BinOr* visitor) = 0;
	virtual void visit(IR::BinXor* visitor) = 0;
	virtual void visit(IR::BinNot* visitor) = 0;

	virtual void visit(IR::RLesThan* visitor) = 0;
	virtual void visit(IR::RLesThanEqual* visitor) = 0;
	virtual void visit(IR::RGreaterThan* visitor) = 0;
	virtual void visit(IR::RGreaterThanEqual* visitor) = 0;
	virtual void visit(IR::REqual* visitor) = 0;
	virtual void visit(IR::RNotEqual* visitor) = 0;

	virtual void visit(IR::Jump* visitor) = 0;
	virtual void visit(IR::Conditional* visitor) = 0;

	virtual void visit(IR::Addr* visitor) = 0;
	virtual void visit(IR::AddrDispl* visitor) = 0;
	virtual void visit(IR::Call* visitor) = 0;
	virtual void visit(IR::Return* visitor) = 0;
	virtual void visit(IR::Phi* visitor) = 0;

	virtual ~IRTreeVisitor() {};
};

class IRToDotVisitor : public IRTreeVisitor {
private:
	std::ofstream dotfile;

	/** Used to specify the identation of the next line */
	unsigned int _identation = 0;


	/**
	 * Stores a pointer to the address of the basic block currently being processed.
	 */
	IR::BasicBlock* _currentBasicBlock;

	/** Used to add a new line to the output */
	std::ofstream& newline() {
		for (int i=0; i<this->_identation; i++) this->dotfile << "\t";

		return this->dotfile;
	}

	void incIdentation() { this->_identation++; }
	void decIdentation() { this->_identation--; }

	/**
	 * Used to implement the tree-way accept methods for binary operations.
	 * This way we don't need to replicate the code. */
	void binaryDispatcher(IR::Instruction* node, const char* label);
	void unaryDispatcher(IR::Instruction* node, const char* label);


	/**
	 * Used to store the source/target of edges across basic blocks. This
	 * is necessary because otherwise DOT would not understand the output
	 * correctly.
	 */
	std::vector< std::tuple<int, int, string> > crossEdges;


public:
	IRToDotVisitor(string filename) {
		this->dotfile.open(filename);
		this->dotfile << "digraph IRGraph {" << endl;
		this->dotfile << "	node [fontsize=10, style=\"rounded,filled\", fillcolor=\"#ffffff\" width=\".2\", height=\".2\", margin=0];" << endl;
		this->dotfile << "	graph [fontsize=8];" << endl;
		this->dotfile << "	edge [fontsize=8];" << endl;
		this->dotfile << "	compound=true;" << endl;
		this->dotfile << "	label=\"This is the IR Tree for " << filename << " \";" << endl;
	}

	~IRToDotVisitor() {
		this->dotfile << "}" << endl;
		this->dotfile.close();
	}

	void visit(IR::Module*);
	void visit(IR::Function*);
	void visit(IR::BasicBlock* bb);

	void visit(IR::ScalarCopy*);
	void visit(IR::CopyFromArray* visitor);
	void visit(IR::CopyToArray* visitor);

	void visit(IR::Register*);
	void visit(IR::Memory*);
	void visit(IR::Immediate*);
	void visit(IR::Func*);

	void visit(IR::IAdd* visitor);
	void visit(IR::ISub* visitor);
	void visit(IR::IMul* visitor);
	void visit(IR::IDiv* visitor);
	void visit(IR::IMod* visitor);
	void visit(IR::IMinus* visitor);
	void visit(IR::IInc* visitor);
	void visit(IR::IDec* visitor);

	void visit(IR::FAdd* visitor);
	void visit(IR::FSub* visitor);
	void visit(IR::FMul* visitor);
	void visit(IR::FDiv* visitor);
	void visit(IR::FMinus* visitor);
	void visit(IR::FInc* visitor);
	void visit(IR::FDec* visitor);

	void visit(IR::BinAnd* visitor);
	void visit(IR::BinOr* visitor);
	void visit(IR::BinXor* visitor);
	void visit(IR::BinNot* visitor);

	void visit(IR::RLesThan* visitor);
	void visit(IR::RLesThanEqual* visitor);
	void visit(IR::RGreaterThan* visitor);
	void visit(IR::RGreaterThanEqual* visitor);
	void visit(IR::REqual* visitor);
	void visit(IR::RNotEqual* visitor);

	void visit(IR::Jump* visitor);
	void visit(IR::Conditional* visitor);

	void visit(IR::Addr* visitor);
	void visit(IR::AddrDispl* visitor);
	void visit(IR::Call* visitor);
	void visit(IR::Return* visitor);
	void visit(IR::Phi* visitor);
};

#endif /* IRVISITORS_H_ */

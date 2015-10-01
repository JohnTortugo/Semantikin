#ifndef MAXIMAL_MUNCH_H
#define MAXIMAL_MUNCH_H 1

#include "InstructionSelection.h"

class MaximalMunch : public InstructionSelection {
private:


public:
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

#endif

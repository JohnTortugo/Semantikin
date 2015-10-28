#include "TreeCanonicalizer.h"

void TreeCanonicalizer::updateNext(Instruction* node) {
	if (this->prevInstruction != nullptr) {
		this->prevInstruction->next(node);
	}
	else {
		this->firstInstruction = node;
	}

	node->prev(this->prevInstruction);
	this->prevInstruction = node;
}

void TreeCanonicalizer::binaryDispatcher(IR::Instruction* node) {
	node->chd2()->accept(this);
	node->chd3()->accept(this);
	updateNext(node);
}

void TreeCanonicalizer::unaryDispatcher(IR::Instruction* node) {
	node->chd2()->accept(this);
	updateNext(node);
}

void TreeCanonicalizer::visit(IR::Module* module) {
	for (auto& func : *(module->functions())) {
		func->accept(this);
	}
}

void TreeCanonicalizer::visit(IR::Function* func) {
	for (auto& bb : *(func->bbs())) {
		bb->accept(this);
	}
}

void TreeCanonicalizer::visit(IR::BasicBlock* bb) {  
	// At first we don't know which is the first instruction
	this->firstInstruction = nullptr;
	this->prevInstruction = nullptr;

	// Iterate over each three connecting the "next" pointers
	for (auto& tree : *(bb->subtrees())) {
		tree->accept(this);
	}

	// Now we know which instruction was the first and we also set the next of the last to nullptr
	if (prevInstruction != nullptr) {
		assert(firstInstruction != nullptr && "First instruction is null in TreeCanonicalizer.");

		bb->firstInstruction(this->firstInstruction);
		bb->lastInstruction(this->prevInstruction);
		this->prevInstruction->next(nullptr);
	}	
}

void TreeCanonicalizer::visit(IR::ScalarCopy* node) {
	unaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::CopyFromArray* node) {
	unaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::CopyToArray* node) {
	unaryDispatcher(node);
}


void TreeCanonicalizer::visit(IR::Register* reg) {
}

void TreeCanonicalizer::visit(IR::Memory* mem) {
}

void TreeCanonicalizer::visit(IR::Immediate* imm) {
}

void TreeCanonicalizer::visit(IR::Func* func) {
}

void TreeCanonicalizer::visit(IR::IAdd* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::ISub* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::IMul* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::IDiv* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::IMod* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::IMinus* node) {
	unaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::IInc* node) {
	unaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::IDec* node) {
	unaryDispatcher(node);
}





void TreeCanonicalizer::visit(IR::FAdd* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::FSub* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::FMul* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::FDiv* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::FMinus* node) {
	unaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::FInc* node) {
	unaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::FDec* node) {
	unaryDispatcher(node);
}






void TreeCanonicalizer::visit(IR::BinAnd* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::BinOr* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::BinXor* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::BinNot* node) {
	unaryDispatcher(node);
}



void TreeCanonicalizer::visit(IR::RLesThan* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::RLesThanEqual* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::RGreaterThan* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::RGreaterThanEqual* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::REqual* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::RNotEqual* node) {
	binaryDispatcher(node);
}



void TreeCanonicalizer::visit(IR::Jump* node) { 
	updateNext(node);
}

void TreeCanonicalizer::visit(IR::Conditional* node) { 
	node->tgt()->accept(this);
	updateNext(node);
}



void TreeCanonicalizer::visit(IR::Addr* node) {
	unaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::AddrDispl* node) {
	binaryDispatcher(node);
}

void TreeCanonicalizer::visit(IR::Call* node) { 
	if (node->arguments() != nullptr) {
		for (auto& arg : *node->arguments()) {
			arg.get()->accept(this);
		}
	}

	node->tgt()->accept(this);
	node->chd2()->accept(this);
	updateNext(node);
}

void TreeCanonicalizer::visit(IR::Return* node) { 
	node->tgt()->accept(this);
	updateNext(node);
}


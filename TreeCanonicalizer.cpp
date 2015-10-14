#include "TreeCanonicalizer.h"

using namespace IR;

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
	for (auto& tree : *(bb->instructions())) {
		tree->accept(this);
	}
}

void TreeCanonicalizer::visit(IR::ScalarCopy* node) {
}

void TreeCanonicalizer::visit(IR::CopyFromArray* node) {
}

void TreeCanonicalizer::visit(IR::CopyToArray* node) {
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
}

void TreeCanonicalizer::visit(IR::ISub* node) {
}

void TreeCanonicalizer::visit(IR::IMul* node) {
}

void TreeCanonicalizer::visit(IR::IDiv* node) {
}

void TreeCanonicalizer::visit(IR::IMod* node) {
}

void TreeCanonicalizer::visit(IR::IMinus* node) {
}

void TreeCanonicalizer::visit(IR::IInc* node) {
}

void TreeCanonicalizer::visit(IR::IDec* node) {
}





void TreeCanonicalizer::visit(IR::FAdd* node) {
}

void TreeCanonicalizer::visit(IR::FSub* node) {
}

void TreeCanonicalizer::visit(IR::FMul* node) {
}

void TreeCanonicalizer::visit(IR::FDiv* node) {
}

void TreeCanonicalizer::visit(IR::FMinus* node) {
}

void TreeCanonicalizer::visit(IR::FInc* node) {
}

void TreeCanonicalizer::visit(IR::FDec* node) {
}






void TreeCanonicalizer::visit(IR::BinAnd* node) {
}

void TreeCanonicalizer::visit(IR::BinOr* node) {
}

void TreeCanonicalizer::visit(IR::BinXor* node) {
}

void TreeCanonicalizer::visit(IR::BinNot* node) {
}



void TreeCanonicalizer::visit(IR::RLesThan* node) {
}

void TreeCanonicalizer::visit(IR::RLesThanEqual* node) {
}

void TreeCanonicalizer::visit(IR::RGreaterThan* node) {
}

void TreeCanonicalizer::visit(IR::RGreaterThanEqual* node) {
}

void TreeCanonicalizer::visit(IR::REqual* node) {
}

void TreeCanonicalizer::visit(IR::RNotEqual* node) {
}



void TreeCanonicalizer::visit(IR::Jump* node) { 
}

void TreeCanonicalizer::visit(IR::Conditional* node) { 
}



void TreeCanonicalizer::visit(IR::Addr* node) {
}

void TreeCanonicalizer::visit(IR::AddrDispl* node) {
}

void TreeCanonicalizer::visit(IR::Call* node) { 
}

void TreeCanonicalizer::visit(IR::Return* node) { 
	cout << "MaxMuch: Ret " << endl;
}

void TreeCanonicalizer::visit(IR::Phi* visitor) { 
	cout << "No visitor available for IR::Phi in " << __FILE__  << ":" <<__LINE__ << endl; 
}


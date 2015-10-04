#include "MaximalMunch.h"

using namespace IR;

#define NOT_IMPLEMENTED 								cout << "MaxMuch: not implemented. " << __FILE__ << ":" << __LINE__ << endl;

#define checkOprAndRetType(obj, ret, opr, operand)		((std::dynamic_pointer_cast<ret>(obj->chd1()) != nullptr) && 	\
														(std::dynamic_pointer_cast<opr>(obj) != nullptr) && 			\
														(std::dynamic_pointer_cast<operand>(obj->chd2()) != nullptr))


#define checkOpr(obj, operand)							(std::dynamic_pointer_cast<operand>(obj) != nullptr)


void MaximalMunch::visit(IR::Module* module) {
	cout << "MaxMuch: module " << endl;

	for (auto& func : *(module->functions())) {
		func->accept(this);
	}
}

void MaximalMunch::visit(IR::Function* func) {
	cout << "MaxMuch: func " << endl;

	for (auto& bb : *(func->bbs())) {
		bb->accept(this);
	}
}

void MaximalMunch::visit(IR::BasicBlock* bb) {  
	cout << "MaxMuch: basicblock " << endl;

	for (auto& tree : *(bb->instructions())) {
		tree->accept(this);
	}
}

void MaximalMunch::visit(IR::ScalarCopy* node) {
	cout << "MaxMuch: scalarcopy " << endl;

	/// Just temporary
	node->chd1()->accept(this);
	node->chd2()->accept(this);
}

void MaximalMunch::visit(IR::CopyFromArray* node) {
}

void MaximalMunch::visit(IR::CopyToArray* node) {
}


void MaximalMunch::visit(IR::Register* reg) {
}

void MaximalMunch::visit(IR::Memory* mem) {
}

void MaximalMunch::visit(IR::Immediate* imm) {
}

void MaximalMunch::visit(IR::Func* func) {
}

void MaximalMunch::visit(IR::IAdd* node) {
	cout << "MaxMuch: IAdd " << endl;
}

void MaximalMunch::visit(IR::ISub* node) {
}

void MaximalMunch::visit(IR::IMul* node) {
	cout << "MaxMuch: IMul " << endl;

	if ( checkOprAndRetType(node->chd2(), IR::Register, IR::ScalarCopy, IR::Memory) && checkOprAndRetType(node->chd3(), IR::Register, IR::ScalarCopy, IR::Memory) ) {
		cout << "IMul Match 1 " << endl;
	}
	else if ( checkOprAndRetType(node->chd2(), IR::Register, IR::ScalarCopy, IR::Memory) && checkOpr(node->chd3(), IR::Immediate) ) {
		cout << "IMul Match 2 " << endl;
	}
	else if ( checkOpr(node->chd2(), IR::Immediate) && checkOprAndRetType(node->chd3(), IR::Register, IR::ScalarCopy, IR::Memory) ) {
		cout << "IMul Match 3 " << endl;
	}
	else if ( checkOpr(node->chd2(), IR::Immediate) && checkOpr(node->chd3(), IR::Immediate) ) {
		cout << "IMul Match 4 " << endl;
	}
	else if ( checkOpr(node->chd2(), IR::Register) && checkOpr(node->chd3(), IR::Register) ) {
		cout << "IMul Match 5 " << endl;
	}
	else {
		cout << "ERROR:: Unmatched IMul." << endl;
	}
}

void MaximalMunch::visit(IR::IDiv* node) {
}

void MaximalMunch::visit(IR::IMod* node) {
}

void MaximalMunch::visit(IR::IMinus* node) {
}

void MaximalMunch::visit(IR::IInc* node) {
}

void MaximalMunch::visit(IR::IDec* node) {
}





void MaximalMunch::visit(IR::FAdd* node) {
}

void MaximalMunch::visit(IR::FSub* node) {
}

void MaximalMunch::visit(IR::FMul* node) {
}

void MaximalMunch::visit(IR::FDiv* node) {
}

void MaximalMunch::visit(IR::FMinus* node) {
}

void MaximalMunch::visit(IR::FInc* node) {
}

void MaximalMunch::visit(IR::FDec* node) {
}






void MaximalMunch::visit(IR::BinAnd* node) {
}

void MaximalMunch::visit(IR::BinOr* node) {
}

void MaximalMunch::visit(IR::BinXor* node) {
}

void MaximalMunch::visit(IR::BinNot* node) {
}



void MaximalMunch::visit(IR::RLesThan* node) {
}

void MaximalMunch::visit(IR::RLesThanEqual* node) {
}

void MaximalMunch::visit(IR::RGreaterThan* node) {
}

void MaximalMunch::visit(IR::RGreaterThanEqual* node) {
}

void MaximalMunch::visit(IR::REqual* node) {
}

void MaximalMunch::visit(IR::RNotEqual* node) {
}



void MaximalMunch::visit(IR::Jump* node) { 
}

void MaximalMunch::visit(IR::Conditional* node) { 
}



void MaximalMunch::visit(IR::Addr* node) {
}

void MaximalMunch::visit(IR::AddrDispl* node) {
}

void MaximalMunch::visit(IR::Call* node) { 
}

void MaximalMunch::visit(IR::Return* node) { 
	cout << "MaxMuch: Ret " << endl;
}

void MaximalMunch::visit(IR::Phi* visitor) { 
	cout << "No codegen available for IR::Phi in " << __FILE__  << ":" <<__LINE__ << endl; 
}


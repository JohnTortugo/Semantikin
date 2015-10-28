#include "MaximalMunch.h"

using namespace IR;

#define NOT_IMPLEMENTED 							cout << "MaxMuch: not implemented. " << __FILE__ << ":" << __LINE__ << endl;

#define isMemoryLoad(obj)							((std::dynamic_pointer_cast<IR::ScalarCopy>(obj) != nullptr) && 				\
								 					(std::dynamic_pointer_cast<IR::Register>(obj->tgt()) != nullptr) && 			\
								 					(std::dynamic_pointer_cast<IR::Memory>(obj->chd2()) != nullptr))

#define isImmediateLoad(obj)						((std::dynamic_pointer_cast<IR::ScalarCopy>(obj) != nullptr) && 				\
								 					(std::dynamic_pointer_cast<IR::Register>(obj->tgt()) != nullptr) && 			\
								 					(std::dynamic_pointer_cast<IR::Immediate>(obj->chd2()) != nullptr))

#define nodeType(obj, operand)						(std::dynamic_pointer_cast<operand>(obj) != nullptr)

#define setColor(obj, color)						obj->myColor(color);						\
													obj->fillColor( this->fillColor(color) );

#define setNodeColors(obj1, obj2, obj3, color)		obj1->myColor(color);						\
													obj1->fillColor( this->fillColor(color) );	\
													obj2->myColor(color);						\
													obj2->fillColor( this->fillColor(color) );	\
													obj3->myColor(color);						\
													obj3->fillColor( this->fillColor(color) );




void MaximalMunch::visit(IR::Module* module) {
	for (auto& func : *(module->functions())) {
		func->accept(this);
	}
}

void MaximalMunch::visit(IR::Function* func) {
	for (auto& bb : *(func->bbs())) {
		bb->accept(this);
	}
}

void MaximalMunch::visit(IR::BasicBlock* bb) {  
	for (auto& tree : *(bb->subtrees())) {
		setColor(tree, 0);
		tree->accept(this);
	}
}

void MaximalMunch::visit(IR::ScalarCopy* node) {
	if (nodeType(node->tgt(), IR::Memory) && nodeType(node->chd2(), IR::Immediate)) {
		setColor(node->tgt(), node->myColor());
		setColor(node->chd2(), node->myColor());
	}
	else if (nodeType(node->tgt(), IR::Memory) && nodeType(node->chd2(), IR::Register)) {
		setColor(node->tgt(), node->myColor());
		setColor(node->chd2(), this->otherColor(node->myColor()));

		node->chd2()->accept(this);
	}
	else if (nodeType(node->tgt(), IR::Memory) && nodeType(node->chd2()->tgt(), IR::Register) ) {
		setColor(node->tgt(), node->myColor());
		setColor(node->chd2(), this->otherColor(node->myColor()));

		node->chd2()->accept(this);
	}
	else if (nodeType(node->tgt(), IR::Memory) && isImmediateLoad(node->chd2()) ) {
		setColor(node->tgt(), node->myColor());

		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());
	}
	else if (nodeType(node->tgt(), IR::Register) && nodeType(node->chd2(), IR::Register) ) {
		setColor(node->tgt(), node->myColor());
		setColor(node->chd2(), this->otherColor(node->myColor()));

		node->chd2()->accept(this);
	}
	else if (nodeType(node->tgt(), IR::Register) && nodeType(node->chd2(), IR::Immediate) ) {
		setColor(node->tgt(), node->myColor());
		setColor(node->chd2(), node->myColor());
	}
	else if (nodeType(node->tgt(), IR::Register) && nodeType(node->chd2(), IR::Memory) ) {
		setColor(node->tgt(), node->myColor());
		setColor(node->chd2(), node->myColor());
	}
	else if (nodeType(node->tgt(), IR::Register) && isMemoryLoad(node->chd2()) ) {
		setColor(node->tgt(), node->myColor());

		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());
	}
	else if (nodeType(node->tgt(), IR::Register) && isImmediateLoad(node->chd2()) ) {
		setColor(node->tgt(), node->myColor());

		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());
	}
	else if (nodeType(node->tgt(), IR::Register) && nodeType(node->chd2()->tgt(), IR::Register) ) {
		setColor(node->tgt(), node->myColor());
		setColor(node->chd2(), node->myColor());
	}
	else {
		cout << "CRITICAL: Unmatched ScalarCopy." << endl;
	}
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
	if (isMemoryLoad(node->chd2()) && isMemoryLoad(node->chd3()) ) {
		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());

		setColor(node->chd3(), this->otherColor(node->myColor()));

		node->chd3()->accept(this);
	}
	else if ( isMemoryLoad(node->chd2()) && isImmediateLoad(node->chd3()) ) {
		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());

		setColor(node->chd3(), node->myColor());
		setColor(node->chd3()->tgt(), node->myColor());
		setColor(node->chd3()->chd2(), node->myColor());
	}
	else if ( isImmediateLoad(node->chd2())	&& isMemoryLoad(node->chd3()) ) {
		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());

		setColor(node->chd3(), node->myColor());
		setColor(node->chd3()->tgt(), node->myColor());
		setColor(node->chd3()->chd2(), node->myColor());
	}
	else if ( nodeType(node->chd2()->tgt(), IR::Register) && isMemoryLoad(node->chd3()) ) {
		setColor(node->chd2(), this->otherColor(node->myColor()));

		setColor(node->chd3(), node->myColor());
		setColor(node->chd3()->tgt(), node->myColor());
		setColor(node->chd3()->chd2(), node->myColor());

		node->chd2()->accept(this);
	}
	else if ( isMemoryLoad(node->chd2()) && nodeType(node->chd3()->tgt(), IR::Register) ) {
		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());

		setColor(node->chd3(), this->otherColor(node->myColor()));

		node->chd3()->accept(this);
	}
	else if ( isImmediateLoad(node->chd2())	&& isImmediateLoad(node->chd3()) ) {
		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());

		setColor(node->chd3(), this->otherColor(node->myColor()));

		node->chd3()->accept(this);
	}
	else if ( nodeType(node->chd2()->tgt(), IR::Register) && nodeType(node->chd3()->tgt(), IR::Register) ) {
		setColor(node->chd2(), this->otherColor(node->myColor()));
		setColor(node->chd3(), this->otherColor(node->myColor()));

		node->chd2()->accept(this);
		node->chd3()->accept(this);
	}
	else if ( nodeType(node->chd2()->tgt(), IR::Register) && isImmediateLoad(node->chd3()) ) {
		setColor(node->chd2(), this->otherColor(node->myColor()));

		setColor(node->chd3(), node->myColor());
		setColor(node->chd3()->tgt(), node->myColor());
		setColor(node->chd3()->chd2(), node->myColor());

		node->chd2()->accept(this);
	}
	else if ( isImmediateLoad(node->chd2())	&& nodeType(node->chd3()->tgt(), IR::Register) ) {
		setColor(node->chd2(), node->myColor());
		setColor(node->chd2()->tgt(), node->myColor());
		setColor(node->chd2()->chd2(), node->myColor());

		setColor(node->chd3(), this->otherColor(node->myColor()));

		node->chd3()->accept(this);
	}
	else {
		cout << "ERROR:: Unmatched IMul." << endl;
	}
}

void MaximalMunch::visit(IR::ISub* node) {
}

void MaximalMunch::visit(IR::IMul* node) {
	if (isMemoryLoad(node->chd2()) && isMemoryLoad(node->chd3()) ) {
		setNodeColors(node->chd2(), node->chd2()->tgt(), node->chd2()->chd2(), this->otherColor(node->myColor()));
		setNodeColors(node->chd3(), node->chd3()->tgt(), node->chd3()->chd2(), this->otherColor(node->myColor()));

		node->chd2()->accept(this);
		node->chd3()->accept(this);
	}
	else if ( isMemoryLoad(node->chd2()) && isImmediateLoad(node->chd3()) ) {
		setNodeColors(node->chd2(), node->chd2()->tgt(), node->chd2()->chd2(), node->myColor());
		setNodeColors(node->chd3(), node->chd3()->tgt(), node->chd3()->chd2(), node->myColor());
	}
	else if ( isImmediateLoad(node->chd2())	&& isMemoryLoad(node->chd3()) ) {
		setNodeColors(node->chd2(), node->chd2()->tgt(), node->chd2()->chd2(), node->myColor());
		setNodeColors(node->chd3(), node->chd3()->tgt(), node->chd3()->chd2(), node->myColor());
	}
	else if ( nodeType(node->chd2()->tgt(), IR::Register) && isMemoryLoad(node->chd3()) ) {
		setColor(node->chd2(), this->otherColor(node->myColor()));
		setNodeColors(node->chd3(), node->chd3()->tgt(), node->chd3()->chd2(), node->myColor());

		node->chd2()->accept(this);
	}
	else if ( isMemoryLoad(node->chd2()) && nodeType(node->chd3()->tgt(), IR::Register) ) {
		setNodeColors(node->chd2(), node->chd2()->tgt(), node->chd2()->chd2(), node->myColor());
		setColor(node->chd3(), this->otherColor(node->myColor()));

		node->chd3()->accept(this);
	}
	else if ( isImmediateLoad(node->chd2())	&& isImmediateLoad(node->chd3()) ) {
		setNodeColors(node->chd2(), node->chd2()->tgt(), node->chd2()->chd2(), node->myColor());
		setNodeColors(node->chd3(), node->chd3()->tgt(), node->chd3()->chd2(), node->myColor());
	}
	else if ( nodeType(node->chd2()->tgt(), IR::Register) && nodeType(node->chd3()->tgt(), IR::Register) ) {
		setColor(node->chd2(), this->otherColor(node->myColor()));
		setColor(node->chd3(), this->otherColor(node->myColor()));

		node->chd2()->accept(this);
		node->chd3()->accept(this);
	}
	else if ( nodeType(node->chd2()->tgt(), IR::Register) && isImmediateLoad(node->chd3()) ) {
		setColor(node->chd2(), this->otherColor(node->myColor()));
		setNodeColors(node->chd3(), node->chd3()->tgt(), node->chd3()->chd2(), node->myColor());

		node->chd2()->accept(this);
	}
	else if ( isImmediateLoad(node->chd2())	&& nodeType(node->chd3()->tgt(), IR::Register) ) {
		setNodeColors(node->chd2(), node->chd2()->tgt(), node->chd2()->chd2(), this->otherColor(node->myColor()));
		setColor(node->chd3(), this->otherColor(node->myColor()));

		node->chd3()->accept(this);
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


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
    /// Generate code for each basic block
	for (auto& bb : *(func->bbs())) {
		bb->accept(this);
	}
}

void MaximalMunch::visit(IR::BasicBlock* bb) {
    /// For each subtree, which as far as I remember is equivalent to each
    /// instruction
	for (auto& tree : *(bb->subtrees())) {
		setColor(tree, 0);
		tree->accept(this);
	}
}

void MaximalMunch::visit(IR::ScalarCopy* node) {
    cout << "Calling scalar copy " << __LINE__ << endl;

	if (nodeType(node->tgt(), IR::Memory) && nodeType(node->chd2()->tgt(), IR::Register)) {
		setColor(node->tgt(), node->myColor());

		setColor(node->chd2(), this->otherColor(node->myColor()));
		node->chd2()->accept(this);
	}
    else if (nodeType(node->tgt(), IR::Register)) {
        if (nodeType(node->chd2(), IR::Immediate)) {
            setColor(node->tgt(), node->myColor());
            setColor(node->chd2(), node->myColor());
        }
        else if (nodeType(node->chd2(), IR::Register)) {
            setColor(node->tgt(), node->myColor());
            setColor(node->chd2(), node->myColor());
        }
        else if (nodeType(node->chd2(), IR::Memory)) {
            setColor(node->tgt(), node->myColor());
            setColor(node->chd2(), node->myColor());
        }
        else {
            cout << "MMunch [" << __FILE__ << ":" << __LINE__ << "] - Did not match case." << endl;
            exit(1);
        }
    }
    else {
        cout << "MMunch [" << __FILE__ << ":" << __LINE__ << "] - Did not match case." << endl;
        exit(1);
    }
}

void MaximalMunch::visit(IR::CopyFromArray* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::CopyToArray* node) {
	NOT_IMPLEMENTED
}


void MaximalMunch::visit(IR::Register* reg) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::Memory* mem) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::Immediate* imm) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::Func* func) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::IAdd* node) {

//	if (nodeType(node->chd2(), IR::Register) && nodeType(node->chd3(), IR::Register)) {
//        setColor(node->chd2(), node->myColor());
//        setColor(node->chd3(), node->myColor());
//    }
//    else {
//        cout << "MMunch [" << __FILE__ << ":" << __LINE__ << "] - Did not match case." << endl;
//        exit(1);
//    }

//	if (isMemoryLoad(node->chd2()) && isMemoryLoad(node->chd3()) ) {
//		setColor(node->chd2(), node->myColor());
//		setColor(node->chd2()->tgt(), node->myColor());
//		setColor(node->chd2()->chd2(), node->myColor());
//
//		setColor(node->chd3(), this->otherColor(node->myColor()));
//
//		node->chd3()->accept(this);
//	}
//	else if ( isMemoryLoad(node->chd2()) && isImmediateLoad(node->chd3()) ) {
//		setColor(node->chd2(), node->myColor());
//		setColor(node->chd2()->tgt(), node->myColor());
//		setColor(node->chd2()->chd2(), node->myColor());
//
//		setColor(node->chd3(), node->myColor());
//		setColor(node->chd3()->tgt(), node->myColor());
//		setColor(node->chd3()->chd2(), node->myColor());
//	}
//	else if ( isImmediateLoad(node->chd2())	&& isMemoryLoad(node->chd3()) ) {
//		setColor(node->chd2(), node->myColor());
//		setColor(node->chd2()->tgt(), node->myColor());
//		setColor(node->chd2()->chd2(), node->myColor());
//
//		setColor(node->chd3(), node->myColor());
//		setColor(node->chd3()->tgt(), node->myColor());
//		setColor(node->chd3()->chd2(), node->myColor());
//	}
//	else if ( nodeType(node->chd2()->tgt(), IR::Register) && isMemoryLoad(node->chd3()) ) {
//		setColor(node->chd2(), this->otherColor(node->myColor()));
//
//		setColor(node->chd3(), node->myColor());
//		setColor(node->chd3()->tgt(), node->myColor());
//		setColor(node->chd3()->chd2(), node->myColor());
//
//		node->chd2()->accept(this);
//	}
//	else if ( isMemoryLoad(node->chd2()) && nodeType(node->chd3()->tgt(), IR::Register) ) {
//		setColor(node->chd2(), node->myColor());
//		setColor(node->chd2()->tgt(), node->myColor());
//		setColor(node->chd2()->chd2(), node->myColor());
//
//		setColor(node->chd3(), this->otherColor(node->myColor()));
//
//		node->chd3()->accept(this);
//	}
//	else if ( isImmediateLoad(node->chd2())	&& isImmediateLoad(node->chd3()) ) {
//		setColor(node->chd2(), node->myColor());
//		setColor(node->chd2()->tgt(), node->myColor());
//		setColor(node->chd2()->chd2(), node->myColor());
//
//		setColor(node->chd3(), this->otherColor(node->myColor()));
//
//		node->chd3()->accept(this);
//	}
//	else if ( nodeType(node->chd2()->tgt(), IR::Register) && nodeType(node->chd3()->tgt(), IR::Register) ) {
//		setColor(node->chd2(), this->otherColor(node->myColor()));
//		setColor(node->chd3(), this->otherColor(node->myColor()));
//
//		node->chd2()->accept(this);
//		node->chd3()->accept(this);
//	}
//	else if ( nodeType(node->chd2()->tgt(), IR::Register) && isImmediateLoad(node->chd3()) ) {
//		setColor(node->chd2(), this->otherColor(node->myColor()));
//
//		setColor(node->chd3(), node->myColor());
//		setColor(node->chd3()->tgt(), node->myColor());
//		setColor(node->chd3()->chd2(), node->myColor());
//
//		node->chd2()->accept(this);
//	}
//	else if ( isImmediateLoad(node->chd2())	&& nodeType(node->chd3()->tgt(), IR::Register) ) {
//		setColor(node->chd2(), node->myColor());
//		setColor(node->chd2()->tgt(), node->myColor());
//		setColor(node->chd2()->chd2(), node->myColor());
//
//		setColor(node->chd3(), this->otherColor(node->myColor()));
//
//		node->chd3()->accept(this);
//	}
//	else {
//		cout << "ERROR:: Unmatched IMul." << endl;
//	}
}

void MaximalMunch::visit(IR::ISub* node) {
	NOT_IMPLEMENTED
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
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::IMod* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::IMinus* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::IInc* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::IDec* node) {
	NOT_IMPLEMENTED
}





void MaximalMunch::visit(IR::FAdd* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::FSub* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::FMul* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::FDiv* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::FMinus* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::FInc* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::FDec* node) {
	NOT_IMPLEMENTED
}






void MaximalMunch::visit(IR::BinAnd* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::BinOr* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::BinXor* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::BinNot* node) {
	NOT_IMPLEMENTED
}



void MaximalMunch::visit(IR::RLesThan* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::RLesThanEqual* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::RGreaterThan* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::RGreaterThanEqual* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::REqual* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::RNotEqual* node) {
	NOT_IMPLEMENTED
}



void MaximalMunch::visit(IR::Jump* node) { 
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::Conditional* node) { 
	NOT_IMPLEMENTED
}



void MaximalMunch::visit(IR::Addr* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::AddrDispl* node) {
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::Call* node) { 
	NOT_IMPLEMENTED
}

void MaximalMunch::visit(IR::Return* node) { 
	NOT_IMPLEMENTED
}


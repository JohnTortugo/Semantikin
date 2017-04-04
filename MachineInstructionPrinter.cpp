#include "MachineInstructionPrinter.h"
#include "MachineInstruction.h"

using namespace IR;

#define NOT_IMPLEMENTED 							cout << "MaxMuch: not implemented. " << __FILE__ << ":" << __LINE__ << endl;

#define isMemoryLoad(obj)							((std::dynamic_pointer_cast<IR::ScalarCopy>(obj) != nullptr) && 				\
								 					(std::dynamic_pointer_cast<IR::Register>(obj->tgt()) != nullptr) && 			\
								 					(std::dynamic_pointer_cast<IR::Memory>(obj->chd2()) != nullptr))

#define isImmediateLoad(obj)						((std::dynamic_pointer_cast<IR::ScalarCopy>(obj) != nullptr) && 				\
								 					(std::dynamic_pointer_cast<IR::Register>(obj->tgt()) != nullptr) && 			\
								 					(std::dynamic_pointer_cast<IR::Immediate>(obj->chd2()) != nullptr))

#define nodeType(obj, operand)						(std::dynamic_pointer_cast<operand>(obj) != nullptr)


void MachineInstructionPrinter::visit(IR::Module* module) {
	for (auto& func : *(module->functions())) {
		func->accept(this);
	}

    std::cout << this->output.str() << endl;
}

void MachineInstructionPrinter::visit(IR::Function* func) {
	for (auto& bb : *(func->bbs())) {
		bb->accept(this);
	}
}

void MachineInstructionPrinter::visit(IR::BasicBlock* bb) {
	for (auto& tree : *(bb->subtrees())) {
		tree->accept(this);
	}
}

void MachineInstructionPrinter::visit(IR::ScalarCopy* node) {
	if (nodeType(node->tgt(), IR::Memory) && nodeType(node->chd2()->tgt(), IR::Register)) {
		node->chd2()->accept(this);
	    node->macInstr()->dump(this->output);
	}
    else if (nodeType(node->tgt(), IR::Register)) {
        if (nodeType(node->chd2(), IR::Immediate)) {
        }
        else if (nodeType(node->chd2(), IR::Register)) {
        }
        else if (nodeType(node->chd2(), IR::Memory)) {
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

void MachineInstructionPrinter::visit(IR::CopyFromArray* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::CopyToArray* node) {
	NOT_IMPLEMENTED
}


void MachineInstructionPrinter::visit(IR::Register* reg) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::Memory* mem) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::Immediate* imm) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::Func* func) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::IAdd* node) {
	if (nodeType(node->chd2()->tgt(), IR::Register) && nodeType(node->chd3()->tgt(), IR::Register)) {
        node->chd2()->accept(this);
		node->chd3()->accept(this);

	    node->macInstr()->dump(this->output);
    }
    else {
        cout << "MMunch [" << __FILE__ << ":" << __LINE__ << "] - Did not match case." << endl;
        exit(1);
    }
}

void MachineInstructionPrinter::visit(IR::ISub* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::IMul* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::IDiv* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::IMod* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::IMinus* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::IInc* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::IDec* node) {
	NOT_IMPLEMENTED
}










void MachineInstructionPrinter::visit(IR::BinAnd* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::BinOr* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::BinXor* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::BinNot* node) {
	NOT_IMPLEMENTED
}



void MachineInstructionPrinter::visit(IR::RLesThan* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::RLesThanEqual* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::RGreaterThan* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::RGreaterThanEqual* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::REqual* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::RNotEqual* node) {
	NOT_IMPLEMENTED
}



void MachineInstructionPrinter::visit(IR::Jump* node) { 
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::Conditional* node) { 
	NOT_IMPLEMENTED
}



void MachineInstructionPrinter::visit(IR::Addr* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::AddrDispl* node) {
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::Call* node) { 
	NOT_IMPLEMENTED
}

void MachineInstructionPrinter::visit(IR::Return* node) { 
	NOT_IMPLEMENTED
}


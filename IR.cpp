#include "IR.h"
#include "IRVisitors.h"

#include <set>

using std::set;

namespace Util {
	enum ControlInstructionType {
		NoControlInstruction = 0,
		BranchInstruction = 1,
		CallInstruction = 2,
		ReturnInstruction = 3
	};

	unsigned char isControlInstr(Instruction_sptr instruction) {
		if (dynamic_cast<IR::BranchInstruction*>(instruction.get())) {
			return BranchInstruction;
		}
		else if (dynamic_cast<IR::Call*>(instruction.get())) {
			return CallInstruction;
		}
		else if (dynamic_cast<IR::Return*>(instruction.get())) {
			return ReturnInstruction;
		}
		else
			return NoControlInstruction;
	}

	STLabelDef_sptr branchTarget(Instruction_sptr instruction) {
//		if (auto branch = dynamic_cast<IR::Conditional*>(instruction.get())) {
//			return std::dynamic_pointer_cast<STLabelDef>( branch->src1() );
//		}
//		else if (auto branch = dynamic_cast<IR::Jump*>(instruction.get())) {
//			return std::dynamic_pointer_cast<STLabelDef>( branch->tgt() );
//		}
//		else
//			return nullptr;
	}

	bool isRegisterParam(shared_ptr<Parser::SymbolTableEntry> var) {
		auto pDecl = std::dynamic_pointer_cast<STParamDecl>(var);

		if (pDecl != nullptr)
			if (pDecl->offset() <= 5)
				return true;

		return false;
	}
}


namespace IR {
	void Module::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Function::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void BasicBlock::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void ScalarCopy::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void CopyFromArray::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void CopyToArray::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void Immediate::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Memory::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Register::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Func::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void IAdd::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void ISub::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IMul::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IDiv::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IMod::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IMinus::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IInc::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IDec::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void FAdd::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FSub::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FMul::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FDiv::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FMinus::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	//void FPlus::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FInc::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FDec::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void BinAnd::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void BinOr::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void BinXor::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void BinNot::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void RLesThan::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void RLesThanEqual::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void RGreaterThan::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void RGreaterThanEqual::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void REqual::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void RNotEqual::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void Jump::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Conditional::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void Addr::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void AddrDispl::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Call::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Return::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Phi::accept(IRTreeVisitor* visitor) { visitor->visit(this); }











	void Register::dump(stringstream& buffer) {
		buffer << "Reg;" << endl;
	}

	void Immediate::dump(stringstream& buffer) {
		buffer << "Imm;" << endl;
	}

	void Memory::dump(stringstream& buffer) {
		buffer << "Mem" << endl;
	}

	void Func::dump(stringstream& buffer) {
		buffer << "Func" << endl;
	}



	void ScalarCopy::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << ";" << endl;
	}

	void CopyFromArray::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = *" << this->_chd2->tgtDataName() << ";" << endl;
	}

	void CopyToArray::dump(stringstream& buffer) {
		buffer << "*" << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << ";" << endl;
	}



	void IAdd::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " + " << this->_chd3->tgtDataName() << ";" << endl;
	}

	void ISub::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " - " << this->_chd3->tgtDataName() << ";" << endl;
	}

	void IMul::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " * " << this->_chd3->tgtDataName() << ";" << endl;
	}

	void IDiv::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " / " << this->_chd3->tgtDataName() << ";" << endl;
	}

	void IMod::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " % " << this->_chd3->tgtDataName() << ";" << endl;
	}

	void IMinus::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = -" << this->_chd2->tgtDataName() << ";" << endl;
	}

	void IInc::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " + 1;" << endl;
	}

	void IDec::dump(stringstream& buffer) {
		buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " - 1;" << endl;
	}



	void FAdd::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " + " << this->_src2->getName() << ";" << endl;
	}

	void FSub::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " - " << this->_src2->getName() << ";" << endl;
	}

	void FMul::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " * " << this->_src2->getName() << ";" << endl;
	}

	void FDiv::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " / " << this->_src2->getName() << ";" << endl;
	}

	void FMinus::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = -" << this->_src1->getName() << ";" << endl;
	}

	void FInc::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " + 1;" << endl;
	}

	void FDec::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " - 1;" << endl;
	}



	void BinAnd::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " & " << this->_src2->getName() << ";" << endl;
	}

	void BinOr::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " | " << this->_src2->getName() << ";" << endl;
	}

	void BinXor::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " ^ " << this->_src2->getName() << ";" << endl;
	}

	void BinNot::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = ~ " << this->_src1->getName() << ";" << endl;
	}



	void RLesThan::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " < " << this->_src2->getName() << ";" << endl;
	}

	void RLesThanEqual::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " <= " << this->_src2->getName() << ";" << endl;
	}

	void RGreaterThan::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " > " << this->_src2->getName() << ";" << endl;
	}

	void RGreaterThanEqual::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " >= " << this->_src2->getName() << ";" << endl;
	}

	void REqual::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " == " << this->_src2->getName() << ";" << endl;
	}

	void RNotEqual::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " != " << this->_src2->getName() << ";" << endl;
	}



	void Jump::dump(stringstream& buffer) {
//		buffer << "goto " << this->_tgt->getName() << ";" << endl;
	}

	void Conditional::dump(stringstream& buffer) {
//		buffer << "if " << this->_tgt->getName() << " goto " << this->_src1->getName() << ";" << endl;
	}



	void Addr::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = &" << this->_src1->getName() << ";" << endl;
	}

	void AddrDispl::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " ++ " << this->_src2->getName() << ";" << endl;
	}

	void Call::dump(stringstream& buffer) {
//		if (this->_tgt != nullptr)
//			buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "(";
//		else
//			buffer << this->_src1->getName() << "(";
//
//		auto arguments = this->arguments();
//
//		if (arguments != nullptr) {
//			if (arguments->size() > 0)
//				buffer << (*arguments)[0]->tgtDataName();
//
//			for (int i=1; i<arguments->size(); i++)
//				buffer << ", " << (*arguments)[i]->tgtDataName();
//		}
//
//		buffer << ");" << endl;
	}

	void Return::dump(stringstream& buffer) {
//		buffer << "return " << this->_tgt->getName() << ";" << endl;
	}

	void Phi::dump(stringstream& buffer) { buffer << "phi();" << endl; }



	void Function::addSymbolTable(shared_ptr<SymbolTable> st) {
		for (auto entry : st->entries() ) {
			if (nameVersions.find(entry.second->getName()) != nameVersions.end()) {
				entry.second->rename( entry.second->getName() + "_" + std::to_string(nameVersions[entry.second->getName()]));
			}

			this->symbolTable()->add( entry.second );
		}
	}

	void Function::dump(stringstream& buffer) {
//		buffer << "function " << this->_addr->getName() << "(...) {" << endl;
//
//		for (auto instruction : *this->_subtrees) {
//			/* Do we have a label here?  */
//			if (instruction.first != nullptr)
//				buffer << std::setfill(' ') << std::setw(15) << instruction.first->getName() << ": ";
//			else
//				buffer << std::setfill(' ') << std::setw(17) << " ";
//
//			/* Is it just a label? */
//			if (instruction.second != nullptr)
//				instruction.second->dump(buffer);
//			else
//				buffer << endl;
//		}
//
//		buffer << "}";
	}


	void Function::appendBasicBlock(BasicBlock_sptr bb) {
		bb->id( this->_bbs->size() );
		this->_bbs->push_back(bb);
		this->_currentBasicBlock = bb;
	}

	void Function::appendLabel(shared_ptr<STLabelDef> label) { }

	void Function::appendInstruction(shared_ptr<IR::Instruction> instr) {
		this->_currentBasicBlock->appendInstruction(instr);
		
		//if (this->_labelPendingSlot) {
		//	this->_subtrees->back().second = instr;
		//	this->_labelPendingSlot = false;
		//}
		//else {
		//	this->_subtrees->push_back( make_pair(nullptr, instr) );
		//}
	}

	void Module::dump() {
	   std::stringstream buffer;

	   buffer << "This is the IR: " << endl;
	   buffer << std::setfill('-') << std::setw(80) << "-" << endl;

		for (auto function : *this->_functions) {
			function->dump(buffer);
			buffer << endl << endl;
		}

		cout << buffer.str();
	}
}

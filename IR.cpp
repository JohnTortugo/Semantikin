#include "IR.h"

namespace IR {

	void ScalarCopy::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << ";" << endl;
	}

	void CopyFromArray::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = *" << this->_src1->getName() << ";" << endl;
	}

	void CopyToArray::dump(stringstream& buffer) {
		buffer << "*" << this->_tgt->getName() << " = " << this->_src1->getName() << ";" << endl;
	}



	void IAdd::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " + " << this->_src2->getName() << ";" << endl;
	}

	void ISub::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " - " << this->_src2->getName() << ";" << endl;
	}

	void IMul::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " * " << this->_src2->getName() << ";" << endl;
	}

	void IDiv::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " / " << this->_src2->getName() << ";" << endl;
	}

	void IMod::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " % " << this->_src2->getName() << ";" << endl;
	}

	void IMinus::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = -" << this->_src1->getName() << ";" << endl;
	}

	void IPlus::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = +" << this->_src1->getName() << ";" << endl;
	}

	void IInc::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "++;" << endl;
	}

	void IDec::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "--;" << endl;
	}

	void FAdd::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " + " << this->_src2->getName() << ";" << endl;
	}

	void FSub::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " - " << this->_src2->getName() << ";" << endl;
	}

	void FMul::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " * " << this->_src2->getName() << ";" << endl;
	}

	void FDiv::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " / " << this->_src2->getName() << ";" << endl;
	}

	void FMinus::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = -" << this->_src1->getName() << ";" << endl;
	}

	void FPlus::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = +" << this->_src1->getName() << ";" << endl;
	}

	void FInc::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "++;" << endl;
	}

	void FDec::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "--;" << endl;
	}



	void BinAnd::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " & " << this->_src2->getName() << ";" << endl;
	}

	void BinOr::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " | " << this->_src2->getName() << ";" << endl;
	}

	void BinXor::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " ^ " << this->_src2->getName() << ";" << endl;
	}

	void BinNot::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = ~ " << this->_src1->getName() << ";" << endl;
	}



	void LogAnd::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " && " << this->_src2->getName() << ";" << endl;
	}

	void LogOr::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " || " << this->_src2->getName() << ";" << endl;
	}

	void LogNot::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = ! " << this->_src1->getName() << ";" << endl;
	}



	void RLesThan::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " < " << this->_src2->getName() << ";" << endl;
	}

	void RLesThanEqual::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " <= " << this->_src2->getName() << ";" << endl;
	}

	void RGreaterThan::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " > " << this->_src2->getName() << ";" << endl;
	}

	void RGreaterThanEqual::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " >= " << this->_src2->getName() << ";" << endl;
	}

	void REqual::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " == " << this->_src2->getName() << ";" << endl;
	}

	void RNotEqual::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " != " << this->_src2->getName() << ";" << endl;
	}



	void Jump::dump(stringstream& buffer) {
		buffer << "goto " << this->_tgt->getName() << ";" << endl;
	}

	void CondTrueJump::dump(stringstream& buffer) {
		buffer << "ifTrue " << this->_tgt->getName() << " goto " << this->_src1->getName() << ";" << endl;
	}

	void CondFalseJump::dump(stringstream& buffer) {
		buffer << "ifFalse " << this->_tgt->getName() << " goto " << this->_src1->getName() << ";" << endl;
	}



	void Addr::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = &" << this->_src1->getName() << ";" << endl;
	}

	void Call::dump(stringstream& buffer) {
		if (this->_tgt != nullptr)
			buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "(";
		else
			buffer << this->_src1->getName() << "(";

		auto arguments = this->arguments();

		if (arguments->size() > 0)
			buffer << (*arguments)[0]->getName();

		for (int i=1; i<arguments->size(); i++)
			buffer << ", " << (*arguments)[i]->getName();

		buffer << ");" << endl;
	}

	void Return::dump(stringstream& buffer) {
		buffer << "return " << this->_tgt->getName() << ";" << endl;
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
		buffer << "function " << this->_addr->getName() << "(...) {" << endl;

		for (auto instruction : *this->_instrs) {
			/* Do we have a label here?  */
			if (instruction.first != nullptr)
				buffer << std::setfill(' ') << std::setw(15) << instruction.first->getName() << ": ";
			else
				buffer << std::setfill(' ') << std::setw(17) << " ";

			/* Is it just a label? */
			if (instruction.second != nullptr)
				instruction.second->dump(buffer);
			else
				buffer << endl;
		}

		buffer << "}";
	}



	void Module::dump() {
	   std::stringstream buffer;

	   buffer << "This is the IR: " << endl;
	   buffer << std::setfill('-') << std::setw(80) << "-" << endl;

		for (auto function : *this->functions) {
			function->dump(buffer);
			buffer << endl << endl;
		}

		cout << buffer.str();
	}

	void Function::appendLabel(shared_ptr<STLabelDef> label) {
		if (this->_labelPendingSlot) {
			this->_instrs->back().second = nullptr;
//			cout << endl << "IR-TAC-Gen:" << endl;
//			cout << "\tAppending label but there is already one at this position." << endl;
//			cout << "New would be: \t\t \"" << label->getName() << "\"" << endl;
//			exit(1);
		}

		/* Set the "address" to which the label points to. */
		label->address((int)this->_instrs->size());

		this->_instrs->push_back( make_pair(label, nullptr) );
		this->_labelPendingSlot = true;

//		cout << label->getName() << ": ";
	}

	void Function::appendInstruction(shared_ptr<IR::Instruction> instr) {
		if (this->_labelPendingSlot) {
			this->_instrs->back().second = instr;
			this->_labelPendingSlot = false;
		}
		else {
			this->_instrs->push_back( make_pair(nullptr, instr) );
		}

//		stringstream ss;
//		instr->dump(ss);
//		cout << ss.str();
	}
}

#include "IR.h"

namespace IR {

	void ScalarCopy::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << ";" << endl;
	}

	void CopyFromArray::dump(stringstream& buffer) { buffer << "? = ?[?];" << endl; }

	void CopyToArray::dump(stringstream& buffer) { buffer << "?[?] = ?;" << endl; }



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

	void FAdd::dump(stringstream& buffer) { buffer << "? = ? + ?;" << endl; }

	void FSub::dump(stringstream& buffer) { buffer << "? = ? - ?;" << endl; }

	void FMul::dump(stringstream& buffer) { buffer << "? = ? * ?;" << endl; }

	void FDiv::dump(stringstream& buffer) { buffer << "? = ? / ?;" << endl; }


	void BinAnd::dump(stringstream& buffer) { buffer << "? = ? & ?;" << endl; }

	void BinOr::dump(stringstream& buffer) { buffer << "? = ? | ?;" << endl; }

	void BinXor::dump(stringstream& buffer) { buffer << "? = ? ^ ?;" << endl; }

	void BinNot::dump(stringstream& buffer) { buffer << "? = !?;" << endl; }


	void LogAnd::dump(stringstream& buffer) { buffer << "? = ? && ?;" << endl; }

	void LogOr::dump(stringstream& buffer) { buffer << "? = ? || ?;" << endl; }

	void LogNot::dump(stringstream& buffer) { buffer << "? = !?;" << endl; }


	void RLesThan::dump(stringstream& buffer) { buffer << "? = ? < ?;" << endl; }

	void RLesThanEqual::dump(stringstream& buffer) { buffer << "? = ? <= ?;" << endl; }

	void RGreaterThan::dump(stringstream& buffer) { buffer << "? = ? > ?;" << endl; }

	void RGreaterThanEqual::dump(stringstream& buffer) { buffer << "? = ? >= ?;" << endl; }

	void REqual::dump(stringstream& buffer) { buffer << "? = ? == ?;" << endl; }

	void RNotEqual::dump(stringstream& buffer) { buffer << "? = ? != ?;" << endl; }



	void Jump::dump(stringstream& buffer) { buffer << "goto ?;" << endl; }

	void CondTrueJump::dump(stringstream& buffer) { buffer << "ifTrue ? goto ?;" << endl; }

	void CondFalseJump::dump(stringstream& buffer) { buffer << "ifFalse ? goto ?;" << endl; }



	void Call::dump(stringstream& buffer) { buffer << "call(?, ?, ?);" << endl; }

	void Return::dump(stringstream& buffer) { buffer << "return ?;" << endl; }

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
		buffer << this->_addr->getName() << ":" << endl;

		for (auto instruction : *this->_instrs) {
			buffer << "\t";
			instruction->dump(buffer);
		}
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
}

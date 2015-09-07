#include "Semantikin.h"
#include "SymbolTable.h"
#include <sstream>
#include <iomanip>

namespace Parser {

	shared_ptr<SymbolTableEntry> SymbolTable::lookup(string name) {
		if (this->_entries.find(name) == this->_entries.end()) {
			if (this->getParent() != nullptr)
				return this->getParent()->lookup(name);
			else
				return nullptr;
		}
		else {
			return this->_entries.find(name)->second;
		}
	}

	bool SymbolTable::add(shared_ptr<SymbolTableEntry> newEntry) {
		if (this->_entries.find(newEntry->getName()) == this->_entries.end()) {
			this->_entries[newEntry->getName()] = newEntry;
			return true;
		}
		else {
			return false;
		}
	}

	void STFunctionDeclaration::dump(std::stringstream& buffer) const {
		buffer.fill(' ');
		buffer << std::left;
		buffer << std::setw(10) << "| Function" 					<< " |";
		buffer << std::right;
		buffer << std::setw(10) << this->name 					<< " |";
		buffer << std::setw(5) << "-" 							<< " |";
		buffer << std::setw(5) << "-" 							<< " |";
		buffer << std::setw(5) << "-" 							<< " |";
		buffer << std::setw(5) << "-" 							<< " |";
		buffer << std::setw(10) << this->label 					<< " |";
		buffer << std::setw(5)  << Util::typeName(this->type())	<< " |";
		buffer << std::setw(5);

		std::stringstream buffer2;
		buffer << "{";
		for (auto _param : this->params()) {
			STParamDecl* param = dynamic_cast<STParamDecl*>(_param.get());

			buffer2 << "(" << std::setw(1) << Util::typeName(param->type());
			buffer2 << "," << std::setw(1) << param->getNumDims() << ")";
		}
		buffer2 << "}";

		buffer << std::setw(40) << buffer2.str() << " |";
		buffer << std::endl;
	}

	void STLocalVarDecl::dump(std::stringstream& buffer) const {
		buffer.fill(' ');
		buffer << std::left;
		buffer << std::setw(10) << "| LocVar" << " |";
		buffer << std::right;
		buffer << std::setw(10) << this->name << " |";
		buffer << std::setw(5) << Util::typeName(this->_type) << " |";
		buffer << std::setw(5) << this->_width << " |";
		buffer << std::setw(5) << this->_offset << " |";
		buffer << std::setw(5) << this->getNumDims() << " |";
		buffer << std::setw(10) << "-" << " |";
		buffer << std::setw(5)  << "-" << " |";
		buffer << std::setw(40)  << "-" << " |";
		buffer << endl;
	}

	void STParamDecl::dump(std::stringstream& buffer) const {
		buffer.fill(' ');
		buffer << std::left;
		buffer << std::setw(10) << "| Param" << " |";
		buffer << std::right;
		buffer << std::setw(10) << this->name << " |";
		buffer << std::setw(5) << Util::typeName(this->_type) << " |";
		buffer << std::setw(5) << this->_width << " |";
		buffer << std::setw(5) << this->_offset << " |";
		buffer << std::setw(5) << this->getNumDims() << " |";
		buffer << std::setw(10) << "-" << " |";
		buffer << std::setw(5)  << "-" << " |";
		buffer << std::setw(40)  << "-" << " |";
		buffer << endl;
	}

	void STLabelDef::dump(std::stringstream& buffer) const {
		buffer.fill(' ');
		buffer << std::left;
		buffer << std::setw(10) << "| Label" << " |";
		buffer << std::right;
		buffer << std::setw(10) << this->name << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(10) << "-" << " |";
		buffer << std::setw(5)  << "-" << " |";
		buffer << std::setw(40)  << "-" << " |";
		buffer << endl;
	}

	void STConstantDef::dump(std::stringstream& buffer) const {
		buffer.fill(' ');
		buffer << std::left;
		buffer << std::setw(10) << "| Const" << " |";
		buffer << std::right;
		buffer << std::setw(10) << this->name << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(10) << "-" << " |";
		buffer << std::setw(5)  << "-" << " |";
		buffer << std::setw(40)  << "-" << " |";
		buffer << endl;
	}

	void STRegister::dump(std::stringstream& buffer) const {
		buffer.fill(' ');
		buffer << std::left;
		buffer << std::setw(10) << "| Temp" << " |";
		buffer << std::right;
		buffer << std::setw(10) << this->name << " |";
		buffer << std::setw(5) << Util::typeName(this->_type) << " |";
		buffer << std::setw(5) << this->_width << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(5) << "-" << " |";
		buffer << std::setw(10) << "-" << " |";
		buffer << std::setw(5)  << "-" << " |";
		buffer << std::setw(40)  << "-" << " |";
		buffer << endl;
	}

	void SymbolTable::dump(std::stringstream& buffer) const {
		map<string, shared_ptr<SymbolTableEntry>>::const_iterator it = this->_entries.begin();

		buffer.fill(' ');
		buffer << "+---------------------------------------------------------------------------------------------------------------+" << endl;
		buffer << std::left;
		buffer << std::setw(10) << "| EntType" 	<< " |";
		buffer << std::right;
		buffer << std::setw(10) << "Name" 		<< " |";
		buffer << std::setw(5) << "VType" 		<< " |";
		buffer << std::setw(5) << "Width" 		<< " |";
		buffer << std::setw(5) << "Offst" 	 	<< " |";
		buffer << std::setw(5) << "NDims" 	 	<< " |";
		buffer << std::setw(10) << "Label" 	 	<< " |";
		buffer << std::setw(5)  << "RetTy" 	 	<< " |";
		buffer << std::setw(40) << "Parameters" 	<< " |";
		buffer << endl;
		buffer << "+---------------------------------------------------------------------------------------------------------------+" << endl;

		while (it != this->_entries.end()) {
			(*it).second->dump(buffer);
			it++;
		}

		buffer << "+---------------------------------------------------------------------------------------------------------------+" << endl;
	}

	int SymbolTable::stackFrameSize() const {
		int frameSize = 0;

		for (auto& entry : _entries) {
			auto var = std::dynamic_pointer_cast<STLocalVarDecl>(entry.second);
			auto tmp = std::dynamic_pointer_cast<STRegister>(entry.second);

			if (var != nullptr) {
				frameSize += var->width();
			}
			else if (tmp != nullptr) {
				frameSize += tmp->width();
			}
		}

		return frameSize;
	}
}

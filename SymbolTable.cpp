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

	void STFunctionDeclaration::dump() const {
		cout << std::left;
		cout << std::setw(10) << "| Function" 		<< " |";
		cout << std::right;
		cout << std::setw(10) << this->name 		<< " |";
		cout << std::setw(5) << "-" 				<< " |";
		cout << std::setw(5) << "-" 				<< " |";
		cout << std::setw(5) << "-" 				<< " |";
		cout << std::setw(5) << "-" 				<< " |";
		cout << std::setw(10) << this->label 		<< " |";
		cout << std::setw(5)  << this->returnType 	<< " |";
		cout << std::setw(5);

		vector<pair<NativeType, int>>::const_iterator it = this->params.begin();
		std::stringstream buffer;
		buffer << "{";
		while (it != this->params.end()) {
			buffer << "(" << std::setw(1) << it->first;
			buffer << "," << std::setw(1) << it->second << ")";

			it++;
		}
		buffer << "}";

		cout << std::setw(40) << buffer.str() << " |";
		cout << endl;
	}

	void STLocalVarDecl::dump() const {
		cout << std::left;
		cout << std::setw(10) << "| LocVar" << " |";
		cout << std::right;
		cout << std::setw(10) << this->name << " |";
		cout << std::setw(5) << this->type << " |";
		cout << std::setw(5) << this->width << " |";
		cout << std::setw(5) << this->offset << " |";
		cout << std::setw(5) << this->numDims << " |";
		cout << std::setw(10) << "-" << " |";
		cout << std::setw(5)  << "-" << " |";
		cout << std::setw(40)  << "-" << " |";
		cout << endl;
	}

	void STParamDecl::dump() const {
		cout << std::left;
		cout << std::setw(10) << "| Param" << " |";
		cout << std::right;
		cout << std::setw(10) << this->name << " |";
		cout << std::setw(5) << this->type << " |";
		cout << std::setw(5) << this->width << " |";
		cout << std::setw(5) << this->offset << " |";
		cout << std::setw(5) << this->numDims << " |";
		cout << std::setw(10) << "-" << " |";
		cout << std::setw(5)  << "-" << " |";
		cout << std::setw(40)  << "-" << " |";
		cout << endl;
	}

	void SymbolTable::dump() const {
		if (this->getParent() != nullptr) {
			this->getParent()->dump();
		}

		map<string, shared_ptr<SymbolTableEntry>>::const_iterator it = this->_entries.begin();

		cout << "+---------------------------------------------------------------------------------------------------------------+" << endl;
		cout << std::left;
		cout << std::setw(10) << "| EntType" 	<< " |";
		cout << std::right;
		cout << std::setw(10) << "Name" 		<< " |";
		cout << std::setw(5) << "VType" 		<< " |";
		cout << std::setw(5) << "Width" 		<< " |";
		cout << std::setw(5) << "Offst" 	 	<< " |";
		cout << std::setw(5) << "NDims" 	 	<< " |";
		cout << std::setw(10) << "Label" 	 	<< " |";
		cout << std::setw(5)  << "RetTy" 	 	<< " |";
		cout << std::setw(40) << "Parameters" 	<< " |";
		cout << endl;
		cout << "+---------------------------------------------------------------------------------------------------------------+" << endl;

		while (it != this->_entries.end()) {
			(*it).second->dump();
			it++;
		}

		cout << "+---------------------------------------------------------------------------------------------------------------+" << endl;
	}
}

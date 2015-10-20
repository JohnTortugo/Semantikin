#include "IR.h"
#include "IRVisitors.h"

#include <set>

namespace IR {

	BasicBlock_list_sptr Function::topologicalSort() {
		auto result = make_shared<BasicBlock_list>();
		auto auxi = make_shared<BasicBlock_list>();
		std::set<BasicBlock_sptr> visited;

		auxi->push_back( this->_entryBasicBlock );
		result->push_back( this->_entryBasicBlock );
		visited.insert( this->_entryBasicBlock );

		while (!auxi->empty()) {
			auto curr = auxi->front(); auxi->pop_front();

			for (auto& succ : *curr->succs()) {
				if (visited.find(succ) != visited.end()) continue;

				auxi->push_back( succ );
				result->push_back( succ );
				visited.insert( succ );
			}
		}

		return result; 
	}

	void Function::addSymbolTable(shared_ptr<SymbolTable> st) {
		for (auto entry : st->entries() ) {
			if (nameVersions.find(entry.second->getName()) != nameVersions.end()) {
				entry.second->rename( entry.second->getName() + "_" + std::to_string(nameVersions[entry.second->getName()]));
			}

			this->symbolTable()->add( entry.second );
		}
	}

	void Function::appendBasicBlock(BasicBlock_sptr bb) {
		bb->id( this->_bbs->size() );
		this->_bbs->push_back(bb);
		this->_currentBasicBlock = bb;
	}

	void Function::appendInstruction(shared_ptr<IR::Instruction> instr) {
		this->_currentBasicBlock->appendInstruction(instr);
	}
}

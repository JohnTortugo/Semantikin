#include "DataFlowAnalysis.h"

#include <iostream>

namespace DFA {

ReachingDefinitions::ReachingDefinitions(BasicBlock_list_sptr bbs) : _bbs(bbs) {
	for (auto& bb : *_bbs) {
		for (auto& instr : bb->instructions()) {
			if ( IR::isADefinition(instr.shared_from_this()) ) {
				auto tgt = IR::whatIsDefined(instr.shared_from_this());

				if (_defs.find(tgt) == _defs.end())
					_defs[tgt] = make_shared<Instruction_list>();

				this->_defs[tgt]->push_back(instr.shared_from_this());
			}
		}
	}
}

void ReachingDefinitions::execute() {
	std::cout << "Going to execute RD. There are " << _defs.size() << " defs. " << std::endl;
}

}

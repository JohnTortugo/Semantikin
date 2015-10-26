#include "DataFlowAnalysis.h"

#include <iostream>

namespace DFA {

	LiveVariables::LiveVariables(BasicBlock_list_sptr bbs) : _bbs(bbs) {
		auto counter = 1;

		// Compute Use[i] and Def[i] sets. Where 'i' is an instruction and
		// Use[i]/Def[i] contains the set of all variables used/defined by that instruction.
		for (auto& bb : *_bbs) {
			for (auto& ins : bb->instructions()) {
				auto instr = ins.shared_from_this();

				_defs[instr] = make_shared<Instruction_set>();

				if ( IR::isADefinition(instr) ) {
					this->_defs[instr]->insert( IR::whatIsDefined(instr) );
				}


			}
		}


	}

}

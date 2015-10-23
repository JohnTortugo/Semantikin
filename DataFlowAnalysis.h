#ifndef __DFA__
#define __DFA__ 1

#include "Semantikin.h"
#include "IR.h"

namespace DFA {

class ReachingDefinitions {
	private:
		BasicBlock_list_sptr _bbs;

		std::map<SymbolTableEntry_sp, Instruction_list_sptr> _defs;
		std::map<Instruction_sptr, Instruction_list_sptr> _gen;
		std::map<Instruction_sptr, Instruction_list_sptr> _kill;

	public:
		ReachingDefinitions(BasicBlock_list_sptr bbs);

		void execute();
};

}

#endif

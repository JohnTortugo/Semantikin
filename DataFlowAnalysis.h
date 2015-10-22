#ifndef __DFA__
#define __DFA__ 1

#include "Semantikin.h"

namespace DFA {

class ReachingDefinitions {
	private:
		BasicBlock_list_sptr _bbs;
		std::map<SymbolTableEntry_sp, Instruction_list_sptr> _defs;

	public:
		ReachingDefinitions(BasicBlock_list_sptr bbs);

		void execute();
};

}

#endif

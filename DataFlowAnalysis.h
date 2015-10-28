#ifndef __DFA__
#define __DFA__ 1

#include "Semantikin.h"
#include "IR.h"

namespace DFA {

class ReachingDefinitions {
	private:
		BasicBlock_list_sptr _bbs;

		std::map<Instruction_sptr, unsigned int> _identifier;

		std::map<STEntry_sptr, Instruction_set_sptr> _defs;
		std::map<Instruction_sptr, Instruction_set_sptr> _gen;
		std::map<Instruction_sptr, Instruction_set_sptr> _kill;

		std::map<Instruction_sptr, Instruction_set_sptr> _inInstr;
		std::map<Instruction_sptr, Instruction_set_sptr> _outInstr;

		std::map<BasicBlock_sptr, Instruction_set_sptr> _inBb;
		std::map<BasicBlock_sptr, Instruction_set_sptr> _outBb;


	public:
		ReachingDefinitions(BasicBlock_list_sptr bbs);

		void execute();

		void dump();
};


class LiveVariables {
	private:
		BasicBlock_list_sptr _bbs;

		std::map<Instruction_sptr, unsigned int> _identifier;

		std::map<Instruction_sptr, STEntry_set_sptr> _defs;
		std::map<Instruction_sptr, STEntry_set_sptr> _uses;

		std::map<Instruction_sptr, STEntry_set_sptr> _inInstr;
		std::map<Instruction_sptr, STEntry_set_sptr> _outInstr;

		std::map<BasicBlock_sptr, STEntry_set_sptr> _inBb;
		std::map<BasicBlock_sptr, STEntry_set_sptr> _outBb;


	public:
		LiveVariables(BasicBlock_list_sptr bbs);

		void execute();

		void dump();
};


}

#endif

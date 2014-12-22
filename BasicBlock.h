#ifndef BASICBLOCK_H_
#define BASICBLOCK_H_

#include "Semantikin.h"
#include "SymbolTable.h"
#include "IR.h"
#include <list>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;

namespace Backend {

	class BasicBlock {
	private:
		int _id;
		IRLine_list_sptr _instructions;
		BasicBlock_sptr _jmpLabel;
		BasicBlock_sptr _fallLabel;
		BasicBlock_sptr_list _predecessors;

	public:
		BasicBlock(int id, IRLine_list_sptr instrs) :
			_id(id),
			_instructions(instrs),
			_jmpLabel(nullptr),
			_fallLabel(nullptr)
		{ }

		IRLine_list_sptr instructions() const { return _instructions; }

		BasicBlock_sptr jmpLabel() const { return _jmpLabel; }
		void jmpLabel(BasicBlock_sptr label) { _jmpLabel = label; }

		BasicBlock_sptr jmpFall() const { return _fallLabel; }
		void jmpFall(BasicBlock_sptr label) { _fallLabel = label; }

		void addPredecessor(BasicBlock_sptr newPred) { _predecessors.push_back(newPred); }

		void remPredecessor(BasicBlock_sptr oldPred) {
			auto predPos = find(_predecessors.begin(), _predecessors.end(), oldPred);

			if (predPos != _predecessors.end())
				_predecessors.erase(predPos);
		}


	};

}

#endif /* BASICBLOCK_H_ */

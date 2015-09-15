#ifndef BASICBLOCK_H_
#define BASICBLOCK_H_

#include "Semantikin.h"
#include "SymbolTable.h"
#include <list>
#include <iostream>
#include <sstream>
#include <memory>

using namespace std;

class IRTreeVisitor;

namespace IR {

	class Instruction;

	class BasicBlock {
	private:
		int _id;
		int _usageCounter;

		Instruction_list_sptr _instructions;

		BasicBlock_list_sptr _preds;
		BasicBlock_list_sptr _succs;

	public:
		BasicBlock(int id) :
			_id(id),
			_usageCounter(0),
			_instructions(nullptr),
			_preds(nullptr),
			_succs(nullptr)
		{ 
			cout << "creating a basic block." << endl;
			this->_instructions =  make_shared< list<shared_ptr<IR::Instruction>> >();
		
		}

		int id() const { return _id; }
		void id(int id) { _id = id; }

		int& usageCounter() { return _usageCounter; }
		void usageCounter(int uc) { _usageCounter = uc; }

		Instruction_list_sptr instructions() const { return _instructions; }

		void appendInstruction(Instruction_sptr instr) {
			this->_instructions->push_back( instr );
		}

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dumpToDot(stringstream& buffer);
	};

}

#endif /* BASICBLOCK_H_ */

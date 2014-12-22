#include "IR.h"
#include "BasicBlock.h"

#include <set>

using std::set;

namespace Util {
	enum ControlInstructionType {
		NoControlInstruction = 0,
		BranchInstruction = 1,
		CallInstruction = 2,
		ReturnInstruction = 3
	};

	unsigned char isControlInstr(Instruction_sptr instruction) {
		if (dynamic_cast<IR::BranchInstruction*>(instruction.get())) {
			return BranchInstruction;
		}
		else if (dynamic_cast<IR::Call*>(instruction.get())) {
			return CallInstruction;
		}
		else if (dynamic_cast<IR::Return*>(instruction.get())) {
			return ReturnInstruction;
		}
		else
			return NoControlInstruction;
	}

	STLabelDef_sptr branchTarget(Instruction_sptr instruction) {
		if (auto branch = dynamic_cast<IR::CondTrueJump*>(instruction.get())) {
			return std::dynamic_pointer_cast<STLabelDef>( branch->tgt() );
		}
		else if (auto branch = dynamic_cast<IR::CondFalseJump*>(instruction.get())) {
			return std::dynamic_pointer_cast<STLabelDef>( branch->src1() );
		}
		else if (auto branch = dynamic_cast<IR::Jump*>(instruction.get())) {
			return std::dynamic_pointer_cast<STLabelDef>( branch->src1() );
		}
		else
			return nullptr;
	}
}


namespace IR {

	void ScalarCopy::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << ";" << endl;
	}

	void CopyFromArray::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = *" << this->_src1->getName() << ";" << endl;
	}

	void CopyToArray::dump(stringstream& buffer) {
		buffer << "*" << this->_tgt->getName() << " = " << this->_src1->getName() << ";" << endl;
	}



	void IAdd::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " + " << this->_src2->getName() << ";" << endl;
	}

	void ISub::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " - " << this->_src2->getName() << ";" << endl;
	}

	void IMul::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " * " << this->_src2->getName() << ";" << endl;
	}

	void IDiv::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " / " << this->_src2->getName() << ";" << endl;
	}

	void IMod::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " % " << this->_src2->getName() << ";" << endl;
	}

	void IMinus::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = -" << this->_src1->getName() << ";" << endl;
	}

	void IPlus::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = +" << this->_src1->getName() << ";" << endl;
	}

	void IInc::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "++;" << endl;
	}

	void IDec::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "--;" << endl;
	}

	void FAdd::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " + " << this->_src2->getName() << ";" << endl;
	}

	void FSub::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " - " << this->_src2->getName() << ";" << endl;
	}

	void FMul::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " * " << this->_src2->getName() << ";" << endl;
	}

	void FDiv::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " / " << this->_src2->getName() << ";" << endl;
	}

	void FMinus::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = -" << this->_src1->getName() << ";" << endl;
	}

	void FPlus::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = +" << this->_src1->getName() << ";" << endl;
	}

	void FInc::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "++;" << endl;
	}

	void FDec::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "--;" << endl;
	}



	void BinAnd::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " & " << this->_src2->getName() << ";" << endl;
	}

	void BinOr::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " | " << this->_src2->getName() << ";" << endl;
	}

	void BinXor::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " ^ " << this->_src2->getName() << ";" << endl;
	}

	void BinNot::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = ~ " << this->_src1->getName() << ";" << endl;
	}



	void LogAnd::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " && " << this->_src2->getName() << ";" << endl;
	}

	void LogOr::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " || " << this->_src2->getName() << ";" << endl;
	}

	void LogNot::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = ! " << this->_src1->getName() << ";" << endl;
	}



	void RLesThan::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " < " << this->_src2->getName() << ";" << endl;
	}

	void RLesThanEqual::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " <= " << this->_src2->getName() << ";" << endl;
	}

	void RGreaterThan::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " > " << this->_src2->getName() << ";" << endl;
	}

	void RGreaterThanEqual::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " >= " << this->_src2->getName() << ";" << endl;
	}

	void REqual::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " == " << this->_src2->getName() << ";" << endl;
	}

	void RNotEqual::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " != " << this->_src2->getName() << ";" << endl;
	}



	void Jump::dump(stringstream& buffer) {
		buffer << "goto " << this->_tgt->getName() << ";" << endl;
	}

	void CondTrueJump::dump(stringstream& buffer) {
		buffer << "ifTrue " << this->_tgt->getName() << " goto " << this->_src1->getName() << ";" << endl;
	}

	void CondFalseJump::dump(stringstream& buffer) {
		buffer << "ifFalse " << this->_tgt->getName() << " goto " << this->_src1->getName() << ";" << endl;
	}



	void Addr::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = &" << this->_src1->getName() << ";" << endl;
	}

	void Call::dump(stringstream& buffer) {
		if (this->_tgt != nullptr)
			buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "(";
		else
			buffer << this->_src1->getName() << "(";

		auto arguments = this->arguments();

		if (arguments->size() > 0)
			buffer << (*arguments)[0]->getName();

		for (int i=1; i<arguments->size(); i++)
			buffer << ", " << (*arguments)[i]->getName();

		buffer << ");" << endl;
	}

	void Return::dump(stringstream& buffer) {
		buffer << "return " << this->_tgt->getName() << ";" << endl;
	}

	void Phi::dump(stringstream& buffer) { buffer << "phi();" << endl; }



	void Function::addSymbolTable(shared_ptr<SymbolTable> st) {
		for (auto entry : st->entries() ) {
			if (nameVersions.find(entry.second->getName()) != nameVersions.end()) {
				entry.second->rename( entry.second->getName() + "_" + std::to_string(nameVersions[entry.second->getName()]));
			}

			this->symbolTable()->add( entry.second );
		}
	}

	void Function::dump(stringstream& buffer) {
		buffer << "function " << this->_addr->getName() << "(...) {" << endl;

		for (auto instruction : *this->_instrs) {
			/* Do we have a label here?  */
			if (instruction.first != nullptr)
				buffer << std::setfill(' ') << std::setw(15) << instruction.first->getName() << ": ";
			else
				buffer << std::setfill(' ') << std::setw(17) << " ";

			/* Is it just a label? */
			if (instruction.second != nullptr)
				instruction.second->dump(buffer);
			else
				buffer << endl;
		}

		buffer << "}";
	}


	void Function::appendLabel(shared_ptr<STLabelDef> label) {
		if (this->_labelPendingSlot) {
			this->_instrs->back().second = nullptr;
		}

		/* Set the "address" to which the label points to. */
		label->address((int)this->_instrs->size());

		this->_instrs->push_back( make_pair(label, nullptr) );
		this->_labelPendingSlot = true;
	}

	void Function::appendInstruction(shared_ptr<IR::Instruction> instr) {
		if (this->_labelPendingSlot) {
			this->_instrs->back().second = instr;
			this->_labelPendingSlot = false;
		}
		else {
			this->_instrs->push_back( make_pair(nullptr, instr) );
		}

//		stringstream ss;
//		instr->dump(ss);
//		cout << ss.str();
	}

	ControlFlowGraph_sptr Function::cfg() {
		/* Index of leader/ender's instructions of each basic block.. */
		vector<pair<int,int>> bbLimits;

		/* Which instructions are leaders? */
		set<int> leaders;

		/* Map from instruction label to bb-id. */
		map<STLabelDef_sptr, BasicBlock_sptr> labelToBB;

		/* The CFG we are building */
		auto newCfg = make_shared<Backend::ControlFlowGraph>();

		/* If the current instruction is a control handling instruction
		 * then the next one is a leader for the fallthrought case. */
		bool nextIsLeader = false;

		/* Identifiers for basic blocks. */
		auto bbId = 1;

		/* Find the leaders */
		unsigned int instrInd = 0;
		for (auto& irLine : *this->_instrs) {
			if (irLine.first != nullptr || nextIsLeader) {
				/* Save index of where the BB starts. */
				bbLimits.push_back(make_pair(instrInd, 0));

				/* We need to know later which instructions aren't leaders. */
				leaders.insert(instrInd);
			}

			nextIsLeader = Util::isControlInstr(irLine.second);

			instrInd++;
		}

		/* Find the "enders". */
		for (auto& ldrIndx : bbLimits) {
			instrInd = ldrIndx.first + 1; // next instruction after the leader (i.e., first in the BB)

			// Find the end of the BB
			while (instrInd < this->_instrs->size() && leaders.find(instrInd) == leaders.end())
				instrInd++;

			// second points to one after end
			ldrIndx.second = instrInd;
		}

		/* Construct the nodes of the CFG. */
		BasicBlock_sptr prevBB = nullptr;
		for (auto& bbBorder : bbLimits) {
			auto beg = this->_instrs->begin();
			auto end = this->_instrs->begin();

			std::advance(beg, bbBorder.first);
			std::advance(end, bbBorder.second);

			auto newBB = make_shared<Backend::BasicBlock>(bbId++, make_shared<list<IRLine>>( list<IRLine>(beg, end) ));

			/* If the current basic block header may be a target of a jump (i.e.,
			 * the first instruction have a label, then we need to keep that
			 * information for later we need to add these edges to the CFG. */
			if (beg->first != nullptr)
				labelToBB[beg->first] = newBB;

			/* We check if the previous basic block may fallthrought to the
			 * current basic block. */
			if (prevBB != nullptr && prevBB->instructions()->size() > 0) {
				auto prevLastInstr 		= prevBB->instructions()->back();
				auto typePrevLastInstr 	= Util::isControlInstr(prevLastInstr.second);

				/* Does the last instruction of the previous BB have a fallthrought?
				 * Only when it is a return instruction that it does not. */
				if (typePrevLastInstr != Util::ReturnInstruction)
					prevBB->jmpFall(newBB);
			}

			/* Add BB to CFG. */
			newCfg->addBasicBlock(newBB);
			
			/* Update the previous BB. */
			prevBB = newBB;
		}

		/* Every function has exactly one exit basic block. */
		auto exitBasicBlock = make_shared<Backend::BasicBlock>(bbId++, make_shared<list<IRLine>>( list<IRLine>() ));

		/* jumping/branching edges. Fallthrought edges have already
		 * been added. */
		for (auto& bb : newCfg->nodes()) {
			/* If the block have instructions. */
			if (bb->instructions() != nullptr && bb->instructions()->size() > 0) {
				auto lastInstr = bb->instructions()->back().second;
				auto typeLastInstr = Util::isControlInstr(lastInstr);

				if (typeLastInstr == Util::ReturnInstruction)
					bb->jmpLabel(exitBasicBlock);
				else if (typeLastInstr == Util::BranchInstruction)
					bb->jmpLabel( labelToBB[ Util::branchTarget(lastInstr) ] );
			}
		}

		/* Add BB to CFG. */
		newCfg->addBasicBlock(exitBasicBlock);

		return newCfg;
	}


	void Module::dump() {
	   std::stringstream buffer;

	   buffer << "This is the IR: " << endl;
	   buffer << std::setfill('-') << std::setw(80) << "-" << endl;

		for (auto function : *this->_functions) {
			function->dump(buffer);
			buffer << endl << endl;
		}

		cout << buffer.str();
	}



}

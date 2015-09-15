#include "IR.h"
#include "IRVisitors.h"
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
			return std::dynamic_pointer_cast<STLabelDef>( branch->src1() );
		}
		else if (auto branch = dynamic_cast<IR::CondFalseJump*>(instruction.get())) {
			return std::dynamic_pointer_cast<STLabelDef>( branch->src1() );
		}
		else if (auto branch = dynamic_cast<IR::Jump*>(instruction.get())) {
			return std::dynamic_pointer_cast<STLabelDef>( branch->tgt() );
		}
		else
			return nullptr;
	}

	string linearDumpTox86ParamRegName(int paramIndex) {
		switch (paramIndex) {
			case 0:
				return "%rdi";
				break;
			case 1:
				return "%rsi";
				break;
			case 2:
				return "%rdx";
				break;
			case 3:
				return "%rcx";
				break;
			case 4:
				return "%r8";
				break;
			case 5:
				return "%r9";
				break;
			default:
				return nullptr;
		}
	}

	bool isRegisterParam(shared_ptr<Parser::SymbolTableEntry> var) {
		auto pDecl = std::dynamic_pointer_cast<STParamDecl>(var);

		if (pDecl != nullptr)
			if (pDecl->offset() <= 5)
				return true;

		return false;
	}

	string linearDumpTox86VarLocation(shared_ptr<Parser::SymbolTableEntry> var) {
		auto pDecl = std::dynamic_pointer_cast<STParamDecl>(var);
		auto vDecl = std::dynamic_pointer_cast<STVariableDeclaration>(var);
		auto cDecl = std::dynamic_pointer_cast<STConstantDef>(var);
		stringstream ss;

		if (pDecl != nullptr) {
			if (pDecl->offset() <= 5)
				return Util::linearDumpTox86ParamRegName(pDecl->offset());
			else
				ss << std::to_string(pDecl->offset()) << "(%rbp)";
		}
		else if (vDecl != nullptr) {
			ss << "-" << std::to_string(vDecl->offset()) << "(%rbp)";
		}
		else if (cDecl != nullptr) {
			if (cDecl->type() == Parser::INT) {
				ss << "$" << cDecl->getName();
			}
			else {
				ss << "!&*+";
			}
		}
		else {
			return "@#$%";
		}

		return ss.str();
	}
}


namespace IR {
	void Module::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Function::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void BasicBlock::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void ScalarCopy::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void CopyFromArray::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void CopyToArray::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void Immediate::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Memory::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Register::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Func::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void IAdd::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void ISub::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IMul::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IDiv::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IMod::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IMinus::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IInc::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void IDec::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void FAdd::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FSub::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FMul::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FDiv::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FMinus::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	//void FPlus::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FInc::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void FDec::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void BinAnd::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void BinOr::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void BinXor::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void BinNot::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void RLesThan::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void RLesThanEqual::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void RGreaterThan::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void RGreaterThanEqual::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void REqual::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void RNotEqual::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void Jump::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void CondTrueJump::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void CondFalseJump::accept(IRTreeVisitor* visitor) { visitor->visit(this); }

	void Addr::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void AddrDispl::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Call::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Return::accept(IRTreeVisitor* visitor) { visitor->visit(this); }
	void Phi::accept(IRTreeVisitor* visitor) { visitor->visit(this); }











	void Register::dump(stringstream& buffer) {
		buffer << "Reg;" << endl;
	}

	void Immediate::dump(stringstream& buffer) {
		buffer << "Imm;" << endl;
	}

	void Memory::dump(stringstream& buffer) {
		buffer << "Mem" << endl;
	}

	void Func::dump(stringstream& buffer) {
		buffer << "Func" << endl;
	}



	void ScalarCopy::dump(stringstream& buffer) {
		buffer << this->_chd1->tgtDataName() << " = " << this->_chd2->tgtDataName() << ";" << endl;
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

	void IInc::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " + 1;" << endl;
	}

	void IDec::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " - 1;" << endl;
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

//	void FPlus::dump(stringstream& buffer) {
//		buffer << this->_tgt->getName() << " = +" << this->_src1->getName() << ";" << endl;
//	}

	void FInc::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " + 1;" << endl;
	}

	void FDec::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " - 1;" << endl;
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

	void AddrDispl::dump(stringstream& buffer) {
		buffer << this->_tgt->getName() << " = " << this->_src1->getName() << " ++ " << this->_src2->getName() << ";" << endl;
	}

	void Call::dump(stringstream& buffer) {
		if (this->_tgt != nullptr)
			buffer << this->_tgt->getName() << " = " << this->_src1->getName() << "(";
		else
			buffer << this->_src1->getName() << "(";

		auto arguments = this->arguments();

		if (arguments != nullptr) {
			if (arguments->size() > 0)
				buffer << (*arguments)[0]->tgtDataName();

			for (int i=1; i<arguments->size(); i++)
				buffer << ", " << (*arguments)[i]->tgtDataName();
		}

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

		for (auto instruction : *this->_subtrees) {
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


	void Function::appendBasicBlock(BasicBlock_sptr bb) {
		this->_bbs->push_back(bb);
		this->_currentBasicBlock = bb;
	}

	void Function::appendLabel(shared_ptr<STLabelDef> label) { }

	void Function::appendInstruction(shared_ptr<IR::Instruction> instr) {
		this->_currentBasicBlock->appendInstruction(instr);
		
		//if (this->_labelPendingSlot) {
		//	this->_subtrees->back().second = instr;
		//	this->_labelPendingSlot = false;
		//}
		//else {
		//	this->_subtrees->push_back( make_pair(nullptr, instr) );
		//}
	}

//	ControlFlowGraph_sptr Function::cfg() {
//		/* Index of leader/ender's instructions of each basic block.. */
//		vector<pair<int,int>> bbLimits;
//
//		/* Which instructions are leaders? */
//		set<int> leaders;
//
//		/* Map from instruction label to bb-id. */
//		map<STLabelDef_sptr, BasicBlock_sptr> labelToBB;
//
//		/* The CFG we are building */
//		auto newCfg = make_shared<Backend::ControlFlowGraph>();
//
//		/* We still need information about the "vars" inside this function, right? */
//		newCfg->addr(this->_addr);
//		newCfg->symbTable(this->_symbTable);
//
//		/* If the current instruction is a control handling instruction
//		 * then the next one is a leader for the fallthrought case. */
//		bool nextIsLeader = false;
//
//		/* Identifiers for basic blocks. */
//		auto bbId = 1;
//
//		/* Find the leaders */
//		unsigned int instrInd = 0;
//		for (auto& irLine : *this->_subtrees) {
//			if (irLine.first != nullptr || nextIsLeader) {
//				/* Save index of where the BB starts. */
//				bbLimits.push_back(make_pair(instrInd, 0));
//
//				/* We need to know later which instructions aren't leaders. */
//				leaders.insert(instrInd);
//			}
//
//			nextIsLeader = Util::isControlInstr(irLine.second);
//
//			instrInd++;
//		}
//
//		/* Find the "enders". */
//		for (auto& ldrIndx : bbLimits) {
//			instrInd = ldrIndx.first + 1; // next instruction after the leader (i.e., first in the BB)
//
//			// Find the end of the BB
//			while (instrInd < this->_subtrees->size() && leaders.find(instrInd) == leaders.end())
//				instrInd++;
//
//			// second points to one after end
//			ldrIndx.second = instrInd;
//		}
//
//		/* Construct the nodes of the CFG. */
//		BasicBlock_sptr prevBB = nullptr;
//		for (auto& bbBorder : bbLimits) {
//			auto beg = this->_subtrees->begin();
//			auto end = this->_subtrees->begin();
//
//			std::advance(beg, bbBorder.first);
//			std::advance(end, bbBorder.second);
//
//			auto newBB = make_shared<Backend::BasicBlock>(bbId++, make_shared<list<IRLine>>( list<IRLine>(beg, end) ));
//
//			/* If the current basic block header may be a target of a jump (i.e.,
//			 * the first instruction have a label, then we need to keep that
//			 * information for later we need to add these edges to the CFG. */
//			if (beg->first != nullptr)
//				labelToBB[beg->first] = newBB;
//
//			/* We check if the previous basic block may fallthrought to the
//			 * current basic block. */
//			if (prevBB != nullptr && prevBB->instructions()->size() > 0) {
//				auto prevLastInstr 		= prevBB->instructions()->back();
//				auto typePrevLastInstr 	= Util::isControlInstr(prevLastInstr.second);
//
//				/* Does the last instruction of the previous BB have a fallthrought?
//				 * Only when it is a return instruction that it does not. */
//				if (typePrevLastInstr != Util::ReturnInstruction)
//					prevBB->jmpFall(newBB);
//			}
//
//			/* Add BB to CFG. */
//			newCfg->addBasicBlock(newBB);
//			
//			/* Update the previous BB. */
//			prevBB = newBB;
//		}
//
//		/* Every function has exactly one exit basic block. */
//		auto exitBasicBlock = make_shared<Backend::BasicBlock>(bbId++, make_shared<list<IRLine>>( list<IRLine>() ));
//
//		/* jumping/branching edges. Fallthrought edges have already
//		 * been added. */
//		for (auto& bb : newCfg->nodes()) {
//			/* If the block have instructions. */
//			if (bb->instructions() != nullptr && bb->instructions()->size() > 0) {
//				auto lastInstr = bb->instructions()->back().second;
//				auto typeLastInstr = Util::isControlInstr(lastInstr);
//
//				if (typeLastInstr == Util::ReturnInstruction)
//					bb->jmpLabel(exitBasicBlock);
//				else if (typeLastInstr == Util::BranchInstruction)
//					bb->jmpLabel( labelToBB[ Util::branchTarget(lastInstr) ] );
//			}
//
//			if (bb->jmpFall() == nullptr && bb->jmpLabel() == nullptr)
//				bb->jmpFall(exitBasicBlock);
//		}
//
//		/* Add BB to CFG. */
//		newCfg->addBasicBlock(exitBasicBlock);
//
//		return newCfg;
//	}


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

























	void ScalarCopy::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax	\t\t\t# x86_scalar_copy" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void CopyFromArray::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax  	\t\t\t# x86_copy_from_array" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq (%rax), %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rbx, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void CopyToArray::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax  	\t\t\t# x86_copy_to_array" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_tgt) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, (%rbx)" << endl;
	}



	void IAdd::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_iadd" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "add %rax, %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rbx, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void ISub::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_isub" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sub %rbx, %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void IMul::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_imul" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "imul %rax, %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rbx, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void IDiv::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_idiv" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "xor %rdx, %rdx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "div %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void IMod::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_imod" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "xor %rdx, %rdx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "div %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rdx, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void IMinus::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_iminus" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "neg %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void IInc::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_inc" << endl;
		buffer << "inc %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void IDec::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_dec" << endl;
		buffer << "dec %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}



	void FAdd::linearDumpTox86(stringstream& buffer) { buffer << "x86_fadd();" << endl; }

	void FSub::linearDumpTox86(stringstream& buffer) { buffer << "x86_fsub();" << endl; }

	void FMul::linearDumpTox86(stringstream& buffer) { buffer << "x86_fmul();" << endl; }

	void FDiv::linearDumpTox86(stringstream& buffer) { buffer << "x86_fdiv();" << endl; }

	void FMinus::linearDumpTox86(stringstream& buffer) { buffer << "x86_fminus();" << endl; }

//	void FPlus::linearDumpTox86(stringstream& buffer) { buffer << "x86_fplus();" << endl; }

	void FInc::linearDumpTox86(stringstream& buffer) { buffer << "x86_finc();" << endl; }

	void FDec::linearDumpTox86(stringstream& buffer) { buffer << "x86_fdec();" << endl; }



	void BinAnd::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_binary_and" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "and %rax, %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rbx, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void BinOr::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_binary_or" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "or %rax, %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rbx, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void BinXor::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_binary_xor" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "xor %rax, %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rbx, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void BinNot::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_binary_not" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "not %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}



	void RLesThan::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_rel_less_than" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "cmpq %rbx, %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "setl %al" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "mov $1, %bl" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sub %al, %bl" << endl;
	}

	void RLesThanEqual::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_rel_less_than_equal" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "cmpq %rbx, %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "setle %al" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "mov $1, %bl" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sub %al, %bl" << endl;
	}

	void RGreaterThan::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_rel_greater_than" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "cmpq %rbx, %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "setg %al" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "mov $1, %bl" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sub %al, %bl" << endl;
	}

	void RGreaterThanEqual::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_rel_greater_than_equal" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "cmpq %rbx, %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "setge %al" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "mov $1, %bl" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sub %al, %bl" << endl;
	}

	void REqual::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_rel_equal" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "cmpq %rbx, %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sete %al" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "mov $1, %bl" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sub %al, %bl" << endl;
	}

	void RNotEqual::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_rel_not_equal" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "cmpq %rbx, %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "setne %al" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "mov $1, %bl" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sub %al, %bl" << endl;
	}



	void Jump::linearDumpTox86(stringstream& buffer) {
		// jump anyway
		buffer << "jmp " << this->_tgt->getName() << endl;
	}

	void CondTrueJump::linearDumpTox86(stringstream& buffer) {
		// jump if the "zero" flag is set
		buffer << "jz " << this->_src1->getName() << endl;
	}

	void CondFalseJump::linearDumpTox86(stringstream& buffer) {
		// jump if the "zero" flag is not set
		buffer << "jnz " << this->_src1->getName() << endl;
	}



	void Addr::linearDumpTox86(stringstream& buffer) {
		if (Util::isRegisterParam(this->_src1)) {
			buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
		}
		else {
			buffer << "lea " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax	\t\t\t # x86_addr_of" << endl;
			buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
		}
	}

	void AddrDispl::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_src1) << ", %rax 	\t\t\t# x86_addr_displ" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation(this->_src2) << ", %rbx" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "sub %rbx, %rax" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(this->_tgt) << endl;
	}

	void Call::linearDumpTox86(stringstream& buffer) {
//		auto calleeName = this->_src1->getName();
//		auto resVar 	= this->_tgt;
//		auto arguments 	= this->arguments();
//
//		buffer << " \t\t\t # x86_call" << endl;
//
//		/* Save register and push parameters */
//		if (resVar != nullptr)
//			buffer << std::setfill(' ') << std::setw(17) << " " << "pushq %rax" << endl;
//
//		if (arguments != nullptr) {
//			int i = 0;
//			int l = arguments->size() - 1;
//
//			// the first six parameters are by register
//			for (i=0; i<6 && i<=l; i++) {
//				buffer << std::setfill(' ') << std::setw(17) << " " << "pushq " << Util::linearDumpTox86ParamRegName(i) << endl;
//				buffer << std::setfill(' ') << std::setw(17) << " " << "movq " << Util::linearDumpTox86VarLocation((*arguments)[i]) << ", " << Util::linearDumpTox86ParamRegName(i) << endl;
//			}
//
//			// the remaining parameters are passed through stack
//			for (; l>=i && l>=6; l--)
//				buffer << std::setfill(' ') << std::setw(17) << " " << "pushq " << Util::linearDumpTox86VarLocation((*arguments)[l]) << endl;
//		}
//
//		/* Do the actual call */
//		buffer << std::setfill(' ') << std::setw(17) << " " << "call " << calleeName << endl;
//
//		/* Restore register and pop parameters */
//		if (arguments != nullptr) {
//			int i = 0;
//			int l = arguments->size() - 1;
//
//			// the remaining parameters are passed through stack
//			for (; l>=6; l--)
//				buffer << std::setfill(' ') << std::setw(17) << " " << "popq " << Util::linearDumpTox86VarLocation((*arguments)[l]) << endl;
//
//			// the first six parameters are by register
//			for (i=l; i>=0; i--)
//				buffer << std::setfill(' ') << std::setw(17) << " " << "popq " << Util::linearDumpTox86ParamRegName(i) << endl;
//		}
//
//		if (resVar != nullptr) {
//			buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rax, " << Util::linearDumpTox86VarLocation(resVar) << endl;
//			buffer << std::setfill(' ') << std::setw(17) << " " << "popq %rax "<< endl;
//		}
	}

	void Return::linearDumpTox86(stringstream& buffer) {
		buffer << "movq " << Util::linearDumpTox86VarLocation(this->_tgt) << ", %rax \t\t\t # x86_ret" << endl;
	}

	void Phi::linearDumpTox86(stringstream& buffer) {
		cout << "It is impossible to dump a PhiFun to x86." << endl;
		exit(-1);
	}

	void Function::linearDumpTox86MemAllocs() {
		auto entries 		= this->_symbTable->entries();
		int paramTotSize 	= 0;
		int paramIndex		= 0;
		int paramOffset		= 16;

		// Find total ammount of memory used by parameters
		for (auto& entry : entries) {
			auto pDecl = std::dynamic_pointer_cast<STParamDecl>(entry.second);
			if (pDecl != nullptr) {
				paramTotSize += pDecl->width();

				// If this parameter still fits in registers (i.e., < 6th
				if (paramIndex <= 5) {
					pDecl->offset(paramIndex++);
				}
				else {
					// No, we have more than 6 parameters, put them in memory
					pDecl->offset(paramOffset);
					paramOffset += 8; // all parameters are 64bits; pDecl->getWidth();
				}
			}
		}

		// update the local variables offsets taking into consideration
		// that now parameters are "outside" the function frame
		for (auto& entry : entries) {
			auto pDecl = std::dynamic_pointer_cast<STParamDecl>(entry.second);
			auto vDecl = std::dynamic_pointer_cast<STVariableDeclaration>(entry.second);

			// this "-8" is because the "0(%rbp)" is the old %rbp
			if (pDecl == nullptr && vDecl != nullptr) {
				vDecl->offset(vDecl->offset() - paramTotSize + 8);
			}
		}
	}

	void Function::linearDumpTox86Prologue(stringstream& buffer) {
		auto frameSize = this->_symbTable->stackFrameSize();

		buffer << std::setfill(' ') << std::setw(17) << " " << "pushq %rbp" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq  %rsp, %rbp" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "subq  $" << frameSize << ", %rsp" << endl;
	}

	void Function::linearDumpTox86Epilogue(stringstream& buffer) {
		buffer << std::setfill(' ') << std::setw(17) << " " << "movq %rbp, %rsp" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "popq %rbp" << endl;
		buffer << std::setfill(' ') << std::setw(17) << " " << "ret" << endl;
	}

	void Function::linearDumpTox86(stringstream& buffer) {
		auto funDecl = std::dynamic_pointer_cast<STFunctionDeclaration>(this->_addr);
		auto label 	 = funDecl->getLabel();

		buffer << ".globl " << label << endl;
		buffer << label << ":" << endl;

		this->linearDumpTox86MemAllocs();

		//this->_symbTable->dump(buffer);

		this->linearDumpTox86Prologue(buffer);

		for (auto instruction : *this->_subtrees) {
			/* Do we have a label here?  */
			if (instruction.first != nullptr)
				buffer << std::setfill(' ') << std::setw(15) << instruction.first->getName() << ": ";
			else
				buffer << std::setfill(' ') << std::setw(17) << " ";

			/* Is it just a label? */
			if (instruction.second != nullptr)
				instruction.second->linearDumpTox86(buffer);
			else
				buffer << endl;
		}

		this->linearDumpTox86Epilogue(buffer);
	}

	void Module::linearDumpTox86(std::stringstream& buffer) {
//	   buffer << "This is the x86 Assembly: " << endl;
//	   buffer << std::setfill('-') << std::setw(80) << "-" << endl;
//
	   buffer << "\t.file \"file.c\"" << endl;
	   buffer << "\t.text" << endl << endl;

		for (auto function : *this->_functions) {
			function->linearDumpTox86(buffer);
			buffer << endl << endl;
		}
	}
}

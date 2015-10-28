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

				_inInstr[instr]= make_shared<STEntry_set>();
				_outInstr[instr]= make_shared<STEntry_set>();

				this->_defs[instr] = instr->defs();
				this->_uses[instr] = instr->uses();
			}

			_outBb[bb] = make_shared<STEntry_set>();
			_inBb[bb]= make_shared<STEntry_set>();
		}
	}


	void LiveVariables::execute() {
		bool hadChanges = true;

		while (hadChanges) {
			hadChanges = false;			

			cout << "LiveVariablesAnalysis iterating." << endl;

			for (auto& bb : *_bbs) {
				// This represents the UNION of INs of successors BBs/Instructions
				auto prevIns = make_shared<STEntry_set>();

				// for each successor basic block
				auto succs = *bb->succs();
				for (auto succ : succs) {
					auto firstIn = _inBb[succ];

					// This is just to contain tmp Unions
					auto tmpRes = make_shared<STEntry_set>();

					// Add together the OUT of the last bb with the OUT of the current one
					std::set_union(	prevIns->begin(), prevIns->end(), 
									firstIn->begin(), firstIn->end(), 
									std::inserter(*tmpRes, tmpRes->begin()));

					prevIns = tmpRes;
				}

				// Output of current basic block comes from the Ins of the successor BBs
				_outBb[bb] = prevIns;

	

				// So the in of the first instruction is the UNION of the OUT of 
				// the predecessors. In the case of the first instruction of the BB the OUT
				// is the UNION of OUT's of all predecessors.
				// Now compute the IN/OUT of each instruction
				auto begInstrIte = bb->instructions().rbegin();
				auto endInstrIte = bb->instructions().rend();

				while (begInstrIte != endInstrIte) {
					auto instr = (*begInstrIte).shared_from_this();

					_outInstr[instr] = prevIns;

					auto tmpIns = make_shared<STEntry_set>();

					// Add together the OUT of the last bb with the OUT of the current one
					std::set_difference(_outInstr[instr]->begin(), _outInstr[instr]->end(), 
										_defs[instr]->begin(), _defs[instr]->end(), 
										std::inserter(*tmpIns, tmpIns->begin()));

					auto newIns = make_shared<STEntry_set>();

					std::set_union(_uses[instr]->begin(), _uses[instr]->end(), 
									tmpIns->begin(), tmpIns->end(), 
									std::inserter(*newIns, newIns->begin()));

					if (*_inInstr[instr] != *newIns) {
						hadChanges = true;	
						_inInstr[instr] = newIns;
					}

					prevIns = newIns;

					begInstrIte++;
				}

				_inBb[bb] = prevIns;
			}
		}

	}

	void LiveVariables::dump() {
		stringstream output;

		output << "Live Variable Analysis dump output: " << endl;

		for (auto& bb : *_bbs) {
			output << "BB" << bb->id() << ":" << endl;

			for (auto& ins : bb->instructions()) {
				auto instr = ins.shared_from_this();

				output << std::setw(3) << _identifier[instr] << ": ";
				instr->dump(output);

				output << "\t|-> IN {";
				for (auto in : *_inInstr[instr]) {
					output << in->getName() << " ";
				}	

				output << "} OUT { ";
				for (auto out : *_outInstr[instr]) {
					output << out->getName() << " ";
				}	

				output << "} DEFS { ";
				for (auto d : *_defs[instr]) {
					output << d->getName() << " ";
				}	

				output << "} USES { ";
				for (auto u : *_uses[instr]) {
					output << u->getName() << " ";
				}	
				output << "} " << endl;
			}
		}

		cout << output.str() << endl;
	}

}

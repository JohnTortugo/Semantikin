#include "DataFlowAnalysis.h"

#include <iostream>

namespace DFA {

ReachingDefinitions::ReachingDefinitions(BasicBlock_list_sptr bbs) : _bbs(bbs) {

}

void ReachingDefinitions::execute() {
	std::cout << "Going to execute RD " << std::endl;
}

}

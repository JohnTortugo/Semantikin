#ifndef CONTROLFLOWGRAPH_H_
#define CONTROLFLOWGRAPH_H_

#include "Semantikin.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace Backend {

	class ControlFlowGraph {
	private:
		vector<BasicBlock_sptr> _nodes;

	public:
		void dumpToDot(string targetFileName);

		void addBasicBlock(BasicBlock_sptr newBB) { _nodes.push_back(newBB); }

		vector<BasicBlock_sptr> nodes() const { return _nodes; }
	};

}

#endif /* CONTROLFLOWGRAPH_H_ */

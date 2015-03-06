#include "ControlFlowGraph.h"
#include "BasicBlock.h"
#include <fstream>
#include <sstream>

using std::stringstream;
using std::ofstream;
using std::endl;

namespace Backend {

	void ControlFlowGraph::dumpToDot(string targetFileName) {
		stringstream buffer;

		buffer << "digraph CFGGraph {" << endl;
		buffer << "\tlabel = \"This is the CFG for function " << this->_addr->getName() << " \";" << endl;

				for (auto bb : this->_nodes) {
					buffer << "\t\"" << bb << "\" [shape=record, label=\"{BB" << bb->id() << "|";
					bb->dumpToDot(buffer);
					buffer << "}\"];" << endl;

					if (bb->jmpFall() != nullptr)
						buffer << "\t\"" << bb << "\" -> \"" << bb->jmpFall() << "\";" << endl;

					if (bb->jmpLabel() != nullptr)
						buffer << "\t\"" << bb << "\" -> \"" << bb->jmpLabel() << "\";" << endl;
				}

		buffer << "\t}" << endl;


		/* Write dot code to file. */
		std::ofstream dotfile;
		dotfile.open(targetFileName);
		dotfile << buffer.str();
		dotfile.close();
	}


}

//#ifndef CONTROLFLOWGRAPH_H_
//#define CONTROLFLOWGRAPH_H_
//
//#include "Semantikin.h"
//#include <vector>
//#include <string>
//#include <memory>
//
//using std::vector;
//using std::string;
//
//namespace Backend {
//
//	class ControlFlowGraph {
//	private:
//		/* Pointer to the symbol table entry describing this function. */
//		shared_ptr<Parser::SymbolTableEntry> _addr;
//
//		/* Pointer to this function's symbol table. */
//		shared_ptr<Parser::SymbolTable> _symbTable;
//
//		/* These are the nodes of the CFG. */
//		vector<BasicBlock_sptr> _nodes;
//
//	public:
//
//		/* Methods related to setting/getting the function's declaration. */
//		void addr(shared_ptr<Parser::SymbolTableEntry> addr) { this->_addr = addr; }
//		shared_ptr<Parser::SymbolTableEntry> addr() { return this->_addr; }
//
//		/* Methods related to setting/getting the symbol table. */
//		void symbTable(shared_ptr<Parser::SymbolTable> st) { this->_symbTable = _symbTable; }
//		shared_ptr<Parser::SymbolTable> symbTable() { return this->_symbTable; }
//
//
//		void dumpToDot(string targetFileName);
//
//		void addBasicBlock(BasicBlock_sptr newBB) { _nodes.push_back(newBB); }
//
//		vector<BasicBlock_sptr> nodes() const { return _nodes; }
//	};
//
//}
//
//#endif /* CONTROLFLOWGRAPH_H_ */

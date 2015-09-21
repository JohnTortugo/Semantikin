#include "Semantikin.h"
#include "IRVisitors.h"

using namespace IR;

#define DOT_NODE_ID(_node_)				"ND" << std::setw(5) << _node_
#define DOT_NEW_NODE(_node_, _label_)	DOT_NODE_ID(_node_) << " [label=\"" << _label_ << "\"];"
#define DOT_EDGE(_src_, _dst_)			DOT_NODE_ID(_src_) << " -> " << DOT_NODE_ID(_dst_) << ";"

void IRToDotVisitor::visit(IR::Module* module) {
	cout << "On ITD-Module " << endl;

	this->incIdentation();

	for (auto func : *module->functions()) {
		func->accept(this);
	}
}

void IRToDotVisitor::visit(IR::Function* func) {
	cout << "On ITD-Func " << endl;

	// Reset the cross edges
	this->crossEdges.clear();

	///// For creating the basic blocks with their subtrees
	this->newline() << endl << "subgraph cluster_bbs_" << func << " { " << endl;
	this->incIdentation();
	this->newline() << "label=\"BBs of function " << func->name() << "\";" << endl;

	// Produces node/edges for each BB
	for (auto& bb: *func->bbs()) {
		bb->accept(this);
	}

	this->decIdentation();
	this->newline() << "}" << endl;



	///// For creating the CFG
	this->newline() << endl << "subgraph cluster_cfg_" << func << " { " << endl;
	this->incIdentation();
	this->newline() << "label=\"CFG for function " << func->name() << "\";" << endl;

	// Create the CFG
	for (auto& edge: this->crossEdges) {
		this->newline() << "BB" << std::get<0>(edge) << func << " [label=\"BB" << std::get<0>(edge) << "\"]"; 
		this->newline() << "BB" << std::get<1>(edge) << func << " [label=\"BB" << std::get<1>(edge) << "\"]";

		this->newline() << "BB" << std::get<0>(edge) << func << " -> BB" << std::get<1>(edge) << func << " [label=" << std::get<2>(edge) << "];" << endl;
	}


	this->decIdentation();
	this->newline() << "}" << endl;
}

void IRToDotVisitor::visit(IR::BasicBlock* bb) {
	cout << "On ITD-BasicBlock " << endl;

	this->_currentBasicBlock = bb;

	this->newline() << endl << "subgraph cluster_" << bb << " { " << endl;
	this->incIdentation();
	this->newline() << "label=\"BB" << bb->id() << "\";" << endl;
	this->newline() << "invis_" << bb << " [style=invisible,label=x,height=.1, width=.1];" << endl;

	for (auto subtree : *bb->instructions()) {
		subtree->accept(this);
	}

	this->decIdentation();
	this->newline() << "}" << endl;
}

void IRToDotVisitor::visit(IR::ScalarCopy* copy) {
	cout << "On ITD-ScalarCopy " << endl;

	this->newline() << DOT_NEW_NODE(copy, "=") << endl;
	this->newline() << DOT_EDGE(copy, copy->chd1()) << endl;
	this->newline() << DOT_EDGE(copy, copy->chd2()) << endl;

	copy->chd1()->accept(this);
	copy->chd2()->accept(this);
}

void IRToDotVisitor::visit(IR::CopyFromArray* copy) {
	cout << "On ITD-CopyFromArray " << endl;

	this->newline() << DOT_NEW_NODE(copy, "=[]") << endl;
	this->newline() << DOT_EDGE(copy, copy->chd1()) << endl;
	this->newline() << DOT_EDGE(copy, copy->chd2()) << endl;

	copy->chd1()->accept(this);
	copy->chd2()->accept(this);
}

void IRToDotVisitor::visit(IR::CopyToArray* copy) {
	cout << "On ITD-CopyToArray " << endl;

	this->newline() << DOT_NEW_NODE(copy, "[]=") << endl;
	this->newline() << DOT_EDGE(copy, copy->chd1()) << endl;
	this->newline() << DOT_EDGE(copy, copy->chd2()) << endl;

	copy->chd1()->accept(this);
	copy->chd2()->accept(this);
}


void IRToDotVisitor::visit(IR::Register* reg) {
	cout << "On ITD-Reg " << endl;
	this->newline() << DOT_NEW_NODE(reg, reg->tgtDataName()) << endl;
}

void IRToDotVisitor::visit(IR::Memory* mem) {
	cout << "On ITD-Mem " << endl;
	this->newline() << DOT_NEW_NODE(mem, mem->tgtDataName()) << endl;
}

void IRToDotVisitor::visit(IR::Immediate* imm) {
	cout << "On ITD-Imm " << endl;
	this->newline() << DOT_NEW_NODE(imm, imm->tgtDataName()) << endl;
}

void IRToDotVisitor::visit(IR::Func* func) {
	cout << "On ITD-Func " << endl;
	this->newline() << DOT_NEW_NODE(func, func->tgtDataName()) << endl;
}

void IRToDotVisitor::visit(IR::IAdd* node) {
	cout << "On ITD-IAdd " << endl;
	binaryDispatcher(node, "+");
}

void IRToDotVisitor::visit(IR::ISub* node) {
	cout << "On ITD-ISub " << endl;
	binaryDispatcher(node, "-");
}

void IRToDotVisitor::visit(IR::IMul* node) {
	cout << "On ITD-IMul " << endl;
	binaryDispatcher(node, "*");
}

void IRToDotVisitor::visit(IR::IDiv* node) {
	cout << "On ITD-IDiv " << endl;
	binaryDispatcher(node, "/");
}

void IRToDotVisitor::visit(IR::IMod* node) {
	cout << "On ITD-IMod " << endl;
	binaryDispatcher(node, "%");
}

void IRToDotVisitor::visit(IR::IMinus* node) {
	cout << "On ITD-IMinus " << endl;
	unaryDispatcher(node, "-");
}

void IRToDotVisitor::visit(IR::IInc* node) {
	cout << "On ITD-IInc " << endl;
	unaryDispatcher(node, "++");

}

void IRToDotVisitor::visit(IR::IDec* node) {
	cout << "On ITD-IDec" << endl;
	unaryDispatcher(node, "--");
}





void IRToDotVisitor::visit(IR::FAdd* node) {
	cout << "On ITD-FAdd " << endl;
	binaryDispatcher(node, "+");
}

void IRToDotVisitor::visit(IR::FSub* node) {
	cout << "On ITD-FSub " << endl;
	binaryDispatcher(node, "-");
}

void IRToDotVisitor::visit(IR::FMul* node) {
	cout << "On ITD-FMul " << endl;
	binaryDispatcher(node, "*");
}

void IRToDotVisitor::visit(IR::FDiv* node) {
	cout << "On ITD-FDiv " << endl;
	binaryDispatcher(node, "/");
}

void IRToDotVisitor::visit(IR::FMinus* node) {
	cout << "On ITD-FMinus " << endl;
	unaryDispatcher(node, "-");
}

void IRToDotVisitor::visit(IR::FInc* node) {
	cout << "On ITD-FInc " << endl;
	unaryDispatcher(node, "++");
}

void IRToDotVisitor::visit(IR::FDec* node) {
	cout << "On ITD-FDec " << endl;
	unaryDispatcher(node, "--");
}






void IRToDotVisitor::visit(IR::BinAnd* node) {
	cout << "On ITD-BinAnd " << endl;
	binaryDispatcher(node, "&");
}

void IRToDotVisitor::visit(IR::BinOr* node) {
	cout << "On ITD-BinOr " << endl;
	binaryDispatcher(node, "|");
}

void IRToDotVisitor::visit(IR::BinXor* node) {
	cout << "On ITD-BinXor " << endl;
	binaryDispatcher(node, "^");
}

void IRToDotVisitor::visit(IR::BinNot* node) {
	cout << "On ITD-BinNot " << endl;
	binaryDispatcher(node, "!");
}



void IRToDotVisitor::visit(IR::RLesThan* node) {
	cout << "On ITD-RLesThan " << endl;
	binaryDispatcher(node, "<");
}

void IRToDotVisitor::visit(IR::RLesThanEqual* node) {
	cout << "On ITD-RLesThanEqual " << endl;
	binaryDispatcher(node, "<=");
}

void IRToDotVisitor::visit(IR::RGreaterThan* node) {
	cout << "On ITD-RGreaterThan " << endl;
	binaryDispatcher(node, ">");
}

void IRToDotVisitor::visit(IR::RGreaterThanEqual* node) {
	cout << "On ITD-RGreaterThanEqual " << endl;
	binaryDispatcher(node, ">=");
}

void IRToDotVisitor::visit(IR::REqual* node) {
	cout << "On ITD-REqual " << endl;
	binaryDispatcher(node, "==");
}

void IRToDotVisitor::visit(IR::RNotEqual* node) {
	cout << "On ITD-RNotEqual " << endl;
	binaryDispatcher(node, "!=");
}



void IRToDotVisitor::visit(IR::Jump* node) { 
	cout << "On ITD-Jump" << endl;

	this->newline() << DOT_NEW_NODE(node, "goto") << endl;

	this->crossEdges.push_back( std::make_tuple(this->_currentBasicBlock->id(), node->lbl1()->id(), "goto") );
}

void IRToDotVisitor::visit(IR::Conditional* node) { 
	cout << "On ITD-If" << endl;

	this->newline() << DOT_NEW_NODE(node, "if") << endl;
	this->newline() << DOT_EDGE(node, node->chd1()) << endl;

	this->crossEdges.push_back( std::make_tuple(this->_currentBasicBlock->id(), node->lbl1()->id(), "True") );
	this->crossEdges.push_back( std::make_tuple(this->_currentBasicBlock->id(), node->lbl2()->id(), "False") );

	node->chd1()->accept(this);
}



void IRToDotVisitor::visit(IR::Addr* node) {
	cout << "On ITD-Addr " << endl;
	unaryDispatcher(node, "@");
}

void IRToDotVisitor::visit(IR::AddrDispl* node) {
	cout << "On ITD-AddrDisp " << endl;
	binaryDispatcher(node, "[]");
}

void IRToDotVisitor::visit(IR::Call* node) { 
	cout << "On ITD-FunCall" << endl;

	this->newline() << DOT_NEW_NODE(node, "call") << endl;
	this->newline() << DOT_EDGE(node, node->chd1()) << endl;
	this->newline() << DOT_EDGE(node, node->chd2()) << endl;

	if (node->arguments() != nullptr) {
		for (auto& arg : *node->arguments()) {
			this->newline() << DOT_EDGE(node, arg.get()) << endl;
			arg.get()->accept(this);
		}
	}

	node->chd1()->accept(this);
	node->chd2()->accept(this);
}

void IRToDotVisitor::visit(IR::Return* node) { 
	cout << "On ITD-Return" << endl;

	this->newline() << DOT_NEW_NODE(node, "Ret") << endl;
	this->newline() << DOT_EDGE(node, node->chd1()) << endl;

	node->chd1()->accept(this);
}

void IRToDotVisitor::visit(IR::Phi* visitor) { cout << "Not implemented." << endl; }



void IRToDotVisitor::binaryDispatcher(IR::Instruction* node, const char* label) {
	this->newline() << DOT_NEW_NODE(node, label) << endl;
	this->newline() << DOT_EDGE(node, node->chd1()) << endl;
	this->newline() << DOT_EDGE(node, node->chd2()) << endl;
	this->newline() << DOT_EDGE(node, node->chd3()) << endl;

	node->chd1()->accept(this);
	node->chd2()->accept(this);
	node->chd3()->accept(this);
}

void IRToDotVisitor::unaryDispatcher(IR::Instruction* node, const char* label) {
	this->newline() << DOT_NEW_NODE(node, label) << endl;
	this->newline() << DOT_EDGE(node, node->chd1()) << endl;
	this->newline() << DOT_EDGE(node, node->chd2()) << endl;

	node->chd1()->accept(this);
	node->chd2()->accept(this);
}

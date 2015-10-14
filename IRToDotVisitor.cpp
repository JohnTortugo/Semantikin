#include "Semantikin.h"
#include "IRVisitors.h"

using namespace IR;


#define DOT_NODE_ID(_node_)								"ND" << std::setw(5) << _node_
#define DOT_NEW_NODE_NO_TGT(_node_, _opr_)				DOT_NODE_ID(_node_) << " [shape=circle, fillcolor=\"#" << _node_->fillColor() << "\", label=\"" << Util::escapeStr(_opr_) << "\"];"
#define DOT_NEW_NODE_WITH_TGT(_node_, _opr_, __tgt__)	DOT_NODE_ID(_node_) << " [shape=record, fillcolor=\"#" << _node_->fillColor() << "\", margin=\".1\", label=\"{" << __tgt__ << " | " << Util::escapeStr(_opr_) << "}\"];"
#define DOT_EDGE(_src_, _dst_)							DOT_NODE_ID(_src_) << " -> " << DOT_NODE_ID(_dst_) << ";"

void IRToDotVisitor::visit(IR::Module* module) {
	this->incIdentation();

	for (auto func : *module->functions()) {
		func->accept(this);
	}
}

void IRToDotVisitor::visit(IR::Function* func) {
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
	this->_currentBasicBlock = bb;

	this->newline() << endl << "subgraph cluster_" << bb << " { " << endl;
	this->incIdentation();
	this->newline() << "label=\"BB" << bb->id() << "\";" << endl;
	this->newline() << "invis_" << bb << " [style=invisible,label=x,height=.1,width=.1];" << endl;

	Instruction_sptr prev = nullptr;
	for (auto& subtree : *bb->instructions()) {
		subtree->accept(this);

		if (prev != nullptr) {
			this->newline() << "{" << endl;
			this->newline() << "	rank=\"same\";" << endl;
			this->newline() << "	rankdir=\"LR\";" << endl;
			this->newline() << "	ND" << prev << " -> ND" << subtree << " [style=dotted, color=blue, minlen=2];" << endl;
			this->newline() << "}" << endl;
		}

		prev = subtree;
	}

	this->decIdentation();
	this->newline() << "}" << endl;
}

void IRToDotVisitor::visit(IR::ScalarCopy* node) {
	unaryDispatcher(node, "=");
}

void IRToDotVisitor::visit(IR::CopyFromArray* node) {
	unaryDispatcher(node, "=[]");
}

void IRToDotVisitor::visit(IR::CopyToArray* node) {
	unaryDispatcher(node, "[]=");
}


void IRToDotVisitor::visit(IR::Register* reg) {
	this->newline() << DOT_NEW_NODE_NO_TGT(reg, reg->tgtDataName()) << endl;
}

void IRToDotVisitor::visit(IR::Memory* mem) {
	this->newline() << DOT_NEW_NODE_NO_TGT(mem, mem->tgtDataName()) << endl;
}

void IRToDotVisitor::visit(IR::Immediate* imm) {
	this->newline() << DOT_NEW_NODE_NO_TGT(imm, imm->tgtDataName()) << endl;
}

void IRToDotVisitor::visit(IR::Func* func) {
	this->newline() << DOT_NEW_NODE_NO_TGT(func, func->tgtDataName()) << endl;
}

void IRToDotVisitor::visit(IR::IAdd* node) {
	binaryDispatcher(node, "+");
}

void IRToDotVisitor::visit(IR::ISub* node) {
	binaryDispatcher(node, "-");
}

void IRToDotVisitor::visit(IR::IMul* node) {
	binaryDispatcher(node, "*");
}

void IRToDotVisitor::visit(IR::IDiv* node) {
	binaryDispatcher(node, "/");
}

void IRToDotVisitor::visit(IR::IMod* node) {
	binaryDispatcher(node, "%");
}

void IRToDotVisitor::visit(IR::IMinus* node) {
	unaryDispatcher(node, "-");
}

void IRToDotVisitor::visit(IR::IInc* node) {
	unaryDispatcher(node, "++");

}

void IRToDotVisitor::visit(IR::IDec* node) {
	unaryDispatcher(node, "--");
}





void IRToDotVisitor::visit(IR::FAdd* node) {
	binaryDispatcher(node, "+");
}

void IRToDotVisitor::visit(IR::FSub* node) {
	binaryDispatcher(node, "-");
}

void IRToDotVisitor::visit(IR::FMul* node) {
	binaryDispatcher(node, "*");
}

void IRToDotVisitor::visit(IR::FDiv* node) {
	binaryDispatcher(node, "/");
}

void IRToDotVisitor::visit(IR::FMinus* node) {
	unaryDispatcher(node, "-");
}

void IRToDotVisitor::visit(IR::FInc* node) {
	unaryDispatcher(node, "++");
}

void IRToDotVisitor::visit(IR::FDec* node) {
	unaryDispatcher(node, "--");
}






void IRToDotVisitor::visit(IR::BinAnd* node) {
	binaryDispatcher(node, "&");
}

void IRToDotVisitor::visit(IR::BinOr* node) {
	binaryDispatcher(node, "|");
}

void IRToDotVisitor::visit(IR::BinXor* node) {
	binaryDispatcher(node, "^");
}

void IRToDotVisitor::visit(IR::BinNot* node) {
	unaryDispatcher(node, "!");
}



void IRToDotVisitor::visit(IR::RLesThan* node) {
	binaryDispatcher(node, "<");
}

void IRToDotVisitor::visit(IR::RLesThanEqual* node) {
	binaryDispatcher(node, "<=");
}

void IRToDotVisitor::visit(IR::RGreaterThan* node) {
	binaryDispatcher(node, ">");
}

void IRToDotVisitor::visit(IR::RGreaterThanEqual* node) {
	binaryDispatcher(node, ">=");
}

void IRToDotVisitor::visit(IR::REqual* node) {
	binaryDispatcher(node, "==");
}

void IRToDotVisitor::visit(IR::RNotEqual* node) {
	binaryDispatcher(node, "!=");
}



void IRToDotVisitor::visit(IR::Jump* node) { 
	this->newline() << DOT_NEW_NODE_NO_TGT(node, "goto") << endl;

	this->crossEdges.push_back( std::make_tuple(this->_currentBasicBlock->id(), node->lbl1()->id(), "goto") );
}

void IRToDotVisitor::visit(IR::Conditional* node) { 
	this->newline() << DOT_NEW_NODE_NO_TGT(node, "if") << endl;
	this->newline() << DOT_EDGE(node, node->tgt()) << endl;

	this->crossEdges.push_back( std::make_tuple(this->_currentBasicBlock->id(), node->lbl1()->id(), "True") );
	this->crossEdges.push_back( std::make_tuple(this->_currentBasicBlock->id(), node->lbl2()->id(), "False") );

	node->tgt()->accept(this);
}



void IRToDotVisitor::visit(IR::Addr* node) {
	unaryDispatcher(node, "@");
}

void IRToDotVisitor::visit(IR::AddrDispl* node) {
	binaryDispatcher(node, "[]");
}

void IRToDotVisitor::visit(IR::Call* node) { 
	this->newline() << DOT_NEW_NODE_WITH_TGT(node, "call", node->tgt()->tgtDataName()) << endl;
	this->newline() << DOT_EDGE(node, node->chd2()) << endl;

	if (node->arguments() != nullptr) {
		for (auto& arg : *node->arguments()) {
			this->newline() << DOT_EDGE(node, arg.get()) << endl;
			arg.get()->accept(this);
		}
	}

	node->tgt()->accept(this);
	node->chd2()->accept(this);
}

void IRToDotVisitor::visit(IR::Return* node) { 
	this->newline() << DOT_NEW_NODE_NO_TGT(node, "Ret") << endl;
	this->newline() << DOT_EDGE(node, node->tgt()) << endl;

	node->tgt()->accept(this);
}

void IRToDotVisitor::visit(IR::Phi* visitor) { cout << "Not implemented." << endl; }



void IRToDotVisitor::binaryDispatcher(IR::Instruction* node, const char* label) {
	this->newline() << DOT_NEW_NODE_WITH_TGT(node, label, node->tgt()->tgtDataName()) << endl;
	this->newline() << DOT_EDGE(node, node->chd2()) << endl;
	this->newline() << DOT_EDGE(node, node->chd3()) << endl;

	node->chd2()->accept(this);
	node->chd3()->accept(this);
}

void IRToDotVisitor::unaryDispatcher(IR::Instruction* node, const char* label) {
	this->newline() << DOT_NEW_NODE_WITH_TGT(node, label, node->tgt()->tgtDataName()) << endl;
	this->newline() << DOT_EDGE(node, node->chd2()) << endl;

	node->chd2()->accept(this);
}

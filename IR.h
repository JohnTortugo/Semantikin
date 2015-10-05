#ifndef IR_H_
#define IR_H_

#include "Semantikin.h"
#include "BasicBlock.h"
#include "AbstractSyntaxTree.h"

#include <string>
#include <list>
#include <memory>
#include <sstream>
#include <iomanip>
#include <map>

using std::stringstream;
using std::string;
using std::list;
using std::map;
using std::shared_ptr;
using std::make_shared;
using namespace Parser;

class IRTreeVisitor;

namespace IR {
	/* This class is the parent of all Three-address-code (TAC) IR instructions. */
	class Instruction {
	protected:
		// This is only used when dumping the IR to a visual format
		string _fillColor = "ffffff";
		unsigned int _myColor = 0;

		Instruction_sptr _chd1 = nullptr;
		Instruction_sptr _chd2 = nullptr;
		Instruction_sptr _chd3 = nullptr;

	public:
		Instruction()
		{ }

		Instruction(Instruction_sptr chd1, Instruction_sptr chd2, Instruction_sptr chd3) :
			_chd1(chd1), _chd2(chd2), _chd3(chd3)
		{ }

		Instruction(Instruction_sptr chd1) :
			_chd1(chd1), _chd2(nullptr), _chd3(nullptr)
		{ }


		void myColor(unsigned int mc) { this->_myColor = mc; }
		unsigned int myColor() { return this->_myColor; }

		void fillColor(const string& fc) { this->_fillColor = fc; }
		const string& fillColor() { return this->_fillColor; }

		void chd1(Instruction_sptr chd) { this->_chd1 = chd; }
		Instruction_sptr chd1() { return this->_chd1; }

		void chd2(Instruction_sptr chd) { this->_chd2 = chd; }
		Instruction_sptr chd2() { return this->_chd2; }

		void chd3(Instruction_sptr chd) { this->_chd3 = chd; }
		Instruction_sptr chd3() { return this->_chd3; }


		virtual const shared_ptr<vector<SymbolTableEntry_sp>> arguments() { return nullptr; }
		virtual void addArgument(SymbolTableEntry_sp argument) { }

		/** Used to traverse the IR tree. */
		virtual void accept(IRTreeVisitor* visitor) = 0;

		/** Used to dump in a "human readable" way the instruction's target operand */
		virtual string tgtDataName() = 0;

		virtual void dump(stringstream& buffer) = 0;

		virtual void linearDumpTox86(stringstream& buffer) = 0;

		virtual ~Instruction() {};
	};


	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all instructions that are actually
	 * placeholder for data. Like immediates, registers and
	 * memory addresses. 									*/
	class Data : public Instruction { };

	class Immediate : public Data {
	private:
		STConstantDef_sptr _value;

	public:
		Immediate(STConstantDef_sptr val) : _value(val)
		{ }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		STConstantDef_sptr value() { return this->_value; }

		string tgtDataName() { return Util::escapeStr(this->_value->getName()); }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer) { }
	};

	class Register : public Data {
	private:
		STRegister_sptr _value;

	public:
		Register(STRegister_sptr val) : _value(val)
		{ }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		STRegister_sptr value() { return this->_value; }

		string tgtDataName() { return this->_value->getName(); }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer) { }
	};

	class Memory : public Data {
	private:
		STVariableDeclaration_sptr _value;

	public:
		Memory(STVariableDeclaration_sptr val) : _value(val)
		{ }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		STVariableDeclaration_sptr value() { return this->_value; }

		string tgtDataName() { return this->_value->getName(); }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer) { }
	};

	class Func : public Data {
	private:
		STFunctionDecl_sptr  _value;

	public:
		Func(STFunctionDecl_sptr val) : _value(val)
		{ }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		STFunctionDecl_sptr value() { return this->_value; }

		string tgtDataName() { return this->_value->getName(); }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer) { }
	};







	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all data movement instructions. 		*/
	class Copy : public Instruction {
	public:
		Copy(Instruction_sptr tgt, Instruction_sptr value) : Instruction(tgt, value, nullptr)
		{
			if (tgt == nullptr || value == nullptr) {
				cout << "Some parameter is null..." << endl;
			}

		}
	};

	class ScalarCopy : public Copy {
	public:
		using Copy::Copy;

		/** Used to obtain the "human readable" name of the destination operand of the operation. */
		string tgtDataName() { return this->_chd1->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class CopyFromArray : public Copy {
		using Copy::Copy;

		/** Used to obtain the "human readable" name of the destination operand of the operation. */
		string tgtDataName() { return this->_chd1->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class CopyToArray : public Copy {
		using Copy::Copy;

		/** Used to obtain the "human readable" name of the destination operand of the operation. */
		string tgtDataName() { return this->_chd1->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};




	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all integer arithmetic instructions. */
	class IntegerArithmetic : public Instruction {
	public:
		IntegerArithmetic(Instruction_sptr chd1, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(chd1, chd2, chd3)
		{ }
	};

	class BinaryIntegerArithmetic : public IntegerArithmetic {
	public:
		BinaryIntegerArithmetic(Instruction_sptr chd1, Instruction_sptr chd2, Instruction_sptr chd3) : IntegerArithmetic(chd1, chd2, chd3)
		{ }
	};

	class UnaryIntegerArithmetic : public IntegerArithmetic {
	public:
		UnaryIntegerArithmetic(Instruction_sptr chd1, Instruction_sptr chd2) 
			: IntegerArithmetic(chd1, chd2, nullptr) { }
	};

	class IAdd : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class ISub : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IMul : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IDiv : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IMod : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IMinus : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IInc : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IDec : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};





	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all floating point arithmetic instructions. */
	class FloatingArithmetic : public Instruction {
	public:
		FloatingArithmetic(Instruction_sptr chd1, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(chd1, chd2, chd3)
		{ }
	};

	class BinaryFloatingArithmetic : public FloatingArithmetic {
	public:
		BinaryFloatingArithmetic(Instruction_sptr chd1, Instruction_sptr chd2, Instruction_sptr chd3) : FloatingArithmetic(chd1, chd2, chd3)
		{ }
	};

	class UnaryFloatingArithmetic : public FloatingArithmetic {
	public:
		UnaryFloatingArithmetic(Instruction_sptr chd1, Instruction_sptr chd2) : FloatingArithmetic(chd1, chd2, nullptr)
		{ }
	};

	class FAdd : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FSub : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FMul : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FDiv : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FMinus : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FInc : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FDec : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all binary arithmetic instructions.  */
	class BitArithmetic : public Instruction {
	public:
		BitArithmetic(Instruction_sptr chd1, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(chd1, chd2, chd3)
		{ }
	};

	class BinAnd : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class BinOr : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class BinXor : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class BinNot : public BitArithmetic {
	public:
		BinNot(Instruction_sptr chd1, Instruction_sptr chd2) : BitArithmetic(chd1, chd2, nullptr)
		{ }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};





	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all relational arithmetic instructions. */
	class RelationalArithmetic : public Instruction {
	public:
		RelationalArithmetic(Instruction_sptr chd1, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(chd1, chd2, chd3)
		{ }
	};

	class RLesThan : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class RLesThanEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class RGreaterThan : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class RGreaterThanEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class REqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class RNotEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Base class for all instructions that change control flow. */
	class BranchInstruction : public Instruction {
	protected:
		BasicBlock_sptr _lbl1;
		BasicBlock_sptr _lbl2;

	public:
		BranchInstruction(BasicBlock_sptr chd1) : Instruction(nullptr, nullptr, nullptr), _lbl1(chd1), _lbl2(nullptr)
		{ }

		BranchInstruction(Instruction_sptr exp, BasicBlock_sptr chd1, BasicBlock_sptr chd2) : Instruction(exp, nullptr, nullptr), _lbl1(chd1), _lbl2(chd2)
		{ }


		BasicBlock_sptr lbl1() { return this->_lbl1; }
		void lbl1(BasicBlock_sptr lbl) { this->_lbl1 = lbl; }

		BasicBlock_sptr lbl2() { return this->_lbl2; }
		void lbl2(BasicBlock_sptr lbl) { this->_lbl2 = lbl; }
	};

	class Jump : public BranchInstruction {
	public:
		Jump(BasicBlock_sptr tgt) : BranchInstruction(tgt)
		{ _lbl1->usageCounter()++; }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { 
			stringstream ss;
			ss << "BB" << this->_lbl1->id(); 
			return ss.str();
		}

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class Conditional : public BranchInstruction {
	public:
		Conditional(Instruction_sptr exp, BasicBlock_sptr chd1, BasicBlock_sptr chd2) : BranchInstruction(exp, chd1, chd2)
		{ _lbl1->usageCounter()++; _lbl2->usageCounter()++; }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { 
			stringstream ss;
			ss << "BB" << this->_lbl1->id() << " & BB" << this->_lbl2->id(); 
			return ss.str();
		}

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Represent taking the address of a variable. */
	class Addr : public Instruction {
	public:
		Addr(Instruction_sptr chd1, Instruction_sptr chd2) : Instruction(chd1, chd2, nullptr)
		{ }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	/**
	 * This class represents a displacement (\param src2) from a base address (\param src1). The final
	 * address is stored in (\param tgt)
	 */
	class AddrDispl : public Instruction {
	public:
		AddrDispl(Instruction_sptr chd1, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(chd1, chd2, chd3)
		{ }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};


	/* Represent the call to a function. */
	class Call : public Instruction {
	private:
		shared_ptr<vector<Instruction_sptr>> _arguments;

	public:
		Call(Instruction_sptr chd1, Instruction_sptr chd2) : Instruction(chd1, chd2, nullptr)
		{ }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		const shared_ptr<vector<Instruction_sptr>> arguments() const { return _arguments; }

		void addArgument(Instruction_sptr argument) {
			if (this->_arguments == nullptr)
				this->_arguments = make_shared<vector<Instruction_sptr>>();

			_arguments->push_back(argument);
		}

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	/* Represent a return instruction. */
	class Return : public Instruction {
	public:
		Return(Instruction_sptr exp) : Instruction(exp, nullptr, nullptr)
		{ 
			if (exp == nullptr) {
				cout << "Exp on return is nullptr." << endl;
			}
		}

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->chd1()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	/* Represent SSA phi functions. */
	class Phi : public Instruction {
	public:
		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Represent function definitions. */
	class Function {
	private:
		/* Pointer to the symbol table entry describing this function. */
		SymbolTableEntry_sp _addr;

		/* Pointer to this function's symbol table. */
		shared_ptr<SymbolTable> _symbTable;

		/* Keep track of how many times each name was used inside the
		 * same function so we can rename them.  				   */
		map<string, int> nameVersions;

		/* This tells if the last slot in the _instrs array has a label
		 * added to it. If it has, then the next instruction added to the
		 * array needs to go into that slot, if it has not, then the instruction
		 * will be added to a new slot. */
		bool _labelPendingSlot = false;

		/* The set of basic blocks composing this function */
		BasicBlock_list_sptr _bbs;

		/* Points to the current basic block being constructed. Used only during IR construction */
		BasicBlock_sptr _currentBasicBlock;

		/* The instructions that compose this function. */
		shared_ptr<list< pair<shared_ptr<STLabelDef>, shared_ptr<Instruction>> >> _subtrees;

	public:
		Function(shared_ptr<SymbolTable> st) :
			_symbTable(st),
			_subtrees(nullptr),
			_bbs(nullptr)
		{
			this->_bbs = make_shared< list<shared_ptr<BasicBlock>> >();
			this->_currentBasicBlock = make_shared<BasicBlock>(0);
			this->_bbs->push_back( this->_currentBasicBlock );
		}


		string name() { return dynamic_cast<STFunctionDeclaration*>(this->_addr.get())->getLabel(); }


		shared_ptr<list< pair<shared_ptr<STLabelDef>, shared_ptr<Instruction>> >> subtrees() { return this->_subtrees; }


		BasicBlock_list_sptr bbs() { return this->_bbs; };



		/* Methods related to Control Flow Graph construction. */
		/// ControlFlowGraph_sptr cfg();

		/* Methods related to setting/getting the function's declaration. */
		void addr(SymbolTableEntry_sp addr) { this->_addr = addr; }

		SymbolTableEntry_sp addr() { return this->_addr; }


		/* Methods related to Linear IR construction. */
		void appendLabel(shared_ptr<STLabelDef> label);
		void appendBasicBlock(BasicBlock_sptr bb);

		void appendInstruction(shared_ptr<IR::Instruction> instr);


		/* Symbol Table Management Methods. */
		void symbolTable(shared_ptr<SymbolTable> st) { this->_symbTable = st; }

		shared_ptr<SymbolTable> symbolTable() { return this->_symbTable; }

		void addSymbolTable(shared_ptr<SymbolTable> st);


		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		/* Mostly debug related methods. */
		void dump(stringstream& buffer);

		/* Dump all instructions to x86. Use a list sweep algorithm on the flat IR. */
		void linearDumpTox86(stringstream& buffer);
		void linearDumpTox86Prologue(stringstream& buffer);
		void linearDumpTox86Epilogue(stringstream& buffer);
		void linearDumpTox86MemAllocs();
	};



	/* Represent a whole compilation unit. */
	class Module {
	private:
		shared_ptr<list<shared_ptr<Function>>> _functions;

		shared_ptr<SymbolTable> _symbTable;

	public:
		Module() : _functions(shared_ptr<list<shared_ptr<Function>>>(new list<shared_ptr<Function>>())) { }

		void addFunction(shared_ptr<Function> fun) { this->_functions->push_back(fun); }

		Function_sptr_list_sptr functions() const { return this->_functions; }

		void symbolTable(shared_ptr<SymbolTable> st) { this->_symbTable = st; }
		shared_ptr<SymbolTable> symbolTable() { return this->_symbTable; }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor);

		void dump();

		/* Dump all instructions to x86. Use a list sweep algorithm on the flat IR. */
		void linearDumpTox86(stringstream& buffer);
	};

}

#endif /* IR_H_ */

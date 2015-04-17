#ifndef IR_H_
#define IR_H_

#include "Semantikin.h"
#include "ControlFlowGraph.h"
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

namespace IR {
	/* This class is the parent of all Three-address-code (TAC) IR instructions. */
	class Instruction {
	protected:
		shared_ptr<SymbolTableEntry> _tgt;
		shared_ptr<SymbolTableEntry> _src1;
		shared_ptr<SymbolTableEntry> _src2;

	public:
		Instruction(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : _tgt(tgt), _src1(src1), _src2(src2)
		{ }

		void tgt(shared_ptr<SymbolTableEntry> tgt) { this->_tgt = tgt; }
		shared_ptr<SymbolTableEntry> tgt() { return this->_tgt; }

		void src1(shared_ptr<SymbolTableEntry> src) { this->_src1 = src; }
		shared_ptr<SymbolTableEntry> src1() { return this->_src1; }

		void src2(shared_ptr<SymbolTableEntry> src) { this->_src2 = src; }
		shared_ptr<SymbolTableEntry> src2() { return this->_src2; }

		virtual const shared_ptr<vector<shared_ptr<SymbolTableEntry>>> arguments() { return nullptr; }
		virtual void addArgument(shared_ptr<SymbolTableEntry> argument) { }

		virtual void dump(stringstream& buffer) = 0;

		virtual void linearDumpTox86(stringstream& buffer) = 0;

		virtual ~Instruction() {};
	};



	/* Parent class of all data movement instructions. */
	class Copy : public Instruction {
	public:
		Copy(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1) : Instruction(tgt, src1, nullptr)
		{ }
	};

	class ScalarCopy : public Copy {
	public:
		ScalarCopy(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src) : Copy(tgt, src)
		{ }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class CopyFromArray : public Copy {
	public:
		CopyFromArray(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1) : Copy(tgt, src1)
		{ }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class CopyToArray : public Copy {
	public:
		CopyToArray(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1) : Copy(tgt, src1)
		{ }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Parent class of all integer arithmetic instructions. */
	class IntegerArithmetic : public Instruction {
	public:
		IntegerArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class BinaryIntegerArithmetic : public IntegerArithmetic {
	public:
		BinaryIntegerArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : IntegerArithmetic(tgt, src1, src2)
		{ }
	};

	class UnaryIntegerArithmetic : public IntegerArithmetic {
	public:
		UnaryIntegerArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src) : IntegerArithmetic(tgt, src, nullptr)
		{ }
	};

	class IAdd : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class ISub : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IMul : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IDiv : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IMod : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IMinus : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IInc : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class IDec : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Parent class of all floating point arithmetic instructions. */
	class FloatingArithmetic : public Instruction {
	public:
		FloatingArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class BinaryFloatingArithmetic : public FloatingArithmetic {
	public:
		BinaryFloatingArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : FloatingArithmetic(tgt, src1, src2)
		{ }
	};

	class UnaryFloatingArithmetic : public FloatingArithmetic {
	public:
		UnaryFloatingArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src) : FloatingArithmetic(tgt, src, nullptr)
		{ }
	};

	class FAdd : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FSub : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FMul : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FDiv : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FMinus : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FPlus : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FInc : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class FDec : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Parent class of all binary arithmetic instructions. */
	class BitArithmetic : public Instruction {
	public:
		BitArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class BinAnd : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class BinOr : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class BinXor : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class BinNot : public BitArithmetic {
	public:
		BinNot(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src) : BitArithmetic(tgt, src, nullptr)
		{ }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};






	/* Parent class of all relational arithmetic instructions. */
	class RelationalArithmetic : public Instruction {
	public:
		RelationalArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class RLesThan : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class RLesThanEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class RGreaterThan : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class RGreaterThanEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class REqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class RNotEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Base class for all instructions that change control flow. */
	class BranchInstruction : public Instruction {
	public:
		BranchInstruction(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class Jump : public BranchInstruction {
	public:
		Jump(shared_ptr<STLabelDef> tgt) : BranchInstruction(tgt, nullptr, nullptr)
		{ tgt->incrementUses(); }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class CondTrueJump : public BranchInstruction {
	public:
		CondTrueJump(shared_ptr<SymbolTableEntry> exp, shared_ptr<STLabelDef> tgt) : BranchInstruction(exp, tgt, nullptr)
		{ tgt->incrementUses(); }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	class CondFalseJump : public BranchInstruction {
	public:
		CondFalseJump(shared_ptr<SymbolTableEntry> exp, shared_ptr<STLabelDef> tgt) : BranchInstruction(exp, tgt, nullptr)
		{ tgt->incrementUses(); }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Represent taking the address of a variable. */
	class Addr : public Instruction {
	public:
		Addr(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src) : Instruction(tgt, src, nullptr)
		{ }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	/**
	 * This class represents a displacement (\param src2) from a base address (\param src1). The final
	 * address is stored in (\param tgt)
	 */
	class AddrDispl : public Instruction {
	public:
		AddrDispl(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Instruction(tgt, src1, src2)
		{ }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};


	/* Represent the call to a function. */
	class Call : public Instruction {
	private:
		shared_ptr<vector<shared_ptr<SymbolTableEntry>>> _arguments;

	public:
		Call(shared_ptr<SymbolTableEntry> callee, shared_ptr<SymbolTableEntry> resTgt) : Instruction(resTgt, callee, nullptr)
		{ }

		const shared_ptr<vector<shared_ptr<SymbolTableEntry>>> arguments() const { return _arguments; }

		void addArgument(shared_ptr<SymbolTableEntry> argument) {
			if (this->_arguments == nullptr)
				this->_arguments = make_shared<vector<shared_ptr<SymbolTableEntry>>>();

			_arguments->push_back(argument);
		}

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	/* Represent a return instruction. */
	class Return : public Instruction {
	public:
		Return(shared_ptr<SymbolTableEntry> exp) : Instruction(exp, nullptr, nullptr)
		{ }

		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};

	/* Represent SSA phi functions. */
	class Phi : public Instruction {
	public:
		void dump(stringstream& buffer);
		void linearDumpTox86(stringstream& buffer);
	};



	/* Represent function definitions. */
	class Function {
	private:
		/* Pointer to the symbol table entry describing this function. */
		shared_ptr<SymbolTableEntry> _addr;

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

		/* The instructions that compose this function. */
		shared_ptr<list< pair<shared_ptr<STLabelDef>, shared_ptr<Instruction>> >> _instrs;

	public:
		Function(shared_ptr<SymbolTable> st) :
			_symbTable(st),
			_instrs(shared_ptr<list<pair<shared_ptr<STLabelDef>, shared_ptr<Instruction>>>>(new list<pair<shared_ptr<STLabelDef>, shared_ptr<Instruction>>>()))
		{ }


		/* Methods related to Control Flow Graph construction. */
		ControlFlowGraph_sptr cfg();

		/* Methods related to setting/getting the function's declaration. */
		void addr(shared_ptr<SymbolTableEntry> addr) { this->_addr = addr; }

		shared_ptr<SymbolTableEntry> addr() { return this->_addr; }


		/* Methods related to Linear IR construction. */
		void appendLabel(shared_ptr<STLabelDef> label);

		void appendInstruction(shared_ptr<IR::Instruction> instr);


		/* Symbol Table Management Methods. */
		void symbolTable(shared_ptr<SymbolTable> st) { this->_symbTable = st; }

		shared_ptr<SymbolTable> symbolTable() { return this->_symbTable; }

		void addSymbolTable(shared_ptr<SymbolTable> st);


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

		void dump();

		/* Dump all instructions to x86. Use a list sweep algorithm on the flat IR. */
		void linearDumpTox86(stringstream& buffer);
	};

}

#endif /* IR_H_ */

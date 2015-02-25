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
	class Instruction : public RValue {
	protected:
		shared_ptr<LValue> _tgt;
		shared_ptr<RValue> _src1;
		shared_ptr<RValue> _src2;

	public:
		Instruction(shared_ptr<LValue> tgt, shared_ptr<RValue> src1, shared_ptr<RValue> src2) : _tgt(tgt), _src1(src1), _src2(src2)
		{ }

		void tgt(shared_ptr<LValue> tgt) { this->_tgt = tgt; }
		shared_ptr<LValue> tgt() { return this->_tgt; }

		void src1(shared_ptr<RValue> src) { this->_src1 = src; }
		shared_ptr<RValue> src1() { return this->_src1; }

		void src2(shared_ptr<RValue> src) { this->_src2 = src; }
		shared_ptr<RValue> src2() { return this->_src2; }

		virtual const shared_ptr<vector<shared_ptr<SymbolTableEntry>>> arguments() { return nullptr; }
		virtual void addArgument(shared_ptr<SymbolTableEntry> argument) { }

		virtual void dump(stringstream& buffer) = 0;

		virtual ~Instruction() {};
	};

	/* Parent class of all data movement instructions. */
	class Copy : public Instruction {
	public:
		Copy(shared_ptr<LValue> tgt, shared_ptr<RValue> src1) : Instruction(tgt, src1, nullptr)
		{ }
	};

	class ScalarCopy : public Copy {
	public:
		ScalarCopy(shared_ptr<LValue> tgt, shared_ptr<RValue> src) : Copy(tgt, src)
		{ }

		void dump(stringstream& buffer);
	};

	class CopyFromArray : public Copy {
	public:
		CopyFromArray(shared_ptr<LValue> tgt, shared_ptr<RValue> src1) : Copy(tgt, src1)
		{ }

		void dump(stringstream& buffer);
	};

	class CopyToArray : public Copy {
	public:
		CopyToArray(shared_ptr<LValue> tgt, shared_ptr<RValue> src1) : Copy(tgt, src1)
		{ }

		void dump(stringstream& buffer);
	};



	/* Parent class of all integer arithmetic instructions. */
	class IntegerArithmetic : public Instruction {
	public:
		IntegerArithmetic(shared_ptr<LValue> tgt, shared_ptr<RValue> src1, shared_ptr<RValue> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class BinaryIntegerArithmetic : public IntegerArithmetic {
	public:
		BinaryIntegerArithmetic(shared_ptr<LValue> tgt, shared_ptr<RValue> src1, shared_ptr<RValue> src2) : IntegerArithmetic(tgt, src1, src2)
		{ }
	};

	class UnaryIntegerArithmetic : public IntegerArithmetic {
	public:
		UnaryIntegerArithmetic(shared_ptr<LValue> tgt) : IntegerArithmetic(tgt, nullptr, nullptr)
		{ }
	};

	class IAdd : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};

	class ISub : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};

	class IMul : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};

	class IDiv : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};

	class IMod : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};

	class IMinus : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};

	class IPlus : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};

	class IInc : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};

	class IDec : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		void dump(stringstream& buffer);
	};



	/* Parent class of all floating point arithmetic instructions. */
	class FloatingArithmetic : public Instruction {
	public:
		FloatingArithmetic(shared_ptr<LValue> tgt, shared_ptr<RValue> src1, shared_ptr<RValue> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class BinaryFloatingArithmetic : public FloatingArithmetic {
	public:
		BinaryFloatingArithmetic(shared_ptr<LValue> tgt, shared_ptr<RValue> src1, shared_ptr<RValue> src2) : FloatingArithmetic(tgt, src1, src2)
		{ }
	};

	class UnaryFloatingArithmetic : public FloatingArithmetic {
	public:
		UnaryFloatingArithmetic(shared_ptr<LValue> tgt) : FloatingArithmetic(tgt, nullptr, nullptr)
		{ }
	};

	class FAdd : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		void dump(stringstream& buffer);
	};

	class FSub : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		void dump(stringstream& buffer);
	};

	class FMul : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		void dump(stringstream& buffer);
	};

	class FDiv : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		void dump(stringstream& buffer);
	};

	class FMinus : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		void dump(stringstream& buffer);
	};

	class FPlus : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		void dump(stringstream& buffer);
	};

	class FInc : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		void dump(stringstream& buffer);
	};

	class FDec : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		void dump(stringstream& buffer);
	};



	/* Parent class of all binary arithmetic instructions. */
	class BitArithmetic : public Instruction {
	public:
		BitArithmetic(shared_ptr<LValue> tgt, shared_ptr<RValue> src1, shared_ptr<RValue> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class BinAnd : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		void dump(stringstream& buffer);
	};

	class BinOr : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		void dump(stringstream& buffer);
	};

	class BinXor : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		void dump(stringstream& buffer);
	};

	class BinNot : public BitArithmetic {
	public:
		BinNot(shared_ptr<LValue> tgt, shared_ptr<RValue> src) : BitArithmetic(tgt, src, nullptr)
		{ }

		void dump(stringstream& buffer);
	};



	/* Parent class of all logical arithmetic instructions. */
	class LogicalArithmetic : public Instruction {
	public:
		LogicalArithmetic(shared_ptr<LValue> tgt, shared_ptr<RValue> src1, shared_ptr<RValue> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class LogAnd : public LogicalArithmetic {
	public:
		using LogicalArithmetic::LogicalArithmetic;

		void dump(stringstream& buffer);
	};

	class LogOr : public LogicalArithmetic {
	public:
		using LogicalArithmetic::LogicalArithmetic;

		void dump(stringstream& buffer);
	};

	class LogXor : public LogicalArithmetic {
	public:
		using LogicalArithmetic::LogicalArithmetic;

		void dump(stringstream& buffer);
	};

	class LogNot : public LogicalArithmetic {
	public:
		LogNot(shared_ptr<LValue> tgt, shared_ptr<RValue> src) : LogicalArithmetic(tgt, src, nullptr)
		{ }

		void dump(stringstream& buffer);
	};



	/* Parent class of all relational arithmetic instructions. */
	class RelationalArithmetic : public Instruction {
	public:
		RelationalArithmetic(shared_ptr<LValue> tgt, shared_ptr<RValue> src1, shared_ptr<RValue> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class RLesThan : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
	};

	class RLesThanEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
	};

	class RGreaterThan : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
	};

	class RGreaterThanEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
	};

	class REqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
	};

	class RNotEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		void dump(stringstream& buffer);
	};



	/* Base class for all instructions that change control flow. */
	class BranchInstruction : public Instruction {
	private:
		shared_ptr<STLabelDef> _target;
		shared_ptr<LValue> _cond;

	public:
		BranchInstruction(shared_ptr<LValue> cond, shared_ptr<STLabelDef> target) : Instruction(nullptr, nullptr, nullptr), _target(target), _cond(cond)
		{ }

		void target(shared_ptr<STLabelDef> target) { this->_target = target; }
		shared_ptr<STLabelDef> target() const { return this->_target; }

		void cond(shared_ptr<LValue> cond) { this->_cond = cond; }
		shared_ptr<LValue> cond() const { return this->_cond; }
	};

	class Jump : public BranchInstruction {
	public:
		Jump(shared_ptr<STLabelDef> tgt) : BranchInstruction(nullptr, tgt)
		{ tgt->incrementUses(); }

		void dump(stringstream& buffer);
	};

	class CondTrueJump : public BranchInstruction {
	public:
		CondTrueJump(shared_ptr<LValue> exp, shared_ptr<STLabelDef> tgt) : BranchInstruction(exp, tgt)
		{ tgt->incrementUses(); }

		void dump(stringstream& buffer);
	};

	class CondFalseJump : public BranchInstruction {
	public:
		CondFalseJump(shared_ptr<LValue> exp, shared_ptr<STLabelDef> tgt) : BranchInstruction(exp, tgt)
		{ tgt->incrementUses(); }

		void dump(stringstream& buffer);
	};



	/* Represent taking the address of a variable. */
	class Addr : public Instruction {
	public:
		Addr(shared_ptr<LValue> tgt, shared_ptr<RValue> src) : Instruction(tgt, src, nullptr)
		{ }

		void dump(stringstream& buffer);
	};

	/* Represent the call to a function. */
	class Call : public Instruction {
	private:
		shared_ptr<vector<shared_ptr<RValue>>> _arguments;
		shared_ptr<STFunctionDeclaration> _callee;

	public:
		Call(shared_ptr<STFunctionDeclaration> callee, shared_ptr<LValue> returnVar) : Instruction(returnVar, nullptr, nullptr), _callee(callee)
		{ }

		const shared_ptr<vector<shared_ptr<RValue>>> arguments() const { return _arguments; }

		void addArgument(shared_ptr<RValue> argument) {
			if (this->_arguments == nullptr)
				this->_arguments = make_shared<vector<shared_ptr<RValue>>>();

			_arguments->push_back(argument);
		}

		void dump(stringstream& buffer);
	};

	/* Represent a return instruction. */
	class Return : public Instruction {
	public:
		Return(shared_ptr<RValue> exp) : Instruction(nullptr, exp, nullptr)
		{ }

		void dump(stringstream& buffer);
	};

	/* Represent SSA phi functions. */
	class Phi : public Instruction {
	public:
		void dump(stringstream& buffer);
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
	};

}

#endif /* IR_H_ */

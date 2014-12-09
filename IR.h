#ifndef IR_H_
#define IR_H_

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

		virtual void dump(stringstream& buffer) = 0;

		virtual ~Instruction() {};
	};

	/* Parent class of all data movement instructions. */
	class Copy : public Instruction {
	public:
		Copy(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Instruction(tgt, src1, src2)
		{ }
	};


	class ScalarCopy : public Copy {
	public:
		ScalarCopy(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src) : Copy(tgt, src, nullptr)
		{ }

		void dump(stringstream& buffer);
	};


	class CopyFromArray : public Copy {
	public:
		CopyFromArray(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Copy(tgt, src1, src2)
		{ }

		void dump(stringstream& buffer);
	};


	class CopyToArray : public Copy {
	public:
		CopyToArray(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Copy(tgt, src1, src2)
		{ }

		void dump(stringstream& buffer);
	};


	/* Parent class of all integer arithmetic instructions. */
	class IntegerArithmetic : public Instruction {
	public:
		IntegerArithmetic(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : Instruction(tgt, src1, src2)
		{ }
	};

	class IAdd : public IntegerArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class ISub : public IntegerArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class IMul : public IntegerArithmetic {
	public:
		IMul(shared_ptr<SymbolTableEntry> tgt, shared_ptr<SymbolTableEntry> src1, shared_ptr<SymbolTableEntry> src2) : IntegerArithmetic(tgt, src1, src2)
		{ }

		void dump(stringstream& buffer);
	};

	class IDiv : public IntegerArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class IMod : public IntegerArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class IInc : public IntegerArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class IDec : public IntegerArithmetic {
	public:
		void dump(stringstream& buffer);
	};


	/* Parent class of all floating point arithmetic instructions. */
	class FloatingArithmetic : public Instruction { };

	class FAdd : public FloatingArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class FSub : public FloatingArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class FMul : public FloatingArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class FDiv : public FloatingArithmetic {
	public:
		void dump(stringstream& buffer);
	};


	/* Parent class of all binary arithmetic instructions. */
	class BinaryArithmetic : public Instruction { };

	class BinAnd : public BinaryArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class BinOr : public BinaryArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class BinXor : public BinaryArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class BinNot : public BinaryArithmetic {
	public:
		void dump(stringstream& buffer);
	};


	/* Parent class of all logical arithmetic instructions. */
	class LogicalArithmetic : public Instruction { };

	class LogAnd : public LogicalArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class LogOr : public LogicalArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class LogXor : public LogicalArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class LogNot : public LogicalArithmetic {
	public:
		void dump(stringstream& buffer);
	};



	/* Parent class of all relational arithmetic instructions. */
	class RelationalArithmetic : public Instruction { };

	class RLesThan : public RelationalArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class RLesThanEqual : public RelationalArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class RGreaterThan : public RelationalArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class RGreaterThanEqual : public RelationalArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class REqual : public RelationalArithmetic {
	public:
		void dump(stringstream& buffer);
	};

	class RNotEqual : public RelationalArithmetic {
	public:
		void dump(stringstream& buffer);
	};


	/* Base class for all instructions that change control flow. */
	class BranchInstruction : public Instruction { };

	class Jump : public BranchInstruction {
	public:
		void dump(stringstream& buffer);
	};

	class CondTrueJump : public BranchInstruction {
	public:
		void dump(stringstream& buffer);
	};

	class CondFalseJump : public BranchInstruction {
	public:
		void dump(stringstream& buffer);
	};


	/* Represent the call to a function. */
	class Call : public Instruction {
	public:
		void dump(stringstream& buffer);
	};

	/* Represent a return instruction. */
	class Return : public Instruction {
	public:
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
		shared_ptr<SymbolTableEntry> _addr;

		shared_ptr<SymbolTable> _symbTable;

		/* Keep track of how many times each name was used inside the
		 * same function so we can rename them.  				   */
		map<string, int> nameVersions;

		/* The instructions that compose this function. */
		shared_ptr<list<shared_ptr<Instruction>>> _instrs;

	public:
		Function(shared_ptr<SymbolTable> st) : _symbTable(st), _instrs(shared_ptr<list<shared_ptr<Instruction>>>(new list<shared_ptr<Instruction>>())) { }

		void addr(shared_ptr<SymbolTableEntry> addr) { this->_addr = addr; }
		shared_ptr<SymbolTableEntry> addr() { return this->_addr; }

		void appendInstruction(shared_ptr<IR::Instruction> instr) { this->_instrs->push_back(instr); }

		void symbolTable(shared_ptr<SymbolTable> st) { this->_symbTable = st; }
		shared_ptr<SymbolTable> symbolTable() { return this->_symbTable; }
		void addSymbolTable(shared_ptr<SymbolTable> st);

		void dump(stringstream& buffer);
	};



	/* Represent a whole compilation unit. */
	class Module {
	private:
		shared_ptr<list<shared_ptr<Function>>> functions;

		shared_ptr<SymbolTable> _symbTable;

	public:
		Module() : functions(shared_ptr<list<shared_ptr<Function>>>(new list<shared_ptr<Function>>())) { }

		void addFunction(shared_ptr<Function> fun) { this->functions->push_back(fun); }

		void symbolTable(shared_ptr<SymbolTable> st) { this->_symbTable = st; }
		shared_ptr<SymbolTable> symbolTable() { return this->_symbTable; }

		void dump();
	};

}

#endif /* IR_H_ */

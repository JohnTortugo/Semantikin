#ifndef IR_H_
#define IR_H_

#include "Semantikin.h"
#include "AbstractSyntaxTree.h"
#include "IRVisitors.h"

#include <string>
#include <list>
#include <memory>
#include <sstream>
#include <iomanip>
#include <map>
#include <iterator>
#include <cstddef>

using std::stringstream;
using std::string;
using std::list;
using std::map;
using std::shared_ptr;
using std::make_shared;
using namespace Parser;


namespace IR {
	bool isADefinition(Instruction_sptr instr);
	SymbolTableEntry_sp whatIsDefined(Instruction_sptr instr) ;



	/* This class is the parent of all IR instructions. */
	class Instruction : public std::enable_shared_from_this<Instruction> {
	protected:
		// This is only used when dumping the IR to a visual format
		string _fillColor = "ffffff";
		unsigned int _myColor = 0;

		Instruction_sptr _tgt = nullptr;
		Instruction_sptr _chd2 = nullptr;
		Instruction_sptr _chd3 = nullptr;

		/* Pointer to next operation (i.e., really an instruction) 
		 * that should be executed */
		Instruction* _next = nullptr;

	public:
		Instruction()
		{ }

		Instruction(Instruction_sptr tgt, Instruction_sptr chd2=nullptr, Instruction_sptr chd3=nullptr) :
			_tgt(tgt), _chd2(chd2), _chd3(chd3), _next(nullptr)
		{ }

		// Set/get the color of this IR node. Used for graphical visualization of the IR tree
		void myColor(unsigned int mc) { this->_myColor = mc; }
		unsigned int myColor() { return this->_myColor; }

		void fillColor(const string& fc) { this->_fillColor = fc; }
		const string& fillColor() { return this->_fillColor; }

		// Used to Get/set the operands of the instruction
		void tgt(Instruction_sptr chd) { this->_tgt = chd; }
		Instruction_sptr tgt() { return this->_tgt; }

		void chd2(Instruction_sptr chd) { this->_chd2 = chd; }
		Instruction_sptr chd2() { return this->_chd2; }

		void chd3(Instruction_sptr chd) { this->_chd3 = chd; }
		Instruction_sptr chd3() { return this->_chd3; }

		virtual void next(Instruction* nxt) { this->_next = nxt; }
		virtual Instruction* next() { return this->_next; }



		virtual const shared_ptr<vector<SymbolTableEntry_sp>> arguments() { return nullptr; }
		virtual void addArgument(SymbolTableEntry_sp argument) { }

		/** Used to traverse the IR tree. */
		virtual void accept(IRTreeVisitor* visitor) = 0;

		/** Used to dump in a "human readable" way the instruction's target operand */
		virtual string tgtDataName() = 0;

		virtual void dump(stringstream& buffer) = 0;

		virtual ~Instruction() {};
	};


	/* Base class for all instructions that change control flow. */
	class BranchInstruction : public Instruction {
	protected:
		BasicBlock_sptr _lbl1;
		BasicBlock_sptr _lbl2;

	public:
		BranchInstruction(BasicBlock_sptr lbl) : Instruction(nullptr, nullptr, nullptr), _lbl1(lbl), _lbl2(nullptr)
		{ }

		BranchInstruction(Instruction_sptr exp, BasicBlock_sptr lbl1, BasicBlock_sptr lbl2) : Instruction(exp, nullptr, nullptr), _lbl1(lbl1), _lbl2(lbl2)
		{ }


		BasicBlock_sptr lbl1() { return this->_lbl1; }
		void lbl1(BasicBlock_sptr lbl) { this->_lbl1 = lbl; }

		BasicBlock_sptr lbl2() { return this->_lbl2; }
		void lbl2(BasicBlock_sptr lbl) { this->_lbl2 = lbl; }
	};




	class InstructionIterator : public std::iterator<std::bidirectional_iterator_tag, IR::Instruction> {
	private:
		Instruction* _current = nullptr;

	public:
		InstructionIterator(Instruction* head) : _current(head) {
		}

		InstructionIterator operator=(const InstructionIterator& other) {
			this->_current = other.current();
		}

		Instruction* current() const { 
			return this->_current; 
		}



		InstructionIterator& operator++() {
			this->_current = this->current()->next();
			return *this;
		}

		InstructionIterator operator++(int) {
			InstructionIterator clone(*this);
			this->_current = this->current()->next();
			return clone;
		}

		bool operator==(const InstructionIterator& other) {
			return this->current() == other.current();
		}

		bool operator!=(const InstructionIterator& other) {
			return !(*this == other);
		}

		Instruction& operator*() {
			return *this->current();
		}

		Instruction& operator->() {
			return *this->current();
		}
	};

	class InstructionSequence {
	private:
		Instruction* _head;		

	public:
		typedef InstructionIterator iterator;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Instruction value_type;
		typedef Instruction* pointer;
		typedef Instruction& reference;

		InstructionSequence(Instruction* head) {
			this->_head = head;
		}

		iterator begin() { return iterator(this->_head); }

		iterator end() { return iterator(nullptr); }
	};

	/** Well, this represents a basic block =) */
	class BasicBlock : public std::enable_shared_from_this<BasicBlock> {
	private:
		int _id;
		int _usageCounter;

		Instruction_list_sptr _subtrees;

		Instruction* _firstInstruction;

		BasicBlock_list_sptr _preds;
		BasicBlock_list_sptr _succs;

	public:
		BasicBlock(int id) :
			_id(id),
			_usageCounter(0),
			_subtrees(  make_shared<Instruction_list>() ),
			_preds( make_shared<BasicBlock_list>() ),
			_succs( make_shared<BasicBlock_list>() )
		{ 
		}

		int id() const { return _id; }
		void id(int id) { _id = id; }

		int& usageCounter() { return _usageCounter; }
		void usageCounter(int uc) { _usageCounter = uc; }

		Instruction_list_sptr subtrees() const { return _subtrees; }

		void firstInstruction(Instruction* first) { this->_firstInstruction = first; }

		InstructionSequence instructions() { 
			return InstructionSequence(this->_firstInstruction); 
		}

		BasicBlock_list_sptr succs() {
			return this->_succs;
		}

		BasicBlock_list_sptr preds() {
			return this->_preds;
		}

		void addPredecessor(BasicBlock_sptr pred) {
			this->_preds->push_back(pred);
		}	

		void appendInstruction(Instruction_sptr instr) {
			this->_subtrees->push_back( instr );
			auto branch = std::dynamic_pointer_cast<IR::BranchInstruction>(instr);

			// If the instruction is a branch we make some checkings and add 
			// edges of the CFG.
			if (branch != nullptr) {
				if (this->succs() != nullptr && this->succs()->size() > 0) {
					cout << "Error: appending a *second* branch instruction in a basic block." << endl;
					exit(-1);
				}

				// all kinds of branches have the first label
				this->succs()->push_back(branch->lbl1());

				// the target basic block receive a predecessor
				branch->lbl1()->addPredecessor(this->shared_from_this());

				// When the branch is a goto it does not have the second label
				if (branch->lbl2() != nullptr) {
					this->succs()->push_back(branch->lbl2());
					branch->lbl2()->addPredecessor(this->shared_from_this());
				}	
			}
		}

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dumpToDot(stringstream& buffer);
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

		/* The set of basic blocks composing this function */
		BasicBlock_list_sptr _bbs;

		/* Points to the current basic block being constructed. Used *only* during 
		 * IR construction */
		BasicBlock_sptr _currentBasicBlock;

		/* This points to the function's entry basic block. */
		BasicBlock_sptr _entryBasicBlock;

		BasicBlock_list_sptr topologicalSort();

	public:
		// This will be used to specify the order in which a client wants to
		// traverse the nodes of the CFG.
		enum class CFGBasicBlockOrder {
			TOPO_ORDER,
			REVERSE_TOPO_ORDER
		};


		Function(shared_ptr<SymbolTable> st) :
			_symbTable(st),
			_bbs(nullptr)
		{
			this->_bbs = make_shared< list<shared_ptr<BasicBlock>> >();
			this->_currentBasicBlock = make_shared<BasicBlock>(0);
			this->_entryBasicBlock = this->_currentBasicBlock;
			this->_bbs->push_back( this->_currentBasicBlock );
		}


		string name() { return dynamic_cast<STFunctionDeclaration*>(this->_addr.get())->getLabel(); }

		// No order specified. Just return the order of the internal structure.
		BasicBlock_list_sptr bbs() { 
			return this->_bbs; 
		};

		BasicBlock_list_sptr bbs(CFGBasicBlockOrder order) {
			if (order == CFGBasicBlockOrder::TOPO_ORDER) {
				return topologicalSort();
			}
			else {
				cout << "CRITICAL: Invalid CFG node order specified." << endl;
				exit(1);
			}
		}


		/* Methods related to setting/getting the function's declaration. */
		void addr(SymbolTableEntry_sp addr) { this->_addr = addr; }

		SymbolTableEntry_sp addr() { return this->_addr; }


		/* Methods related to IR construction. */
		void appendBasicBlock(BasicBlock_sptr bb);
		void appendInstruction(shared_ptr<IR::Instruction> instr);


		/* Symbol Table Management Methods. */
		void symbolTable(shared_ptr<SymbolTable> st) { this->_symbTable = st; }

		shared_ptr<SymbolTable> symbolTable() { return this->_symbTable; }

		void addSymbolTable(shared_ptr<SymbolTable> st);


		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		/* Mostly debug related methods. */
		void dump(stringstream& output)  {
			output << endl << "Code for function: " << this->name() << endl;

			for (auto& bb : *this->bbs()) {
				output << "BB" << bb->id() << ": " << endl;
				
				for (auto& instr : bb->instructions()) {
					instr.dump(output);
				}

				output << endl;
			}
		}
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
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump() {
			std::stringstream buffer;

			buffer << "This is the IR: " << endl;
			buffer << std::setfill('-') << std::setw(80) << "-" << endl;

			for (auto function : *this->_functions) {
				function->dump(buffer);
				buffer << endl << endl;
			}

			cout << buffer.str();
		}
	};








	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all instructions that are actually
	 * placeholder for data. Like immediates, registers and
	 * memory addresses. 									
	 */
	class Data : public Instruction {
	public:
		void next(Instruction* nxt) { 
			cout << "IR Error: Operands should not have next ptrs." << endl; 
		}

		Instruction* next() { 
			cout << "IR Error: Operands do not have next ptrs." << endl; 
			return this->_next; 
		}

		virtual SymbolTableEntry_sp value() = 0;
	};

	class Immediate : public Data {
	private:
		STConstantDef_sptr _value;

	public:
		Immediate(SymbolTableEntry_sp val) : _value( std::dynamic_pointer_cast<STConstantDef>(val) )
		{ }

		Immediate(STConstantDef_sptr val) : _value(val)
		{ }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		SymbolTableEntry_sp value() { return this->_value; }

		string tgtDataName() { return Util::escapeStr(this->_value->getName()); }

		void dump(stringstream& buffer) {
			buffer << "Imm;" << endl;
		}
	};

	class Register : public Data {
	private:
		STRegister_sptr _value;

	public:
		Register(SymbolTableEntry_sp val) : _value( std::dynamic_pointer_cast<STRegister>(val) )
		{ }

		Register(STRegister_sptr val) : _value(val)
		{ }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		SymbolTableEntry_sp value() { return this->_value; }

		string tgtDataName() { return this->_value->getName(); }

		void dump(stringstream& buffer) { buffer << "Reg;" << endl; }
	};

	class Memory : public Data {
	private:
		STVariableDeclaration_sptr _value;

	public:
		Memory(SymbolTableEntry_sp val) : _value( std::dynamic_pointer_cast<STVariableDeclaration>(val) )
		{ }

		Memory(STVariableDeclaration_sptr val) : _value(val)
		{ }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		SymbolTableEntry_sp value() { return this->_value; }

		string tgtDataName() { return this->_value->getName(); }

		void dump(stringstream& buffer) {
			buffer << "Mem;" << endl;
		}

	};

	class Func : public Data {
	private:
		STFunctionDecl_sptr  _value;

	public:
		Func(SymbolTableEntry_sp val) : _value( std::dynamic_pointer_cast<STFunctionDeclaration>(val) )
		{ }

		Func(STFunctionDecl_sptr val) : _value(val)
		{ }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		SymbolTableEntry_sp value() { return this->_value; }

		string tgtDataName() { return this->_value->getName(); }

		void dump(stringstream& buffer) {
			buffer << "Func;" << endl;
		}
	};







	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all data movement instructions. 		*/
	class Copy : public Instruction {
	public:
		Copy(Instruction_sptr tgt, Instruction_sptr value) : Instruction(tgt, value, nullptr) {
			assert(tgt != nullptr && value != nullptr && "A parameter to IR::Copy is null.\n");
		}
	};

	class ScalarCopy : public Copy {
	public:
		using Copy::Copy;

		/** Used to obtain the "human readable" name of the destination operand of the operation. */
		string tgtDataName() { return this->_tgt->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << ";" << endl;
		}
	};

	class CopyFromArray : public Copy {
		using Copy::Copy;

		/** Used to obtain the "human readable" name of the destination operand of the operation. */
		string tgtDataName() { return this->_tgt->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = *" << this->_chd2->tgtDataName() << ";" << endl;
		}
	};

	class CopyToArray : public Copy {
		using Copy::Copy;

		/** Used to obtain the "human readable" name of the destination operand of the operation. */
		string tgtDataName() { return this->_tgt->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << "*" << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << ";" << endl;
		}
	};




	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all integer arithmetic instructions. */
	class IntegerArithmetic : public Instruction {
	public:
		IntegerArithmetic(Instruction_sptr tgt, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(tgt, chd2, chd3)
		{ }
	};

	class BinaryIntegerArithmetic : public IntegerArithmetic {
	public:
		BinaryIntegerArithmetic(Instruction_sptr tgt, Instruction_sptr chd2, Instruction_sptr chd3) : IntegerArithmetic(tgt, chd2, chd3)
		{ }
	};

	class UnaryIntegerArithmetic : public IntegerArithmetic {
	public:
		UnaryIntegerArithmetic(Instruction_sptr tgt, Instruction_sptr chd2) 
			: IntegerArithmetic(tgt, chd2, nullptr) { }
	};

	class IAdd : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " + " << this->_chd3->tgtDataName() << ";" << endl;
		}
	};

	class ISub : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " - " << this->_chd3->tgtDataName() << ";" << endl;
		}
	};

	class IMul : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " * " << this->_chd3->tgtDataName() << ";" << endl;
		}
	};

	class IDiv : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " / " << this->_chd3->tgtDataName() << ";" << endl;
		}
	};

	class IMod : public BinaryIntegerArithmetic {
	public:
		using BinaryIntegerArithmetic::BinaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " % " << this->_chd3->tgtDataName() << ";" << endl;
		}
	};

	class IMinus : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = -" << this->_chd2->tgtDataName() << ";" << endl;
		}
	};

	class IInc : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " + 1;" << endl;
		}
	};

	class IDec : public UnaryIntegerArithmetic {
	public:
		using UnaryIntegerArithmetic::UnaryIntegerArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->_chd2->tgtDataName() << " - 1;" << endl;
		}
	};





	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all floating point arithmetic instructions. */
	class FloatingArithmetic : public Instruction {
	public:
		FloatingArithmetic(Instruction_sptr tgt, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(tgt, chd2, chd3)
		{ }
	};

	class BinaryFloatingArithmetic : public FloatingArithmetic {
	public:
		BinaryFloatingArithmetic(Instruction_sptr tgt, Instruction_sptr chd2, Instruction_sptr chd3) : FloatingArithmetic(tgt, chd2, chd3)
		{ }
	};

	class UnaryFloatingArithmetic : public FloatingArithmetic {
	public:
		UnaryFloatingArithmetic(Instruction_sptr tgt, Instruction_sptr chd2) : FloatingArithmetic(tgt, chd2, nullptr)
		{ }
	};

	class FAdd : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " + " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class FSub : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " - " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class FMul : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " * " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class FDiv : public BinaryFloatingArithmetic {
	public:
		using BinaryFloatingArithmetic::BinaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " / " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class FMinus : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = -" << this->chd2()->tgtDataName() << ";" << endl;
		}
	};

	class FInc : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " + 1;" << endl;
		}
	};

	class FDec : public UnaryFloatingArithmetic {
	public:
		using UnaryFloatingArithmetic::UnaryFloatingArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " - 1;" << endl;
		}
	};



	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all binary arithmetic instructions.  */
	class BitArithmetic : public Instruction {
	public:
		BitArithmetic(Instruction_sptr tgt, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(tgt, chd2, chd3)
		{ }
	};

	class BinAnd : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " & " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class BinOr : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " | " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class BinXor : public BitArithmetic {
	public:
		using BitArithmetic::BitArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " ^ " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class BinNot : public BitArithmetic {
	public:
		BinNot(Instruction_sptr tgt, Instruction_sptr chd2) : BitArithmetic(tgt, chd2, nullptr)
		{ }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = ~ " << this->chd2()->tgtDataName() << ";" << endl;
		}
	};





	/********************************************************/
	/********************************************************/
	/********************************************************/
	/********************************************************/
	/* Parent class of all relational arithmetic instructions. */
	class RelationalArithmetic : public Instruction {
	public:
		RelationalArithmetic(Instruction_sptr tgt, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(tgt, chd2, chd3)
		{ }
	};

	class RLesThan : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->chd2()->tgtDataName() << " < " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class RLesThanEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->chd2()->tgtDataName() << " <= " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class RGreaterThan : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->chd2()->tgtDataName() << " > " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class RGreaterThanEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->chd2()->tgtDataName() << " >= " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class REqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->chd2()->tgtDataName() << " == " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};

	class RNotEqual : public RelationalArithmetic {
	public:
		using RelationalArithmetic::RelationalArithmetic;

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->_tgt->tgtDataName() << " = " << this->chd2()->tgtDataName() << " != " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};



	class Jump : public BranchInstruction {
	public:
		Jump(BasicBlock_sptr lbl1) : BranchInstruction(lbl1)
		{ _lbl1->usageCounter()++; }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { 
			stringstream ss;
			ss << "BB" << this->_lbl1->id(); 
			return ss.str();
		}

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << "goto BB" << this->lbl1()->id() << ";" << endl;
		}
	};

	class Conditional : public BranchInstruction {
	public:
		Conditional(Instruction_sptr exp, BasicBlock_sptr lbl1, BasicBlock_sptr lbl2) : BranchInstruction(exp, lbl1, lbl2)
		{ _lbl1->usageCounter()++; _lbl2->usageCounter()++; }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { 
			stringstream ss;
			ss << "BB" << this->_lbl1->id() << " & BB" << this->_lbl2->id(); 
			return ss.str();
		}

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << "if " << this->tgt()->tgtDataName() << " goto BB" << this->lbl1()->id() << " else goto BB" << this->lbl2()->id() << ";" << endl;
		}
	};



	/* Represent taking the address of a variable. */
	class Addr : public Instruction {
	public:
		Addr(Instruction_sptr tgt, Instruction_sptr lbl2) : Instruction(tgt, lbl2, nullptr)
		{ }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = &" << this->chd2()->tgtDataName() << ";" << endl;
		}
	};

	/**
	 * This class represents a displacement (\param src2) from a base address (\param src1). The final
	 * address is stored in (\param tgt)
	 */
	class AddrDispl : public Instruction {
	public:
		AddrDispl(Instruction_sptr tgt, Instruction_sptr chd2, Instruction_sptr chd3) : Instruction(tgt, chd2, chd3)
		{ }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << " ++ " << this->chd3()->tgtDataName() << ";" << endl;
		}
	};


	/* Represent the call to a function. */
	class Call : public Instruction {
	private:
		shared_ptr<vector<Instruction_sptr>> _arguments;

	public:
		Call(Instruction_sptr tgt, Instruction_sptr chd2) : Instruction(tgt, chd2, nullptr)
		{ }

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		const shared_ptr<vector<Instruction_sptr>> arguments() const { return _arguments; }

		void addArgument(Instruction_sptr argument) {
			if (this->_arguments == nullptr)
				this->_arguments = make_shared<vector<Instruction_sptr>>();

			_arguments->push_back(argument);
		}

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer)  {
			if (this->tgt() != nullptr)
				buffer << this->tgt()->tgtDataName() << " = " << this->chd2()->tgtDataName() << "(";
			else
				buffer << this->chd2()->tgtDataName() << "(";

			auto arguments = this->arguments();

			if (arguments != nullptr) {
				if (arguments->size() > 0)
					buffer << (*arguments)[0]->tgtDataName();

				for (int i=1; i<arguments->size(); i++)
					buffer << ", " << (*arguments)[i]->tgtDataName();
			}

			buffer << ");" << endl;
		}
	};

	/* Represent a return instruction. */
	class Return : public Instruction {
	public:
		Return(Instruction_sptr exp) : Instruction(exp, nullptr, nullptr) { 
			assert(exp && "Exp on IR::Return is nullptr.");
		}

		/** Used to dump in a "human readable" way the instruction's target operand */
		string tgtDataName() { return this->tgt()->tgtDataName(); }

		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) {
			buffer << "ret " << this->tgt()->tgtDataName() << ";" << endl;
		}
	};

	/* Represent SSA phi functions. */
	class Phi : public Instruction {
	public:
		/* Used to traverse the IR tree. */
		void accept(IRTreeVisitor* visitor) { visitor->visit(this); }

		void dump(stringstream& buffer) { 
			buffer << "phi();" << endl; 
		}
	};





}

#endif /* IR_H_ */

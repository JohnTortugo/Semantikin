#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_
#include "Semantikin.h"

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

namespace Parser {
	enum NativeType {
		NOT_A_TYPE,
		VOID,
		INT,
		FLOAT,
		STRING
	};

	enum TypeWidth {
		VOID_WIDTH = 0,
		INT_WIDTH = 4,
		FLOAT_WIDTH = 8,
		STRING_WIDTH = 4
	};

	class SymbolTableEntry {
	protected:
		/* Name the user gave to the variable/function. */
		string name;

		/* the variable's type. */
		NativeType _type;

	public:
		SymbolTableEntry() : name(""), _type(Parser::NOT_A_TYPE) { }

		SymbolTableEntry(string nm, NativeType tp) : name(nm), _type(tp) {}

		virtual string getName() { return this->name; }

		NativeType type() const { return this->_type; }
		void type(NativeType tp) { this->_type = tp; }

		void rename(string newName) { this->name = newName; }

		virtual void dump() const = 0;

		virtual ~SymbolTableEntry() {};
	};

	/* This is a base class for all r-values: instructions(!),
	 * variables and contants(!). */
	class RValue {
	public:
		virtual string getResName() = 0;

		virtual ~RValue() {}
	};

	/* Represent any kind of variable: parameter, temporary, local. */
	class LValue : public SymbolTableEntry, public RValue {
	protected:
		/* How many bytes this variable occupy. */
		int width;

		/* Offset within the function frame where this variable will be stored. */
		int offset;

		/* Is this variable scalar (dims.size() == 0) or a vector with N dimensions
		 * (dims.size() == N)? If it is a vector, each index store the size of each
		 * respective dimension. 												 */
		vector<int> _dims;


	public:
		LValue() : SymbolTableEntry("", Parser::NOT_A_TYPE), width(0), offset(0)
		{ }

		LValue(string nm, NativeType tp, int wd, int off) : SymbolTableEntry(nm, tp), width(wd), offset(off)
		{ }

		const vector<int>& dims() const { return _dims; }
		int getNumDims() const { return _dims.size(); }
		void addDim(int size) { _dims.push_back(size); }

		string getResName() { return this->name; }

		int getOffset() const {
			return offset;
		}

		int getWidth() const {
			return width;
		}
	};


	/* This represent a local variable declaration. */
	class STLocalVarDecl : public LValue {
	public:
		STLocalVarDecl(string nm, NativeType tp, int wd, int off) : LValue(nm, tp, wd, off)
		{ }

		void dump() const;
	};


	/* This represent a formal parameter declaration in a function. */
	class STParamDecl : public LValue {
	public:
		STParamDecl(string nm, NativeType tp, int wd, int off) : LValue(nm, tp, wd, off)
		{ }

		void dump() const;
	};


	/* This entry will represent a temporary variable used in the IR. */
	class STTempVar : public LValue {
	public:
		STTempVar(string nm, NativeType tp, int wd, int off) : LValue(nm, tp, wd, off)
		{ }

		void dump() const;
	};


	/* This entry will represent a address label used in the IR. */
	class STLabelDef : public SymbolTableEntry {
	private:
		int _address;
		int _usageCounter;

	public:
		STLabelDef(string nm) : SymbolTableEntry(nm, Parser::NOT_A_TYPE), _address(0), _usageCounter(0)
		{ }

		void address(int addr) { this->_address = addr; }
		int address() const { return this->_address; }

		int usageCounter() const { return this->_usageCounter; }
		void incrementUses() { this->_usageCounter++; }

		void dump() const;
	};


	/* This is used to store the constants when representing the IR. */
	class STConstantDef : public RValue, public SymbolTableEntry {
	private:
		/* TODO: make these fields part of a union */
		int _integer;
		float _floating;
		string _str;

	public:
		/* Didn't like this too much, but I would have to overload
		 * it in the union anyway, right? 						*/
		STConstantDef(string nm, string s) : SymbolTableEntry(nm, Parser::STRING), _str(s), _integer(0), _floating(0) {}
		STConstantDef(string nm, int i)    : SymbolTableEntry(nm, Parser::INT), _str(""), _integer(i), _floating(0) {}
		STConstantDef(string nm, float f)  : SymbolTableEntry(nm, Parser::FLOAT), _str(""), _integer(0), _floating(f) {}

		string getName() {
			switch (this->_type) {
				case Parser::STRING: return this->_str;
				case Parser::INT: return std::to_string(this->_integer);
				case Parser::FLOAT: return std::to_string(this->_floating);
				case Parser::NOT_A_TYPE: throw System::EXCEPTION_UNEXPECTED_TYPE;
			}
		}

		string getResName() { return this->getName(); }

		void dump() const;
	};

	/* Represents the definition of a function. */
	class STFunctionDeclaration : public SymbolTableEntry {
	private:
		/* Compiler assigned function's label. */
		string label;

		vector<shared_ptr<SymbolTableEntry>> _params;

	public:
		STFunctionDeclaration(string nm, string lbl, NativeType rtp) : SymbolTableEntry(nm, rtp), label(lbl)
		{ }

		string getLabel() const { return label; }

		const vector<shared_ptr<SymbolTableEntry>>& params() const { return _params; }
		void addParam(shared_ptr<SymbolTableEntry> newParam) { _params.push_back(newParam); }

		void dump() const;
	};


	class SymbolTable {
	private:
		map<string, shared_ptr<SymbolTableEntry>> _entries;
		shared_ptr<SymbolTable> parent;

	public:
		SymbolTable(shared_ptr<SymbolTable> _parent) : parent(_parent) {}

		shared_ptr<SymbolTable> getParent() const { return this->parent; }

		shared_ptr<SymbolTableEntry> lookup(string name);

		bool add(shared_ptr<SymbolTableEntry> _entry);

		map<string, shared_ptr<SymbolTableEntry>> entries() { return this->_entries; }

		void dump() const ;
	};

}

#endif /* SYMBOLTABLE_H_ */

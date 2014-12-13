#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

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

	public:
		SymbolTableEntry() { }

		SymbolTableEntry(string nm) : name(nm) {}

		virtual string getName() { return this->name; }

		void rename(string newName) { this->name = newName; }

		virtual void dump() const = 0;

		virtual ~SymbolTableEntry() {};
	};

	/* Represent any kind of variable: parameter, temporary, local. */
	class STVariableDeclaration : public SymbolTableEntry {
	protected:
		/* the variable's type. */
		NativeType type;

		/* How many bytes this variable occupy. */
		int width;

		/* Offset within the function frame where this variable will be stored. */
		int offset;

		/* Is this variable scalar (dims.size() == 0) or a vector with N dimensions
		 * (dims.size() == N)? If it is a vector, each index store the size of each
		 * respective dimension. 												 */
		vector<int> _dims;


	public:
		STVariableDeclaration() : type(Parser::VOID), width(0), offset(0) { }

		STVariableDeclaration(string nm, NativeType tp, int wd, int off) : SymbolTableEntry(nm), type(tp), width(wd), offset(off)
		{ }

		const vector<int>& dims() const { return _dims; }
		int getNumDims() const { return _dims.size(); }
		void addDim(int size) { _dims.push_back(size); }

		int getOffset() const {
			return offset;
		}

		NativeType getType() const {
			return type;
		}

		int getWidth() const {
			return width;
		}
	};


	/* This represent a local variable declaration. */
	class STLocalVarDecl : public STVariableDeclaration {
	public:
		STLocalVarDecl(string _name, NativeType _type, int _width, int _offset) {
			name = _name;
			type = _type;
			width = _width;
			offset = _offset;
		}

		void dump() const;
	};


	/* This represent a formal parameter declaration in a function. */
	class STParamDecl : public STVariableDeclaration {
	public:
		STParamDecl(string _name, NativeType _type, int _width, int _offset) {
			name = _name;
			type = _type;
			width = _width;
			offset = _offset;
		}

		void dump() const;
	};


	/* This entry will represent a temporary variable used in the IR. */
	class STTempVar : public STVariableDeclaration {
	public:
		STTempVar(string _name, NativeType _type, int _width, int _offset) : STVariableDeclaration(_name, _type, _width, _offset)
		{ }

		void dump() const;
	};


	/* This entry will represent a address label used in the IR. */
	class STLabelDef : public SymbolTableEntry {
	private:
		int _address;

		int _usageCounter;

	public:
		STLabelDef(string nm) : SymbolTableEntry(nm), _address(0), _usageCounter(0)
		{ }

		void address(int addr) { this->_address = addr; }
		int address() const { return this->_address; }

		int usageCounter() const { return this->_usageCounter; }
		void incrementUses() { this->_usageCounter++; }

		void dump() const;
	};


	/* This is used to store the constants when representing the IR. */
	class STConstantDef : public SymbolTableEntry {
	private:
		/* TODO: make these fields part of a union */
		NativeType _type;
		int _integer;
		float _floating;
		string _str;

	public:
		/* Didn't like this too much, but I would have to overload
		 * it in the union anyway, right? 						*/
		STConstantDef(string nm, string s) : SymbolTableEntry(nm), _type(Parser::STRING), _str(s), _integer(0), _floating(0) {}
		STConstantDef(string nm, int i)    : SymbolTableEntry(nm), _type(Parser::INT), _str(""), _integer(i), _floating(0) {}
		STConstantDef(string nm, float f)  : SymbolTableEntry(nm), _type(Parser::FLOAT), _str(""), _integer(0), _floating(f) {}

		string getName() {
			switch (this->_type) {
				case Parser::STRING: return this->_str;
				case Parser::INT: return std::to_string(this->_integer);
				case Parser::FLOAT: return std::to_string(this->_floating);
			}
		}

		void dump() const;
	};

	/* Represents the definition of a function. */
	class STFunctionDeclaration : public SymbolTableEntry {
	private:
		/* Compiler assigned function's label. */
		string label;

		/* The function's return type. */
		NativeType returnType;

		vector<shared_ptr<SymbolTableEntry>> _params;

	public:
		STFunctionDeclaration(string _name, string _label, NativeType _returnType) : SymbolTableEntry(_name), label(_label), returnType(_returnType)
		{ }

		string getLabel() const { return label; }

		const vector<shared_ptr<SymbolTableEntry>>& params() const { return _params; }
		void addParam(shared_ptr<SymbolTableEntry> newParam) { _params.push_back(newParam); }

		NativeType getReturnType() const { return returnType; }

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

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

		virtual void dump(std::stringstream& buffer) const = 0;

		virtual ~SymbolTableEntry() {};
	};


	/* Represent any kind of variable: parameter, temporary, local. */
	class STVariableDeclaration : public SymbolTableEntry {
	protected:
		/* How many bytes this variable occupy. */
		int _width;

		/* Offset within the function frame where this variable will be stored. */
		int _offset;

		/* Is this variable scalar (dims.size() == 0) or a vector with N dimensions
		 * (dims.size() == N)? If it is a vector, each index store the size of each
		 * respective dimension. 												 */
		vector<int> _dims;


	public:
		STVariableDeclaration() : SymbolTableEntry("", Parser::NOT_A_TYPE), _width(0), _offset(0)
		{ }

		STVariableDeclaration(string nm, NativeType tp, int wd, int off) : SymbolTableEntry(nm, tp), _width(wd), _offset(off)
		{ }

		const vector<int>& dims() const { return _dims; }
		int getNumDims() const { return _dims.size(); }
		void addDim(int size) { _dims.push_back(size); }

		int offset() const { return _offset; }
		void offset(int ofts) { this->_offset = ofts; }

		int width() const { return _width; }
	};


	/* This represent a local variable declaration. */
	class STLocalVarDecl : public STVariableDeclaration {
	public:
		STLocalVarDecl(string nm, NativeType tp, int wd, int off) : STVariableDeclaration(nm, tp, wd, off)
		{ }

		void dump(std::stringstream& buffer) const;
	};


	/* This represent a formal parameter declaration in a function. */
	class STParamDecl : public STVariableDeclaration {
	public:
		STParamDecl(string nm, NativeType tp, int wd, int off) : STVariableDeclaration(nm, tp, wd, off)
		{ }

		void dump(std::stringstream& buffer) const;
	};



	/* This is the base class for all kinds of registers. */
	class STRegister : public SymbolTableEntry {
	private:
		unsigned int _width = 0;

	public:
		STRegister(string nm, NativeType tp, int wd) : SymbolTableEntry(nm, tp), _width(wd)
		{ }

		unsigned int width() const { return this->_width; }

		void dump(std::stringstream& buffer) const;
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

		void dump(std::stringstream& buffer) const;
	};


	/* This is used to store the constants when representing the IR. */
	class STConstantDef : public SymbolTableEntry {
	private:
		/* TODO: make these fields part of a union */
		int _integer;
		float _floating;
		string _str;

	public:
		/* TODO: Change this	*/
		STConstantDef(string nm, int i)    : SymbolTableEntry(nm, Parser::INT), _str(""), _integer(i), _floating(0) {}

		string getName() {
			switch (this->_type) {
				case Parser::INT: return std::to_string(this->_integer);
                case Parser::VOID: return "void";
				case Parser::NOT_A_TYPE: 
                    throw System::EXCEPTION_UNEXPECTED_TYPE;
			}
		}

		void dump(std::stringstream& buffer) const;
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

		void dump(std::stringstream& buffer) const;
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

		void dump(std::stringstream& buffer) const ;

		int stackFrameSize() const ;
	};

}

#endif /* SYMBOLTABLE_H_ */

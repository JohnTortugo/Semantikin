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
		DOUBLE,
		STRING
	};

	enum TypeWidth {
		VOID_WIDTH = 2,
		INT_WIDTH = 4,
		FLOAT_WIDTH = 4,
		DOUBLE_WIDTH = 8,
		STRING_WIDTH = 10
	};

	class SymbolTableEntry {
	protected:
		/* Name the user gave to the variable/function. */
		string name;

	public:
		virtual string getName() const = 0;

		virtual void dump() const = 0;

		virtual ~SymbolTableEntry() {};
	};

	class STFunctionDeclaration : public SymbolTableEntry {
	private:
		/* Compiler assigned function's label. */
		string label;

		/* The function's return type. */
		NativeType returnType;

		/* Specification of the function's formal parameters in terms of
		 * <Type,NumberOfDimensions> 								  */
		vector<pair<NativeType, int>> params;

	public:
		STFunctionDeclaration(string _name, string _label, NativeType _returnType, vector<pair<NativeType, int>> _params) {
			this->name = _name;
			this->label = _label;
			this->returnType = _returnType;
			this->params = _params;
		}

		string getName() const { return this->name; }

		string getLabel() const { return label; }

		const vector<pair<NativeType, int>>* getParams() const { return &params; }

		NativeType getReturnType() const { return returnType; }

		void dump() const;
	};

	class STVariableDeclaration : public SymbolTableEntry {
	protected:
		/* the variable's type. */
		NativeType type;

		/* How many bytes this variable occupy. */
		int width;

		/* Offset within the function frame where this variable will be stored. */
		int offset;

		/* Is this variable scalar (numDims == 0) or a vector with N dimensions
		 * (numDims == N)?  												 */
		int numDims;

		STVariableDeclaration() : type(INT), width(0), offset(0), numDims(0) { }

	public:
		int getNumDims() const {
			return numDims;
		}

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
		STLocalVarDecl(string _name, NativeType _type, int _width, int _offset, int _numDims) {
			this->name = _name;
			this->type = _type;
			this->width = _width;
			this->offset = _offset;
			this->numDims = _numDims;
		}

		string getName() const { return this->name; }

		void dump() const;
	};

	/* This represent a formal parameter declaration in a function. */
	class STParamDecl : public STVariableDeclaration {
	public:
		STParamDecl(string _name, NativeType _type, int _width, int _offset, int _numDims) {
			/* TODO: Should add the proper validations here. */
			this->name = _name;
			this->type = _type;
			this->width = _width;
			this->offset = _offset;
			this->numDims = _numDims;
		}

		string getName() const { return this->name; }

		void dump() const;
	};

	/* This entry will represent a temporary variable used in the IR. */
	class STTempVar : public STVariableDeclaration {
		STTempVar(string _name, NativeType _type, int _width, int _offset) {
			this->name = _name;
			this->type = _type;
			this->width = _width;
			this->offset = _offset;
			this->numDims = 0;
		}

		string getName() const { return this->name; }

		void dump() const;
	};

	/* This entry will represent a address label used in the IR. */
	class STLabelDef : public SymbolTableEntry {
	private:
		int _address;

	public:
		STLabelDef(string name, int address) {
			this->name = name;
			this->_address = address;
		}
	};

	/* This is used to store the constants when representing the IR. */
	class STConstantDef : public SymbolTableEntry {
	private:
		/* TODO: make these fields part of a union */
		NativeType _type;
		int _integer;
		float _floating;
		double _doubling;
		string _str;

	public:
		/* Didn't like this too much, but I would have to overload
		 * it in the union anyway, right? 						*/
		STConstantDef(string s) : _type(Parser::STRING), _str(s), _integer(0), _floating(0), _doubling(0) {};
		STConstantDef(int i) : _type(Parser::INT), _str(nullptr), _integer(i), _floating(0), _doubling(0) {};
		STConstantDef(float f) : _type(Parser::FLOAT), _str(nullptr), _integer(0), _floating(f), _doubling(0) {};
		STConstantDef(double d) : _type(Parser::DOUBLE), _str(nullptr), _integer(0), _floating(0), _doubling(d) {};
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

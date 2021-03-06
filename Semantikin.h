#pragma once

#include <string>
#include <list>
#include <map>
#include <set>

#include <memory>
#include <cassert>

using std::shared_ptr;
using std::string;
using std::list;
using std::set;
using std::pair;
using std::map;

/*******************************************************************************
 ** A few forward declarations.		 										  **
 ******************************************************************************/
namespace Parser {
	enum TypeWidth {
		VOID_WIDTH = 0,
		INT_WIDTH = 8
	};

	enum NativeType {
		NOT_A_TYPE,
		VOID,
		INT,
	};

	class STLValue;
	class STLabelDef;
	class STConstantDef;
	class STRegister;
	class STVariableDeclaration;
	class STFunctionDeclaration;
	class SymbolTable;
	class SymbolTableEntry;
}

namespace IR {
	class Instruction;
	class BranchInstruction;
	class Call;
	class Return;

	class BasicBlock;
	class Function;
}

namespace Backend {
    class MachineInstruction;
}


/*******************************************************************************
 ** Names used all throught the code. 										  **
 ******************************************************************************/
typedef shared_ptr<IR::Function>					Function_sptr;
typedef shared_ptr<list<Function_sptr>> 			Function_sptr_list_sptr;

typedef shared_ptr<IR::BasicBlock> 					BasicBlock_sptr;
typedef list<BasicBlock_sptr> 						BasicBlock_list;
typedef shared_ptr<BasicBlock_list> 				BasicBlock_list_sptr;

typedef shared_ptr<Backend::MachineInstruction>	    MachineInstruction_sptr;

typedef shared_ptr<IR::Instruction>					Instruction_sptr;
typedef list<Instruction_sptr> 						Instruction_list;
typedef set<Instruction_sptr> 						Instruction_set;
typedef shared_ptr<Instruction_list> 				Instruction_list_sptr;
typedef shared_ptr<Instruction_set> 				Instruction_set_sptr;

typedef shared_ptr<Parser::STLabelDef>				STLabelDef_sptr;
typedef shared_ptr<Parser::STConstantDef>			STConstantDef_sptr;
typedef shared_ptr<Parser::STVariableDeclaration>	STVariableDeclaration_sptr;
typedef shared_ptr<Parser::STRegister>				STRegister_sptr;
typedef shared_ptr<Parser::STFunctionDeclaration>	STFunctionDecl_sptr;

typedef shared_ptr<Parser::SymbolTableEntry>		STEntry_sptr;
typedef set<STEntry_sptr>							STEntry_set;
typedef shared_ptr<STEntry_set>						STEntry_set_sptr;



/*******************************************************************************
 ** System configuration.. constants, etc. 									  **
 ******************************************************************************/
namespace System {
	const unsigned int EXCEPTION_UNEXPECTED_TYPE = 1001;
	const unsigned int EXCEPTION_UNREACHABLE_CODE = 1002;
	const unsigned int POINTER_SIZE = 8;

	const string NAT_FUN_STRCPY = "strcpy";
}



/*******************************************************************************
 ** Utilitary functions used throught the program.							  **
 ******************************************************************************/
namespace Util {
	/* This function returns zero if the parameter instruction not is a control
	 * handling instruction and a non-zero value if is a control handling
	 * instruction.
	 *
	 * Return:
	 * 		0 - Non-control instruction.
	 * 		1 - A branch instruction.
	 * 		2 - A call instruction.
	 * 		3 - A return instruction.
	 */
	static unsigned char isControlInstr(Instruction_sptr instruction);

	/* This function returns the target of the branch instruction, i.e.,
	 * the target of a jump/branch. If the instruction is a call/ret or
	 * not a control instruction the function returns nullptr. */
	static STLabelDef_sptr branchTarget(Instruction_sptr instruction);

	/* This functions transforms double-quotes and slashes in escaped
	 * sequences of characters. That is:
	 *
	 * Behavior:
	 * "	->	\"
	 * \	-> 	\\
	 * >	->	&gt;
	 * <	-> 	&lt;
	 * &	-> 	&amp;
	 *
	 */
	string escapeStr(string str);

	/*
	 * Return the textual name of a type.
	 */
	string typeName(Parser::NativeType type);



}


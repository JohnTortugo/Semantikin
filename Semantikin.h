#ifndef SEMANTIKIN_H_
#define SEMANTIKIN_H_

#include <string>
#include <list>
#include <memory>
#include <cassert>

using std::shared_ptr;
using std::string;
using std::list;
using std::pair;

/*******************************************************************************
 ** A few forward declarations.		 										  **
 ******************************************************************************/
namespace Parser {
	enum TypeWidth {
		VOID_WIDTH = 0,
		INT_WIDTH = 8,
		FLOAT_WIDTH = 8,
		STRING_WIDTH = 8
	};

	enum NativeType {
		NOT_A_TYPE,
		VOID,
		INT,
		FLOAT,
		STRING
	};

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



/*******************************************************************************
 ** Names used all throught the code. 										  **
 ******************************************************************************/
typedef shared_ptr<IR::Function>					Function_sptr;
typedef shared_ptr<list<Function_sptr>> 			Function_sptr_list_sptr;

typedef shared_ptr<IR::BasicBlock> 					BasicBlock_sptr;
typedef shared_ptr<list<BasicBlock_sptr>> 			BasicBlock_list_sptr;

typedef shared_ptr<IR::Instruction>					Instruction_sptr;
typedef shared_ptr<list<Instruction_sptr>> 			Instruction_list_sptr;

typedef shared_ptr<Parser::STLabelDef>				STLabelDef_sptr;
typedef shared_ptr<Parser::STConstantDef>			STConstantDef_sptr;
typedef shared_ptr<Parser::STVariableDeclaration>	STVariableDeclaration_sptr;
typedef shared_ptr<Parser::STRegister>				STRegister_sptr;
typedef shared_ptr<Parser::STFunctionDeclaration>	STFunctionDecl_sptr;

typedef shared_ptr<Parser::SymbolTableEntry>		SymbolTableEntry_sp;



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






#endif /* SEMANTIKIN_H_ */

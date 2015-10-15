#include "Semantikin.h"
#include "Driver.h"
#include "AstVisitors.h"
#include "IRVisitors.h"
#include "MaximalMunch.h"
#include "TreeCanonicalizer.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

// returns the "value" for the "option" or 0 if the option was not
// specified
char* getCmdOption(char** begin, char** end, const std::string& option) {
	char** itr = std::find(begin, end, option);

	if (itr != end && ++itr != end)
		return *itr;

	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
	return std::find(begin, end, option) != end;
}

void printHeader() {
	printf("This is Semantikin v. 0.1\n");
	printf("--------------------------\n\n");
}

void printHelp() {
	printHeader();
	printf("%10s | %s\n", "-h", 		"Print this help message.");
	printf("%10s | %s\n", "-i", 		"[Mandatory] Specify the source input file.");
	printf("%10s | %s\n", "-o", 		"Specify the executable output file.");
	printf("%10s | %s\n", "-dumps", 	"Specify that no further processing [mounting/linking] is required.");
	printf("%10s | %s\n", "-dumpAsts", 	"Dump the AST for all functions to DOT format.");
	printf("%10s | %s\n", "-dumpCfgs", 	"Dump the CFG for all functions to DOT format.");
	printf("%10s | %s\n", "-dumpIR", 	"Dump the Internal Representation for all functions.");
	printf("%10s | %s\n", "-dumpIRTree","Dump the Internal Representation for all functions in a tree-like format.");
	printf("%10s | %s\n", "-dumpX86", 	"Dump the X86 (AT&T) assembly for all functions.");
}

int main(int argc, char *argv[]) {
	if (cmdOptionExists(argv, argv+argc, "-h")) {
		printHelp();
		exit(0);
	}

	if (!cmdOptionExists(argv, argv+argc, "-i")) {
		fprintf(stderr, "You need to specify an input file.\n");
		exit(1);
	}

	Parser::Driver driver;
	Parser::CompilationUnit* astModule = nullptr;

	char* inputFileName = getCmdOption(argv, argv+argc, "-i");



	/** Start parsing */
	if (access(inputFileName, F_OK | R_OK)) {
		fprintf(stderr, "Could not access the specified input file.\n");
		exit(1);
	}

	if (!driver.parse(inputFileName, astModule))
		exit(1);



	/* Do semantic analyzis. */
	AstSemaVisitor semantic;
	astModule->accept(&semantic);




	/* Generate the intermediate representation */
	AstTACGenVisitor irgen(semantic.currentOffset());
	astModule->accept(&irgen);

	/* Obtain pointer to the IR generated module. */
	auto irModule = irgen.module();

	
	/* Call tree canonicalizer */
	TreeCanonicalizer tc;
	irModule->accept(&tc);



	/* Generate code using maximal munch algorithm */
	MaximalMunch codeGen;
	irModule->accept(&codeGen);





	/* Just print out the AST of the function */
	if (cmdOptionExists(argv, argv+argc, "-dumpAsts")) {
		for (auto& func : *astModule->getFunctions()) {
			AstToDotVisitor ast("ast_" + func->getName() + ".dot");
			func->accept(&ast);
		}
	}

	/* Check if we need to print the IR-Tree */
	if (cmdOptionExists(argv, argv+argc, "-dumpIRTree")) {
		string dotFileName(inputFileName);

		IRToDotVisitor itdVisitor(dotFileName + ".dot");
		irModule->accept(&itdVisitor);
	}

	/* Just dump the IR */
	if (cmdOptionExists(argv, argv+argc, "-dumpIR")) {
		cout << " -dumpIR is currently not working." << endl;
		// irModule->dump();
	}



	stringstream output;
	for (auto& f : *irModule->functions()) {
		output << endl << "Code for function: " << f->name() << endl;

		for (auto& bb : *f->bbs()) {
			
			for (auto& instr : bb->instructions()) {
				instr.dump(output);
			}

		}
	}

	cout << output.str() << endl;

	/* Finish */
	exit(0);
}

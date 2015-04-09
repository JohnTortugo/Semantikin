#include "Semantikin.h"
#include "Driver.h"
#include "AstVisitors.h"
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
	printf("%10s | %s\n", "-dumpAsts", 	"Dump the AST for all functions to DOT format.");
	printf("%10s | %s\n", "-dumpCfgs", 	"Dump the CFG for all functions to DOT format.");
	printf("%10s | %s\n", "-dumpIR", 	"Dump the Internal Representation for all functions.");
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

	/* Parsing. */
	char* inputFileName = getCmdOption(argv, argv+argc, "-i");

	if (!driver.parse(inputFileName, astModule))
		exit(1);

	/* Do semantic analyzis. */
	AstSemaVisitor semantic;
	astModule->accept(&semantic);

	/* Generate TAC-SSA IR. */
	AstTACGenVisitor irgen(semantic.currentOffset());
	astModule->accept(&irgen);

	/* Obtain pointer to the IR generated module. */
	shared_ptr<IR::Module> irModule = irgen.module();

	/* Just print out the AST of the function */
	if (cmdOptionExists(argv, argv+argc, "-dumpAsts")) {
		for (auto& func : *astModule->getFunctions()) {
			AstToDotVisitor ast("ast_" + func->getName() + ".dot");
			func->accept(&ast);
		}
	}

	/* print out the cfg of the function */
	if (cmdOptionExists(argv, argv+argc, "-dumpCfgs")) {
		for (auto& func : *irModule->functions()) {
			auto cfg = func->cfg();
			cfg->dumpToDot("cfg_" + func->addr()->getName() + ".dot");
		}
	}

	/* Just dump the IR */
	if (cmdOptionExists(argv, argv+argc, "-dumpIR")) {
		irModule->dump();
	}

	/* print the x86 assembly of each instruction */
	stringstream x86Stream;
	irModule->linearDumpTox86(x86Stream);

	if (cmdOptionExists(argv, argv+argc, "-dumpX86")) {
		std::ofstream asmFile(getCmdOption(argv, argv+argc, "-dumpX86"));
		asmFile << x86Stream.str() << std::endl;
		asmFile.close();
	}

	/* Call GCC to assembly and link */
	char asmFileName[50] 	= {"/tmp/Semantikin_XXXXXX"};
	int asmFile 			= mkstemp(asmFileName);

	if (!asmFile || asmFile == -1) {
		fprintf(stderr, "It was not possible to create a temporary file.\n");
		exit(1);
	}
	else {
		write(asmFile, x86Stream.str().c_str(), x86Stream.str().size());
		close(asmFile);

		string outputName = "executable";

		if (cmdOptionExists(argv, argv+argc, "-o"))
			outputName = getCmdOption(argv, argv+argc, "-o");

		stringstream ss;
		ss << "gcc lib.c -x assembler " << asmFileName << " -o " << outputName;
		cout << ss.str() << endl;
		system(ss.str().c_str());
	}

	/* Finish */
	exit(0);
}

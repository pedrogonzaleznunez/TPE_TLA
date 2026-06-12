#ifndef COMPILER_STATE_HEADER
#define COMPILER_STATE_HEADER

#include "../../backend/semantic-analysis/SymbolTable.h"

/**
 * The global state of the compiler. Should transport every data structure
 * needed across the different phases of a compilation.
 */
typedef struct {
	/**
	 * The root node of the AST.
	 */
	void * abstractSyntaxtTree;

	/**
	 * The symbol table used for semantic analysis.
	 * Holds all declared hardware components, variables, and loop variables,
	 * along with type information and scope management.
	 */
	SymbolTable * symbolTable;

	/**
	 * The output file path for the generated Arduino .ino code.
	 * If NULL, the output defaults to <input_name>.ino in the same directory
	 * as the source file.
	 */
	char * outputPath;

	/**
	 * Logger instance for diagnostic messages during compilation.
	 */
	Logger * logger;
} CompilerState;

#endif
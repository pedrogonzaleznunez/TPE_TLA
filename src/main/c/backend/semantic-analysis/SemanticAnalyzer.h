#ifndef SEMANTIC_ANALYZER_HEADER
#define SEMANTIC_ANALYZER_HEADER

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/CompilerState.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"

/**
 * Initialize the semantic analyzer module.
 * Returns a destructor that should be called on shutdown.
 */
ModuleDestructor initializeSemanticAnalyzerModule(CompilerState * state);

/**
 * Executes semantic analysis on the given compiler state.
 *
 * Performs two passes on the AST:
 *   Pass 1: Collects all hardware and variable declarations into the symbol table.
 *   Pass 2: Recursively type-checks all expressions and statements.
 *
 * Returns SUCCEEDED if no semantic errors are found, FAILED otherwise.
 */
CompilationStatus executeSemanticAnalysis(CompilerState * state);

#endif
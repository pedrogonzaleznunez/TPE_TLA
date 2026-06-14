#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/CompilationStatus.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

typedef enum {
	SYM_HARDWARE,
	SYM_VARIABLE,
	SYM_FOR_LOOP_VAR
} SymbolKind;

// Symbol entry

typedef struct SymbolEntry {
	char * name;
	SymbolKind kind;
	DataType dataType;
	bool initialized;
	ComponentType componentType; // meaningful when kind == SYM_HARDWARE
	int pin;                     // meaningful when kind == SYM_HARDWARE with single pin 
	bool hasSinglePin;           // false for multi-pin components (ultrasonic, LCD, etc.) 
	int * pinList;               // multi-pin array (for ultrasonic, LCD, etc.) 
	int pinCount;                // number of pins in pinList 
	int scopeLevel;              // nesting depth at which this symbol was declared 
	struct SymbolEntry * next;   // linked list (chaining within hash bucket) 
	struct SymbolEntry * scopeNext; // linked list (per-scope list, separate from hash chain) 
} SymbolEntry;

// Symbol table (hash table + scope stack)

typedef struct SymbolTable {
	// hash table buckets
	SymbolEntry ** buckets;
	int bucketCount;

	// scope stack: each scope is a linked list of entries
	SymbolEntry ** scopes;      // array of scope lists 
	int * scopeIds;             // parallel array: scope id for each level 
	int scopeCapacity;
	int scopeCount;             // current number of scopes on stack 
	int currentScopeLevel;      // 0 = global, increments with each push 
	int nextScopeId;

	bool preserve;              // if true, popScope does NOT destroy entries
	Logger * logger;
} SymbolTable;

// Function prototypes

/**
 * Creates a new symbol table with the given number of hash buckets
 * and pushes the global scope (level 0).
 * Returns NULL on allocation failure.
 */
SymbolTable * createSymbolTable(int bucketCount, Logger * logger);

/**
 * Pushes a new scope onto the scope stack.
 */
void pushScope(SymbolTable * table);

/**
 * Pops the innermost scope, freeing all symbols declared in it.
 * Cannot pop the global scope.
 */
void popScope(SymbolTable * table);

/**
 * Creates a new symbol in the current scope.
 * Returns FAILED if a symbol with the same name already exists in the current scope,
 * or SUCCEEDED on success.
 */
CompilationStatus createSymbol(
	SymbolTable * table,
	const char * name,
	SymbolKind kind,
	DataType dataType,
	bool initialized,
	ComponentType componentType,
	int pin,
	bool hasSinglePin,
	int * pinList,
	int pinCount
);

/**
 * Updates the data type and initialization status of a symbol
 * in the current scope only.
 * Returns FAILED if the symbol is not found in the current scope.
 */
CompilationStatus updateSymbol(
	SymbolTable * table,
	const char * name,
	DataType dataType,
	bool initialized
);

/**
 * Looks up a symbol by name, searching from the innermost scope outward.
 * Returns NULL if the symbol is not found in any scope.
 */
SymbolEntry * lookupSymbol(SymbolTable * table, const char * name);

/**
 * Sets the preserve mode on the symbol table.
 * When preserve is true, popScope does NOT destroy entries or remove them
 * from hash buckets — it only adjusts the scope stack pointer.
 * This is useful for multi-pass semantic analysis where pass 1 collects
 * declarations and pass 2 needs to look them up.
 */
void setPreserveMode(SymbolTable * table, bool preserve);

/**
 * Destroys the symbol table and frees all associated memory.
 */
void destroySymbolTable(SymbolTable * table);

#endif

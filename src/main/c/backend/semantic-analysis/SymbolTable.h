#ifndef SYMBOL_TABLE_HEADER
#define SYMBOL_TABLE_HEADER

#include "../../support/logging/Logger.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

// Data types for the symbol table                                  

typedef enum {
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_BOOL,
	TYPE_TIME,
	TYPE_VOID,
	TYPE_ERROR,
	TYPE_UNKNOWN
} DataType;

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

	Logger * logger;
} SymbolTable;

#endif
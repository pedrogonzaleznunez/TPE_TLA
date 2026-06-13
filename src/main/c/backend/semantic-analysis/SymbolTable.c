#include "SymbolTable.h"
#include "../../support/type/CompilationStatus.h"
#include <string.h>
#include <stdlib.h>

static unsigned int hashString(const char * str, int bucketCount) {
	unsigned int hash = 5381;
	int c;
	while ((c = (unsigned char)(*str++)) != '\0') {
		hash = ((hash << 5) + hash) + c; // hash * 33 + c 
	}
	return hash % bucketCount;
}

static SymbolEntry * createSymbolEntry(
	const char * name,
	SymbolKind kind,
	DataType dataType,
	bool initialized,
	ComponentType componentType,
	int pin,
	bool hasSinglePin,
	int * pinList,
	int pinCount,
	int scopeLevel
) {
	SymbolEntry * entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));
	if (entry == NULL) {
		return NULL;
	}
	entry->name = (char *)malloc(strlen(name) + 1);
	if (entry->name == NULL) {
		free(entry);
		return NULL;
	}
	strcpy(entry->name, name);
	entry->kind = kind;
	entry->dataType = dataType;
	entry->initialized = initialized;
	entry->componentType = componentType;
	entry->pin = pin;
	entry->hasSinglePin = hasSinglePin;
	entry->pinList = pinList;
	entry->pinCount = pinCount;
	entry->scopeLevel = scopeLevel;
	entry->next = NULL;
	entry->scopeNext = NULL;
	return entry;
}

static void freeSymbolEntry(SymbolEntry * entry) {
	if (entry == NULL) {
		return;
	}
	free(entry->name);
	if (entry->pinList != NULL) {
		free(entry->pinList);
	}
	free(entry);
}

SymbolTable * createSymbolTable(int bucketCount, Logger * logger) {
	SymbolTable * table = (SymbolTable *)malloc(sizeof(SymbolTable));
	if (table == NULL) {
		return NULL;
	}

	table->bucketCount = (bucketCount <= 0) ? 64 : bucketCount;
	table->buckets = (SymbolEntry **)calloc(table->bucketCount, sizeof(SymbolEntry *));
	if (table->buckets == NULL) {
		free(table);
		return NULL;
	}

	// Initial scope stack capacity: 16 is generous for nesting depth
	table->scopeCapacity = 16;
	table->scopes = (SymbolEntry **)malloc(table->scopeCapacity * sizeof(SymbolEntry *));
	table->scopeIds = (int *)malloc(table->scopeCapacity * sizeof(int));
	if (table->scopes == NULL || table->scopeIds == NULL) {
		free(table->buckets);
		free(table->scopes);
		free(table->scopeIds);
		free(table);
		return NULL;
	}

	// Push the global scope (level 0) 
	table->scopes[0] = NULL;
	table->scopeIds[0] = 0;
	table->scopeCount = 1;
	table->currentScopeLevel = 0;
	table->nextScopeId = 1;

	table->preserve = false;

	table->logger = logger;

	logDebugging(logger, "SymbolTable created with %d buckets", table->bucketCount);
	return table;
}

void pushScope(SymbolTable * table) {
	if (table->scopeCount >= table->scopeCapacity) {
		table->scopeCapacity *= 2;
		table->scopes = (SymbolEntry **)realloc(
			table->scopes, table->scopeCapacity * sizeof(SymbolEntry *));
		table->scopeIds = (int *)realloc(
			table->scopeIds, table->scopeCapacity * sizeof(int));
		if (table->scopes == NULL || table->scopeIds == NULL) {
			logCritical(table->logger,
				"pushScope: failed to reallocate scope stack");
			return;
		}
	}

	table->currentScopeLevel++;
	table->scopes[table->scopeCount] = NULL;
	table->scopeIds[table->scopeCount] = table->nextScopeId;
	table->nextScopeId++;
	table->scopeCount++;

	logDebugging(table->logger, "pushScope: new level %d (scopeId %d)",
		table->currentScopeLevel, table->scopeIds[table->scopeCount - 1]);
}

void popScope(SymbolTable * table) {
	if (table->scopeCount <= 1) {
		logWarning(table->logger,
			"popScope: cannot pop the global scope");
		return;
	}

	table->scopeCount--;
	table->currentScopeLevel--;

	if (!table->preserve) {
		// Free all entries in the scope being popped 
		SymbolEntry * entry = table->scopes[table->scopeCount];
		while (entry != NULL) {
			SymbolEntry * next = entry->scopeNext;

			// Also remove from hash bucket chain
			unsigned int idx = hashString(entry->name, table->bucketCount);
			SymbolEntry ** prevPtr = &(table->buckets[idx]);
			while (*prevPtr != NULL) {
				if (*prevPtr == entry) {
					*prevPtr = entry->next;
					break;
				}
				prevPtr = &((*prevPtr)->next);
			}

			freeSymbolEntry(entry);
			entry = next;
		}
	}

	table->scopes[table->scopeCount] = NULL;

	logDebugging(table->logger, "popScope: back to level %d (scopeCount %d)",
		table->currentScopeLevel, table->scopeCount);
}

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
) {
	// Check for duplicate in current scope
	SymbolEntry * existing = table->scopes[table->scopeCount - 1];
	while (existing != NULL) {
		if (strcmp(existing->name, name) == 0) {
			logError(table->logger,
				"Semantic error: symbol '%s' already declared in current scope",
				name);
			return FAILED;
		}
		existing = existing->scopeNext;
	}

	SymbolEntry * entry = createSymbolEntry(
		name, kind, dataType, initialized,
		componentType, pin, hasSinglePin, pinList, pinCount,
		table->currentScopeLevel);

	if (entry == NULL) {
		logCritical(table->logger,
			"createSymbol: failed to allocate SymbolEntry for '%s'", name);
		return FAILED;
	}

	// Insert into hash bucket
	unsigned int idx = hashString(name, table->bucketCount);
	entry->next = table->buckets[idx];
	table->buckets[idx] = entry;

	// Insert into current scope list
	entry->scopeNext = table->scopes[table->scopeCount - 1];
	table->scopes[table->scopeCount - 1] = entry;

	logDebugging(table->logger,
		"createSymbol: '%s' (kind=%d, type=%d) at scope level %d",
		name, kind, dataType, table->currentScopeLevel);

	return SUCCEEDED;
}

CompilationStatus updateSymbol(
	SymbolTable * table,
	const char * name,
	DataType dataType,
	bool initialized
) {
	// Lookup in current scope only (can't change outer scope symbols)
	SymbolEntry * entry = table->scopes[table->scopeCount - 1];
	while (entry != NULL) {
		if (strcmp(entry->name, name) == 0) {
			entry->dataType = dataType;
			entry->initialized = initialized;
			logDebugging(table->logger,
				"updateSymbol: '%s' -> type=%d, initialized=%d",
				name, dataType, initialized);
			return SUCCEEDED;
		}
		entry = entry->scopeNext;
	}

	logError(table->logger,
		"Semantic error: cannot update symbol '%s' — not found in current scope",
		name);
	return FAILED;
}

SymbolEntry * lookupSymbol(SymbolTable * table, const char * name) {
	// Search from innermost scope outward
	for (int i = table->scopeCount - 1; i >= 0; i--) {
		SymbolEntry * entry = table->scopes[i];
		while (entry != NULL) {
			if (strcmp(entry->name, name) == 0) {
				logDebugging(table->logger,
					"lookupSymbol: '%s' found at scope level %d (scopeIdx %d)",
					name, entry->scopeLevel, i);
				return entry;
			}
			entry = entry->scopeNext;
		}
	}

	logDebugging(table->logger,
		"lookupSymbol: '%s' not found in any scope", name);
	return NULL;
}

void setPreserveMode(SymbolTable * table, bool preserve) {
	if (table == NULL) {
		return;
	}
	table->preserve = preserve;
	logDebugging(table->logger,
		"setPreserveMode: preserve = %s", preserve ? "true" : "false");
}

void destroySymbolTable(SymbolTable * table) {
	if (table == NULL) {
		return;
	}

	// Free all entries from all scopes (while also cleaning hash buckets)
	for (int i = 0; i < table->scopeCount; i++) {
		SymbolEntry * entry = table->scopes[i];
		while (entry != NULL) {
			SymbolEntry * next = entry->scopeNext;

			// Remove from hash bucket (if not already removed by popScope)
			unsigned int idx = hashString(entry->name, table->bucketCount);
			SymbolEntry ** prevPtr = &(table->buckets[idx]);
			while (*prevPtr != NULL) {
				if (*prevPtr == entry) {
					*prevPtr = entry->next;
					break;
				}
				prevPtr = &((*prevPtr)->next);
			}

			freeSymbolEntry(entry);
			entry = next;
		}
	}

	free(table->scopes);
	free(table->scopeIds);
	free(table->buckets);
	free(table);
}
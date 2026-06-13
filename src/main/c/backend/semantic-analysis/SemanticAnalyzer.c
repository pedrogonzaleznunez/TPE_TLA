#include "SemanticAnalyzer.h"
#include "SymbolTable.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/CompilationStatus.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Module destructor                                                    

static Logger * semanticLogger = NULL;

static void destroySemanticAnalyzerModule() {
	if (semanticLogger != NULL) {
		destroyLogger(semanticLogger);
		semanticLogger = NULL;
	}
}

ModuleDestructor initializeSemanticAnalyzerModule() {
	semanticLogger = createLogger("SemanticAnalyzer");
	if (semanticLogger == NULL) {
		logCritical(NULL, "initializeSemanticAnalyzerModule: failed to create logger");
		return NULL;
	}
	return destroySemanticAnalyzerModule;
}


// Forward declarations for internal helpers                            


static CompilationStatus pass1(Program * program, SymbolTable * table, Logger * logger);
static CompilationStatus pass2(Program * program, SymbolTable * table, Logger * logger);

static CompilationStatus collectHardwareDeclarations(Program * program, SymbolTable * table, Logger * logger);
static CompilationStatus collectRoutineDeclarations(StmtList * stmts, SymbolTable * table, Logger * logger);

static DataType typeOf(Expr * expr, SymbolTable * table, Logger * logger, CompilationStatus * status);
static CompilationStatus validateStmt(Stmt * stmt, SymbolTable * table, Logger * logger);
static CompilationStatus validateExprStmtCall(char * object, char * method, ArgList * args, SymbolTable * table, Logger * logger);
static CompilationStatus checkMethodSignature(const char * componentName, ComponentType compType, const char * method, ArgList * args, SymbolTable * table, Logger * logger);

// Valid method table for each component type                           

typedef struct {
	const char * methodName;
	DataType returnType;
	int paramCount;
	DataType paramTypes[3]; // max 3 params, though currently max is 1
} MethodInfo;

typedef struct {
	ComponentType compType;
	const char * typeName;
	int methodCount;
	MethodInfo methods[6]; // max methods per component
} ComponentMethodTable;

// Maybe we can add more methods in the future

static const ComponentMethodTable COMPONENT_METHODS[] = {
	{ COMP_LED,           "LED",           3, {
		{ "turn_on",       TYPE_VOID,  0, {} },
		{ "turn_off",      TYPE_VOID,  0, {} },
		{ "on",            TYPE_VOID,  1, { TYPE_INT } },
	}},
	{ COMP_BUZZER,        "BUZZER",        2, {
		{ "turn_on",       TYPE_VOID,  0, {} },
		{ "turn_off",      TYPE_VOID,  0, {} },
	}},
	{ COMP_BUTTON,        "BUTTON",        1, {
		{ "is_pressed",    TYPE_BOOL,  0, {} },
	}},
	{ COMP_POTENTIOMETER, "POTENTIOMETER", 1, {
		{ "read_value",    TYPE_INT,   0, {} },
	}},
	{ COMP_SERVO,         "SERVO",         1, {
		{ "write",         TYPE_VOID,  1, { TYPE_INT } },
	}},
	{ COMP_ULTRASONIC,    "ULTRASONIC",    1, {
		{ "read_distance", TYPE_FLOAT, 0, {} },
	}},
	{ COMP_DHT11,         "DHT11",         2, {
		{ "read_temperature", TYPE_FLOAT, 0, {} },
		{ "read_humidity",    TYPE_FLOAT, 0, {} },
	}},
	{ COMP_LCD,           "LCD",           2, {
		{ "print",         TYPE_VOID,  1, { TYPE_STRING } },
		{ "clear",         TYPE_VOID,  0, {} },
	}},
};

static const int COMPONENT_METHOD_COUNT =
	sizeof(COMPONENT_METHODS) / sizeof(COMPONENT_METHODS[0]);


// Pin requirements per component    

/**
 * Returns:
 *   -1  if the component does not use pin-based validation (only fixed pins)
 *    0  if the component requires a single digital pin (0-13)
 *    1  if the component requires an analog pin (0-5, meaning A0-A5)
 *    2  if ultrasonic (2 pins: trig, echo)
 *    6  if LCD (6 pins: rs, en, d4, d5, d6, d7)
 */
static int getExpectedPinCount(ComponentType type) {
	switch (type) {
		case COMP_LED:           return 1;
		case COMP_BUZZER:        return 1;
		case COMP_BUTTON:        return 1;
		case COMP_POTENTIOMETER: return 1;
		case COMP_SERVO:         return 1;
		case COMP_ULTRASONIC:    return 2;
		case COMP_DHT11:         return 1;
		case COMP_LCD:           return 6;
		default:                 return -1;
	}
}

static bool isAnalogComponent(ComponentType type) {
	return type == COMP_POTENTIOMETER;
}

// extractPinArray: extract pin values from a PinSpec into an int array 

static int * extractPinValues(PinSpec * pinSpec, int * outCount) {
	int * pins = NULL;
	int count = 0;

	if (pinSpec == NULL) {
		*outCount = 0;
		return NULL;
	}

	switch (pinSpec->type) {
		case PIN_SPEC_SINGLE_INT: {
			count = 1;
			pins = (int *)malloc(sizeof(int));
			if (pins != NULL) pins[0] = pinSpec->singlePin;
			break;
		}
		case PIN_SPEC_SINGLE_IDENTIFIER: {
			count = 1;
			pins = (int *)malloc(sizeof(int));
			if (pins != NULL) {
				// For analog identifiers like A0, A1..., store them as negative
				// to distinguish from numeric pins during duplication checks
				const char * id = pinSpec->singleIdentifier;
				int val = 0;
				if (id != NULL && id[0] == 'A') {
					val = atoi(id + 1); // "A0" -> 0, "A1" -> 1, etc.
					// Store as negative to indicate analog (will be converted back)
					pins[0] = -(val + 1); // A0 -> -1, A1 -> -2, etc.
				} else if (id != NULL) {
					pins[0] = atoi(id);
				} else {
					pins[0] = -1;
				}
			}
			break;
		}
		case PIN_SPEC_NAMED_LIST: {
			PinSpecEntry * entry = pinSpec->namedPins;
			while (entry != NULL) {
				count++;
				entry = entry->next;
			}
			pins = (int *)malloc(count * sizeof(int));
			if (pins != NULL) {
				int i = 0;
				entry = pinSpec->namedPins;
				while (entry != NULL) {
					pins[i++] = entry->value;
					entry = entry->next;
				}
			}
			break;
		}
		case PIN_SPEC_INT_LIST: {
			PinSpecEntry * entry = pinSpec->intPins;
			while (entry != NULL) {
				count++;
				entry = entry->next;
			}
			pins = (int *)malloc(count * sizeof(int));
			if (pins != NULL) {
				int i = 0;
				entry = pinSpec->intPins;
				while (entry != NULL) {
					pins[i++] = entry->value;
					entry = entry->next;
				}
			}
			break;
		}
	}

	*outCount = count;
	return pins;
}

static void freePinValues(int * pins) {
	free(pins);
}

// Duplicate pin checking

typedef struct PinEntry {
	int pin;
	struct PinEntry * next;
} PinEntry;

static PinEntry * pinList = NULL;

static void clearPinList() {
	while (pinList != NULL) {
		PinEntry * temp = pinList;
		pinList = pinList->next;
		free(temp);
	}
}

static bool isPinDuplicate(int pin) {
	PinEntry * cur = pinList;
	while (cur != NULL) {
		if (cur->pin == pin) return true;
		cur = cur->next;
	}
	return false;
}

static void addPin(int pin) {
	PinEntry * entry = (PinEntry *)malloc(sizeof(PinEntry));
	if (entry == NULL) return;
	entry->pin = pin;
	entry->next = pinList;
	pinList = entry;
}

// Valid named pin names for components 

static bool validateLcdPinNames(PinSpec * pinSpec) {
	if (pinSpec->type != PIN_SPEC_NAMED_LIST) return false;

	static const char * expected[] = { "rs", "en", "d4", "d5", "d6", "d7" };
	int idx = 0;
	PinSpecEntry * entry = pinSpec->namedPins;

	while (entry != NULL && idx < 6) {
		if (strcmp(entry->name, expected[idx]) != 0) {
			return false;
		}
		entry = entry->next;
		idx++;
	}

	return (idx == 6 && entry == NULL);
}

static bool validateUltrasonicPinNames(PinSpec * pinSpec) {
	if (pinSpec->type != PIN_SPEC_NAMED_LIST) return false;

	static const char * expected[] = { "trig", "echo" };
	int idx = 0;
	PinSpecEntry * entry = pinSpec->namedPins;

	while (entry != NULL && idx < 2) {
		if (strcmp(entry->name, expected[idx]) != 0) {
			return false;
		}
		entry = entry->next;
		idx++;
	}

	return (idx == 2 && entry == NULL);
}

// PASS 1: Collect hardware and variable declarations  

static CompilationStatus collectHardwareDeclarations(
	Program * program, SymbolTable * table, Logger * logger)
{
	CompilationStatus status = SUCCEEDED;

	if (program->hardware == NULL || program->hardware->declarations == NULL) {
		return SUCCEEDED;
	}

	HardwareDeclList * list = program->hardware->declarations;
	while (list != NULL && list->declaration != NULL) {
		HardwareDecl * decl = list->declaration;

		// Check: duplicate component name
		SymbolEntry * existing = lookupSymbol(table, decl->identifier);
		if (existing != NULL) {
			logError(logger,
				"Semantic error (line?): component '%s' already declared",
				decl->identifier);
			status = FAILED;
			list = list->next;
			continue;
		}

		// Extract pin values
		int pinCount = 0;
		int * pins = extractPinValues(decl->pinSpec, &pinCount);

		// Check: expected pin count
		int expected = getExpectedPinCount(decl->componentType);
		if (expected > 0 && pinCount != expected) {
			logError(logger,
				"Semantic error: component '%s' expects %d pin(s), but %d provided",
				decl->identifier, expected, pinCount);
			status = FAILED;
			freePinValues(pins);
			list = list->next;
			continue;
		}

		// Check: pin names for LCD and Ultrasonic
		if (decl->componentType == COMP_LCD && !validateLcdPinNames(decl->pinSpec)) {
			logError(logger,
				"Semantic error: component '%s' (LCD) requires named pins: rs, en, d4, d5, d6, d7",
				decl->identifier);
			status = FAILED;
			freePinValues(pins);
			list = list->next;
			continue;
		}

		if (decl->componentType == COMP_ULTRASONIC && !validateUltrasonicPinNames(decl->pinSpec)) {
			logError(logger,
				"Semantic error: component '%s' (ULTRASONIC) requires named pins: trig, echo",
				decl->identifier);
			status = FAILED;
			freePinValues(pins);
			list = list->next;
			continue;
		}

		// Check: pin range and duplicates
		bool pinsOk = true;
		for (int i = 0; i < pinCount; i++) {
			int rawPin = pins[i];
			bool isAnalog = false;
			int effectivePin = rawPin;

			// Detect analog pins stored as negative values
			if (rawPin < 0) {
				isAnalog = true;
				effectivePin = (-rawPin) - 1; // -1 -> A0 (0), -2 -> A1 (1), etc.
			}

			// Validate range
			if (isAnalog) {
				if (effectivePin < 0 || effectivePin > 5) {
					logError(logger,
						"Semantic error: component '%s' uses invalid analog pin A%d "
						"(valid range: A0-A5)",
						decl->identifier, effectivePin);
					pinsOk = false;
				}
			} else {
				if (effectivePin < 0 || effectivePin > 13) {
					logError(logger,
						"Semantic error: component '%s' uses invalid digital pin %d "
						"(valid range: 0-13)",
						decl->identifier, effectivePin);
					pinsOk = false;
				}
			}

			// Check duplicate pin (only for digital pins; analog pins are unique by definition)
			if (!isAnalog && isPinDuplicate(effectivePin)) {
				logError(logger,
					"Semantic error: digital pin %d is already used by another component "
					"(component '%s')",
					effectivePin, decl->identifier);
				pinsOk = false;
			}

			// Check duplicate analog pin
			if (isAnalog && isPinDuplicate(rawPin)) {
				logError(logger,
					"Semantic error: analog pin A%d is already used by another component "
					"(component '%s')",
					effectivePin, decl->identifier);
				pinsOk = false;
			}

			// Add to global pin list if ok so far
			if (pinsOk) {
				if (isAnalog) {
					addPin(rawPin); // use the negative encoding
				} else {
					addPin(effectivePin);
				}
			}
		}

		if (!pinsOk) {
			status = FAILED;
			freePinValues(pins);
			list = list->next;
			continue;
		}

		// Determine data type for the component (for type checking method calls)
		DataType compDataType = TYPE_UNKNOWN;

		// Create symbol: SYM_HARDWARE
		bool hasSinglePin = (pinCount <= 1);
		int singlePin = (pinCount > 0) ? pins[0] : -1;
		int * pinListCopy = NULL;
		if (pinCount > 0) {
			pinListCopy = (int *)malloc(pinCount * sizeof(int));
			if (pinListCopy != NULL) {
				memcpy(pinListCopy, pins, pinCount * sizeof(int));
			}
		}

		CompilationStatus symStatus = createSymbol(
			table,
			decl->identifier,
			SYM_HARDWARE,
			compDataType,
			true,                            // hardware is always "initialized"
			decl->componentType,
			singlePin,
			hasSinglePin,
			pinListCopy,
			pinCount
		);

		if (symStatus == FAILED) {
			logError(logger,
				"Semantic error: failed to create symbol for component '%s'",
				decl->identifier);
			status = FAILED;
			freePinValues(pins);
			free(pinListCopy);
			list = list->next;
			continue;
		}

		freePinValues(pins);
		list = list->next;
	}

	return status;
}

static CompilationStatus collectRoutineDeclarations(
	StmtList * stmts, SymbolTable * table, Logger * logger)
{
	CompilationStatus status = SUCCEEDED;
	StmtList * cur = stmts;

	while (cur != NULL && cur->stmt != NULL) {
		Stmt * stmt = cur->stmt;

		switch (stmt->type) {
			case STMT_VAR: {
				DataType varType = TYPE_UNKNOWN;
				if (stmt->var.value != NULL) {
					CompilationStatus typeStatus = SUCCEEDED;
					varType = typeOf(stmt->var.value, table, logger, &typeStatus);
				}

				SymbolEntry * existing = lookupSymbol(table, stmt->var.name);
				if (existing != NULL &&
					existing->scopeLevel == table->currentScopeLevel)
				{
					logError(logger,
						"Semantic error: variable '%s' already declared in current scope",
						stmt->var.name);
					status = FAILED;
					cur = cur->next;
					continue;
				}

				CompilationStatus symStatus = createSymbol(
					table,
					stmt->var.name,
					SYM_VARIABLE,
					varType,
					(stmt->var.value != NULL),
					-1,    // componentType unused
					true,  // hasSinglePin unused
					-1,    // pin unused
					NULL,  // pinList unused
					0      // pinCount unused
				);

				if (symStatus == FAILED) {
					logError(logger,
						"Semantic error: failed to create symbol for variable '%s'",
						stmt->var.name);
					status = FAILED;
				}
				break;
			}

			case STMT_FOR_RANGE: {
				// Declare loop variable in the current scope (before entering body scope)
				SymbolEntry * existing = lookupSymbol(table, stmt->forRange.varName);
				if (existing != NULL &&
					existing->scopeLevel == table->currentScopeLevel)
				{
					logError(logger,
						"Semantic error: loop variable '%s' already declared",
						stmt->forRange.varName);
					status = FAILED;
					cur = cur->next;
					continue;
				}

				CompilationStatus symStatus = createSymbol(
					table,
					stmt->forRange.varName,
					SYM_FOR_LOOP_VAR,
					TYPE_INT,
					false,
					-1,
					true,
					-1,
					NULL,
					0
				);

				if (symStatus == FAILED) {
					logError(logger,
						"Semantic error: failed to create for-loop variable '%s'",
						stmt->forRange.varName);
					status = FAILED;
				}

				// Recurse into for body with a new scope
				pushScope(table);
				if (stmt->forRange.body != NULL) {
					CompilationStatus bodyStatus =
						collectRoutineDeclarations(stmt->forRange.body, table, logger);
					if (bodyStatus == FAILED) status = FAILED;
				}
				popScope(table);
				break;
			}

			case STMT_BLOCK: {
				pushScope(table);
				if (stmt->block.body != NULL) {
					CompilationStatus bodyStatus =
						collectRoutineDeclarations(stmt->block.body, table, logger);
					if (bodyStatus == FAILED) status = FAILED;
				}
				popScope(table);
				break;
			}

			case STMT_IF: {
				// Then branch
				pushScope(table);
				if (stmt->if_.thenBranch != NULL) {
					CompilationStatus thenStatus =
						collectRoutineDeclarations(stmt->if_.thenBranch, table, logger);
					if (thenStatus == FAILED) status = FAILED;
				}
				popScope(table);

				// Else branch
				pushScope(table);
				if (stmt->if_.elseBranch != NULL) {
					CompilationStatus elseStatus =
						collectRoutineDeclarations(stmt->if_.elseBranch, table, logger);
					if (elseStatus == FAILED) status = FAILED;
				}
				popScope(table);
				break;
			}

			case STMT_REPEAT_EVERY: {
				pushScope(table);
				if (stmt->repeatEvery.body != NULL) {
					CompilationStatus bodyStatus =
						collectRoutineDeclarations(stmt->repeatEvery.body, table, logger);
					if (bodyStatus == FAILED) status = FAILED;
				}
				popScope(table);
				break;
			}

			case STMT_REPEAT_TIMES: {
				pushScope(table);
				if (stmt->repeatTimes.body != NULL) {
					CompilationStatus bodyStatus =
						collectRoutineDeclarations(stmt->repeatTimes.body, table, logger);
					if (bodyStatus == FAILED) status = FAILED;
				}
				popScope(table);
				break;
			}

			case STMT_CALL:
			case STMT_ASSIGN:
			case STMT_WAIT:
				// No new declarations in these statements
				break;
		}

		cur = cur->next;
	}

	return status;
}

static CompilationStatus pass1(Program * program, SymbolTable * table, Logger * logger) {
	CompilationStatus status = SUCCEEDED;

	logDebugging(logger, "=== Semantic Analysis: Pass 1 (Declaration Collection) ===");

	// Collect hardware declarations
	CompilationStatus hwStatus = collectHardwareDeclarations(program, table, logger);
	if (hwStatus == FAILED) status = FAILED;

	// Collect routine-level declarations (variables, for-loop vars)
	if (program->routine != NULL && program->routine->stmts != NULL) {
		CompilationStatus rtStatus =
			collectRoutineDeclarations(program->routine->stmts, table, logger);
		if (rtStatus == FAILED) status = FAILED;
	}

	logDebugging(logger, "=== Pass 1 complete (status: %d) ===", status);
	return status;
}

// PASS 2: Type checking

/**
 * Look up the method table entry for a given component type and method name.
 * Returns NULL if no match found.
 */
static const MethodInfo * findMethod(ComponentType compType, const char * methodName) {
	for (int i = 0; i < COMPONENT_METHOD_COUNT; i++) {
		if (COMPONENT_METHODS[i].compType != compType) continue;
		for (int j = 0; j < COMPONENT_METHODS[i].methodCount; j++) {
			if (strcmp(COMPONENT_METHODS[i].methods[j].methodName, methodName) == 0) {
				return &COMPONENT_METHODS[i].methods[j];
			}
		}
	}
	return NULL;
}

/**
 * Check that a method call is valid: the object is a known hardware component,
 * the method exists, and the argument arity and types are correct.
 */
static CompilationStatus checkMethodSignature(
	const char * componentName,
	ComponentType compType,
	const char * method,
	ArgList * args,
	SymbolTable * table,
	Logger * logger)
{
	const MethodInfo * methodInfo = findMethod(compType, method);
	if (methodInfo == NULL) {
		const char * typeName = "unknown";
		for (int i = 0; i < COMPONENT_METHOD_COUNT; i++) {
			if (COMPONENT_METHODS[i].compType == compType) {
				typeName = COMPONENT_METHODS[i].typeName;
				break;
			}
		}
		logError(logger,
			"Semantic error: component '%s' of type %s has no method '%s'",
			componentName, typeName, method);
		return FAILED;
	}

	// Count actual arguments
	int argCount = 0;
	ArgList * a = args;
	while (a != NULL) {
		argCount++;
		a = a->next;
	}

	if (argCount != methodInfo->paramCount) {
		logError(logger,
			"Semantic error: method '%s' on component '%s' expects %d argument(s), "
			"but %d provided",
			method, componentName, methodInfo->paramCount, argCount);
		return FAILED;
	}

	// Type-check each argument
	int idx = 0;
	a = args;
	while (a != NULL) {
		CompilationStatus argStatus = SUCCEEDED;
		DataType argType = typeOf(a->expr, table, logger, &argStatus);
		if (argStatus == FAILED) {
			return FAILED;
		}

		if (methodInfo->paramTypes[idx] != TYPE_UNKNOWN &&
			argType != methodInfo->paramTypes[idx])
		{
			logError(logger,
				"Semantic error: argument %d of method '%s' on component '%s' "
				"expects type %d, but got type %d",
				idx + 1, method, componentName,
				methodInfo->paramTypes[idx], argType);
			return FAILED;
		}

		a = a->next;
		idx++;
	}

	return SUCCEEDED;
}

/**
 * Compute the data type of an expression recursively, bottom-up.
 * Sets *status to FAILED if a type error is detected.
 */
static DataType typeOf(Expr * expr, SymbolTable * table, Logger * logger,
	CompilationStatus * status)
{
	if (expr == NULL) {
		*status = FAILED;
		return TYPE_ERROR;
	}

	switch (expr->type) {
		case EXPR_INTEGER:
			return TYPE_INT;

		case EXPR_FLOAT:
			return TYPE_FLOAT;

		case EXPR_STRING:
			return TYPE_STRING;

		case EXPR_BOOL:
			return TYPE_BOOL;

		case EXPR_TIME:
			return TYPE_TIME;

		case EXPR_IDENTIFIER: {
			SymbolEntry * entry = lookupSymbol(table, expr->string);
			if (entry == NULL) {
				logError(logger,
					"Semantic error: identifier '%s' not declared",
					expr->string);
				*status = FAILED;
				return TYPE_ERROR;
			}
			return entry->dataType;
		}

		case EXPR_MEMBER: {
			// member access: object.member (ej: dht.temperature)
			SymbolEntry * obj = lookupSymbol(table, expr->member.object);
			if (obj == NULL) {
				logError(logger,
					"Semantic error: component '%s' not declared",
					expr->member.object);
				*status = FAILED;
				return TYPE_ERROR;
			}
			if (obj->kind != SYM_HARDWARE) {
				logError(logger,
					"Semantic error: '%s' is not a hardware component",
					expr->member.object);
				*status = FAILED;
				return TYPE_ERROR;
			}

			// For members like dht.temperature, treat as method call type
			// temperature -> read_temperature, humidity -> read_humidity
			const char * impliedMethod = expr->member.member;

			// Map member names to method names for known sensors
			if (obj->componentType == COMP_DHT11) {
				if (strcmp(impliedMethod, "temperature") == 0) {
					return TYPE_FLOAT;
				} else if (strcmp(impliedMethod, "humidity") == 0) {
					return TYPE_FLOAT;
				}
			}
			if (obj->componentType == COMP_ULTRASONIC) {
				if (strcmp(impliedMethod, "distance") == 0) {
					return TYPE_FLOAT;
				}
			}

			logError(logger,
				"Semantic error: component '%s' has no member '%s'",
				expr->member.object, expr->member.member);
			*status = FAILED;
			return TYPE_ERROR;
		}

		case EXPR_CALL: {
			SymbolEntry * obj = lookupSymbol(table, expr->call.object);
			if (obj == NULL) {
				logError(logger,
					"Semantic error: component '%s' not declared",
					expr->call.object);
				*status = FAILED;
				return TYPE_ERROR;
			}
			if (obj->kind != SYM_HARDWARE) {
				logError(logger,
					"Semantic error: '%s' is not a hardware component",
					expr->call.object);
				*status = FAILED;
				return TYPE_ERROR;
			}

			const MethodInfo * methodInfo = findMethod(
				obj->componentType, expr->call.method);
			if (methodInfo == NULL) {
				const char * typeName = "unknown";
				for (int i = 0; i < COMPONENT_METHOD_COUNT; i++) {
					if (COMPONENT_METHODS[i].compType == obj->componentType) {
						typeName = COMPONENT_METHODS[i].typeName;
						break;
					}
				}
				logError(logger,
					"Semantic error: component '%s' of type %s has no method '%s'",
					expr->call.object, typeName, expr->call.method);
				*status = FAILED;
				return TYPE_ERROR;
			}

			// Validate argument count and types
			CompilationStatus sigStatus = checkMethodSignature(
				expr->call.object, obj->componentType,
				expr->call.method, expr->call.args,
				table, logger);
			if (sigStatus == FAILED) {
				*status = FAILED;
				return TYPE_ERROR;
			}

			return methodInfo->returnType;
		}

		case EXPR_BINARY: {
			CompilationStatus leftStatus = SUCCEEDED;
			CompilationStatus rightStatus = SUCCEEDED;
			DataType leftType = typeOf(expr->binary.left, table, logger, &leftStatus);
			DataType rightType = typeOf(expr->binary.right, table, logger, &rightStatus);

			if (leftStatus == FAILED || rightStatus == FAILED) {
				*status = FAILED;
				return TYPE_ERROR;
			}

			switch (expr->binary.op) {
				case OP_ADD:
				case OP_SUB:
				case OP_MUL:
				case OP_DIV:
					if (leftType != TYPE_INT && leftType != TYPE_FLOAT) {
						logError(logger,
							"Semantic error: left operand of arithmetic operation "
							"must be numeric (got type %d)", leftType);
						*status = FAILED;
						return TYPE_ERROR;
					}
					if (rightType != TYPE_INT && rightType != TYPE_FLOAT) {
						logError(logger,
							"Semantic error: right operand of arithmetic operation "
							"must be numeric (got type %d)", rightType);
						*status = FAILED;
						return TYPE_ERROR;
					}
					// If either operand is float, result is float
					return (leftType == TYPE_FLOAT || rightType == TYPE_FLOAT)
						? TYPE_FLOAT : TYPE_INT;

				case OP_EQ:
				case OP_NE:
					// Allow numeric + numeric, or string + string, or bool + bool
					if (leftType != rightType) {
						logError(logger,
							"Semantic error: operands of equality comparison "
							"must have the same type (left: %d, right: %d)",
							leftType, rightType);
						*status = FAILED;
						return TYPE_ERROR;
					}
					return TYPE_BOOL;

				case OP_LT:
				case OP_GT:
				case OP_LE:
				case OP_GE:
					if ((leftType != TYPE_INT && leftType != TYPE_FLOAT) ||
						(rightType != TYPE_INT && rightType != TYPE_FLOAT))
					{
						logError(logger,
							"Semantic error: operands of relational comparison "
							"must be numeric (left: %d, right: %d)",
							leftType, rightType);
						*status = FAILED;
						return TYPE_ERROR;
					}
					return TYPE_BOOL;

				case OP_AND:
				case OP_OR:
					if (leftType != TYPE_BOOL) {
						logError(logger,
							"Semantic error: left operand of logical operator "
							"must be boolean (got %d)", leftType);
						*status = FAILED;
						return TYPE_ERROR;
					}
					if (rightType != TYPE_BOOL) {
						logError(logger,
							"Semantic error: right operand of logical operator "
							"must be boolean (got %d)", rightType);
						*status = FAILED;
						return TYPE_ERROR;
					}
					return TYPE_BOOL;

				default:
					*status = FAILED;
					return TYPE_ERROR;
			}
		}

		case EXPR_UNARY: {
			CompilationStatus operandStatus = SUCCEEDED;
			DataType operandType = typeOf(expr->unary.operand, table, logger, &operandStatus);

			if (operandStatus == FAILED) {
				*status = FAILED;
				return TYPE_ERROR;
			}

			if (expr->unary.op == OP_NOT) {
				if (operandType != TYPE_BOOL) {
					logError(logger,
						"Semantic error: operand of 'not' must be boolean (got %d)",
						operandType);
					*status = FAILED;
					return TYPE_ERROR;
				}
				return TYPE_BOOL;
			}

			if (expr->unary.op == OP_NEG) {
				if (operandType != TYPE_INT && operandType != TYPE_FLOAT) {
					logError(logger,
						"Semantic error: operand of negation "
						"must be numeric (got %d)", operandType);
					*status = FAILED;
					return TYPE_ERROR;
				}
				return operandType;
			}

			*status = FAILED;
			return TYPE_ERROR;
		}

		default:
			*status = FAILED;
			return TYPE_ERROR;
	}
}

/**
 * Validate a method call statement (STMT_CALL).
 */
static CompilationStatus validateExprStmtCall(
	char * object, char * method, ArgList * args,
	SymbolTable * table, Logger * logger)
{
	SymbolEntry * obj = lookupSymbol(table, object);
	if (obj == NULL) {
		logError(logger,
			"Semantic error: component '%s' not declared", object);
		return FAILED;
	}
	if (obj->kind != SYM_HARDWARE) {
		logError(logger,
			"Semantic error: '%s' is not a hardware component", object);
		return FAILED;
	}

	return checkMethodSignature(object, obj->componentType, method, args, table, logger);
}

/**
 * Recursively validate all statements in a list.
 */
static CompilationStatus validateStmt(Stmt * stmt, SymbolTable * table, Logger * logger) {
	if (stmt == NULL) return SUCCEEDED;

	CompilationStatus status = SUCCEEDED;

	switch (stmt->type) {
		case STMT_CALL: {
			CompilationStatus callStatus = validateExprStmtCall(
				stmt->call.object, stmt->call.method,
				stmt->call.args, table, logger);
			if (callStatus == FAILED) status = FAILED;
			break;
		}

		case STMT_VAR: {
			if (stmt->var.value != NULL) {
				CompilationStatus exprStatus = SUCCEEDED;
				DataType exprType = typeOf(stmt->var.value, table, logger, &exprStatus);
				if (exprStatus == FAILED) {
					status = FAILED;
				}
			}
			break;
		}

		case STMT_ASSIGN: {
			SymbolEntry * var = lookupSymbol(table, stmt->assign.name);
			if (var == NULL) {
				logError(logger,
					"Semantic error: variable '%s' not declared", stmt->assign.name);
				status = FAILED;
				break;
			}

			CompilationStatus exprStatus = SUCCEEDED;
			DataType exprType = typeOf(stmt->assign.value, table, logger, &exprStatus);
			if (exprStatus == FAILED) {
				status = FAILED;
				break;
			}

			if (var->dataType != TYPE_UNKNOWN && var->dataType != exprType) {
				logError(logger,
					"Semantic error: cannot assign expression of type %d "
					"to variable '%s' of type %d",
					exprType, stmt->assign.name, var->dataType);
				status = FAILED;
			}
			break;
		}

		case STMT_BLOCK: {
			// Scope was already pushed during pass 1; we need to push again for pass 2
			pushScope(table);
			if (stmt->block.body != NULL) {
				StmtList * cur = stmt->block.body;
				while (cur != NULL) {
					if (cur->stmt != NULL) {
						CompilationStatus s = validateStmt(cur->stmt, table, logger);
						if (s == FAILED) status = FAILED;
					}
					cur = cur->next;
				}
			}
			popScope(table);
			break;
		}

		case STMT_IF: {
			// Validate condition
			CompilationStatus condStatus = SUCCEEDED;
			DataType condType = typeOf(stmt->if_.cond, table, logger, &condStatus);
			if (condStatus == FAILED) {
				status = FAILED;
			} else if (condType != TYPE_BOOL) {
				logError(logger,
					"Semantic error: if condition must be boolean (got type %d)",
					condType);
				status = FAILED;
			}

			// Validate then branch
			pushScope(table);
			if (stmt->if_.thenBranch != NULL) {
				StmtList * cur = stmt->if_.thenBranch;
				while (cur != NULL) {
					if (cur->stmt != NULL) {
						CompilationStatus s = validateStmt(cur->stmt, table, logger);
						if (s == FAILED) status = FAILED;
					}
					cur = cur->next;
				}
			}
			popScope(table);

			// Validate else branch
			pushScope(table);
			if (stmt->if_.elseBranch != NULL) {
				StmtList * cur = stmt->if_.elseBranch;
				while (cur != NULL) {
					if (cur->stmt != NULL) {
						CompilationStatus s = validateStmt(cur->stmt, table, logger);
						if (s == FAILED) status = FAILED;
					}
					cur = cur->next;
				}
			}
			popScope(table);
			break;
		}

		case STMT_WAIT: {
			// Wait requires a time literal
			if (stmt->wait.timeLiteral == NULL) {
				logError(logger,
					"Semantic error: wait statement requires a time literal");
				status = FAILED;
			}
			break;
		}

		case STMT_REPEAT_EVERY: {
			if (stmt->repeatEvery.timeLiteral == NULL) {
				logError(logger,
					"Semantic error: repeat_every statement requires a time literal");
				status = FAILED;
			}
			pushScope(table);
			if (stmt->repeatEvery.body != NULL) {
				StmtList * cur = stmt->repeatEvery.body;
				while (cur != NULL) {
					if (cur->stmt != NULL) {
						CompilationStatus s = validateStmt(cur->stmt, table, logger);
						if (s == FAILED) status = FAILED;
					}
					cur = cur->next;
				}
			}
			popScope(table);
			break;
		}

		case STMT_REPEAT_TIMES: {
			pushScope(table);
			if (stmt->repeatTimes.body != NULL) {
				StmtList * cur = stmt->repeatTimes.body;
				while (cur != NULL) {
					if (cur->stmt != NULL) {
						CompilationStatus s = validateStmt(cur->stmt, table, logger);
						if (s == FAILED) status = FAILED;
					}
					cur = cur->next;
				}
			}
			popScope(table);
			break;
		}

		case STMT_FOR_RANGE: {
			// Validate from and to expressions
			CompilationStatus fromStatus = SUCCEEDED;
			DataType fromType = typeOf(stmt->forRange.from, table, logger, &fromStatus);
			if (fromStatus == FAILED) {
				status = FAILED;
			} else if (fromType != TYPE_INT && fromType != TYPE_FLOAT) {
				logError(logger,
					"Semantic error: for-range 'from' must be numeric (got %d)",
					fromType);
				status = FAILED;
			}

			CompilationStatus toStatus = SUCCEEDED;
			DataType toType = typeOf(stmt->forRange.to, table, logger, &toStatus);
			if (toStatus == FAILED) {
				status = FAILED;
			} else if (toType != TYPE_INT && toType != TYPE_FLOAT) {
				logError(logger,
					"Semantic error: for-range 'to' must be numeric (got %d)",
					toType);
				status = FAILED;
			}

			pushScope(table);
			if (stmt->forRange.body != NULL) {
				StmtList * cur = stmt->forRange.body;
				while (cur != NULL) {
					if (cur->stmt != NULL) {
						CompilationStatus s = validateStmt(cur->stmt, table, logger);
						if (s == FAILED) status = FAILED;
					}
					cur = cur->next;
				}
			}
			popScope(table);
			break;
		}
	}

	return status;
}

static CompilationStatus pass2(Program * program, SymbolTable * table, Logger * logger) {
	CompilationStatus status = SUCCEEDED;

	logDebugging(logger, "=== Semantic Analysis: Pass 2 (Type Checking) ===");

	if (program->routine != NULL && program->routine->stmts != NULL) {
		StmtList * cur = program->routine->stmts;
		while (cur != NULL) {
			if (cur->stmt != NULL) {
				CompilationStatus stmtStatus = validateStmt(cur->stmt, table, logger);
				if (stmtStatus == FAILED) status = FAILED;
			}
			cur = cur->next;
		}
	}

	logDebugging(logger, "=== Pass 2 complete (status: %d) ===", status);
	return status;
}

// executeSemanticAnalysis

CompilationStatus executeSemanticAnalysis(CompilerState * state) {
	if (state == NULL) {
		logCritical(semanticLogger,
			"executeSemanticAnalysis: compiler state is NULL");
		return FAILED;
	}

	Logger * logger = (state->logger != NULL) ? state->logger : semanticLogger;

	if (state->abstractSyntaxtTree == NULL) {
		logError(logger,
			"executeSemanticAnalysis: AST is NULL — nothing to analyze");
		return FAILED;
	}

	Program * program = (Program *) state->abstractSyntaxtTree;

	// Ensure we have a symbol table
	if (state->symbolTable == NULL) {
		state->symbolTable = createSymbolTable(64, logger);
		if (state->symbolTable == NULL) {
			logCritical(logger,
				"executeSemanticAnalysis: failed to create symbol table");
			return FAILED;
		}
	}

	// Reset the global pin list
	clearPinList();

	CompilationStatus status = SUCCEEDED;

	// Enable preserve mode so pass 1's popScope does not destroy entries.
	// This way pass 2 can still find symbols declared in nested scopes.
	setPreserveMode(state->symbolTable, true);

	// Pass 1: collect declarations
	CompilationStatus p1 = pass1(program, state->symbolTable, logger);
	if (p1 == FAILED) status = FAILED;

	// Pass 2: type checking (only if pass 1 succeeded)
	// Preserve mode stays on so pass 2's own pushScope/popScope also
	// does not destroy entries — destroySymbolTable will clean up at the end.
	if (status == SUCCEEDED) {
		CompilationStatus p2 = pass2(program, state->symbolTable, logger);
		if (p2 == FAILED) status = FAILED;
	}

	// Clean up the global pin list
	clearPinList();

	logInformation(logger,
		"Semantic analysis completed with status: %s",
		(status == SUCCEEDED) ? "SUCCEEDED" : "FAILED");

	return status;
}
#include "BisonActions.h"
#include <string.h>

/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Logger * _logger = NULL;

void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState) {
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	return _shutdownBisonActionsModule;
}

/* PRIVATE FUNCTIONS */

static void _log(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

static StmtList * _appendToStmtList(StmtList * list, Stmt * stmt) {
	StmtList * node = calloc(1, sizeof(StmtList));
	node->stmt = stmt;
	if (list == NULL) return node;
	StmtList * tail = list;
	while (tail->next != NULL) tail = tail->next;
	tail->next = node;
	return list;
}

/* PUBLIC FUNCTIONS */

/* --- Hardware --- */

ComponentType ComponentTypeSemanticAction(TokenLabel token) {
	_log(__FUNCTION__);
	switch (token) {
		case LED:           return COMP_LED;
		case BUZZER:        return COMP_BUZZER;
		case BUTTON:        return COMP_BUTTON;
		case POTENTIOMETER: return COMP_POTENTIOMETER;
		case SERVO:         return COMP_SERVO;
		case ULTRASONIC:    return COMP_ULTRASONIC;
		case DHT11:         return COMP_DHT11;
		case LCD:           return COMP_LCD;
		default:            return COMP_LED;
	}
}

PinSpec * IntegerPinSpecSemanticAction(int pin) {
	_log(__FUNCTION__);
	PinSpec * ps = calloc(1, sizeof(PinSpec));
	ps->type = PIN_SPEC_SINGLE_INT;
	ps->singlePin = pin;
	return ps;
}

PinSpec * IdentifierPinSpecSemanticAction(char * pinIdentifier) {
	_log(__FUNCTION__);
	PinSpec * ps = calloc(1, sizeof(PinSpec));
	ps->type = PIN_SPEC_SINGLE_IDENTIFIER;
	ps->singleIdentifier = pinIdentifier;
	return ps;
}

PinSpec * NamedListPinSpecSemanticAction(PinSpecEntry * entries) {
	_log(__FUNCTION__);
	PinSpec * ps = calloc(1, sizeof(PinSpec));
	ps->type = PIN_SPEC_NAMED_LIST;
	ps->namedPins = entries;
	return ps;
}

PinSpec * IntListPinSpecSemanticAction(PinSpecEntry * entries) {
	_log(__FUNCTION__);
	PinSpec * ps = calloc(1, sizeof(PinSpec));
	ps->type = PIN_SPEC_INT_LIST;
	ps->intPins = entries;
	return ps;
}

PinSpecEntry * NewPinSpecEntryListSemanticAction(PinSpecEntry * entry) {
	_log(__FUNCTION__);
	return entry;
}

PinSpecEntry * AppendPinSpecEntryListSemanticAction(PinSpecEntry * list, PinSpecEntry * entry) {
	_log(__FUNCTION__);
	if (list == NULL) return entry;
	PinSpecEntry * tail = list;
	while (tail->next != NULL) tail = tail->next;
	tail->next = entry;
	return list;
}

PinSpecEntry * NamedPinEntrySemanticAction(char * name, int value) {
	_log(__FUNCTION__);
	PinSpecEntry * e = calloc(1, sizeof(PinSpecEntry));
	e->name = name;
	e->value = value;
	return e;
}

PinSpecEntry * IntPinEntrySemanticAction(int value) {
	_log(__FUNCTION__);
	PinSpecEntry * e = calloc(1, sizeof(PinSpecEntry));
	e->name = NULL;
	e->value = value;
	return e;
}

HardwareDecl * HardwareDeclSemanticAction(ComponentType ct, char * identifier, PinSpec * pinSpec) {
	_log(__FUNCTION__);
	HardwareDecl * d = calloc(1, sizeof(HardwareDecl));
	d->componentType = ct;
	d->identifier = identifier;
	d->pinSpec = pinSpec;
	return d;
}

HardwareDeclList * NewHardwareDeclListSemanticAction(HardwareDecl * decl) {
	_log(__FUNCTION__);
	HardwareDeclList * node = calloc(1, sizeof(HardwareDeclList));
	node->declaration = decl;
	return node;
}

HardwareDeclList * AppendHardwareDeclListSemanticAction(HardwareDeclList * list, HardwareDecl * decl) {
	_log(__FUNCTION__);
	HardwareDeclList * node = calloc(1, sizeof(HardwareDeclList));
	node->declaration = decl;
	if (list == NULL) return node;
	HardwareDeclList * tail = list;
	while (tail->next != NULL) tail = tail->next;
	tail->next = node;
	return list;
}

HardwareBlock * HardwareBlockSemanticAction(HardwareDeclList * decls) {
	_log(__FUNCTION__);
	HardwareBlock * b = calloc(1, sizeof(HardwareBlock));
	b->declarations = decls;
	return b;
}

/* --- Routine --- */

RoutineBlock * RoutineBlockSemanticAction(StmtList * stmts) {
	_log(__FUNCTION__);
	RoutineBlock * b = calloc(1, sizeof(RoutineBlock));
	b->stmts = stmts;
	return b;
}

/* --- Statements --- */

StmtList * AppendStmtSemanticAction(StmtList * list, Stmt * stmt) {
	_log(__FUNCTION__);
	return _appendToStmtList(list, stmt);
}

Stmt * CallStmtSemanticAction(char * object, char * method, ArgList * args) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_CALL;
	s->call.object = object;
	s->call.method = method;
	s->call.args = args;
	return s;
}

Stmt * VarStmtSemanticAction(char * name, Expr * value) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_VAR;
	s->var.name = name;
	s->var.value = value;
	return s;
}

Stmt * IfStmtSemanticAction(Expr * cond, StmtList * thenBranch, StmtList * elseBranch) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_IF;
	s->if_.cond = cond;
	s->if_.thenBranch = thenBranch;
	s->if_.elseBranch = elseBranch;
	return s;
}

Stmt * RepeatEveryStmtSemanticAction(char * timeLiteral, StmtList * body) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_REPEAT_EVERY;
	s->repeatEvery.timeLiteral = timeLiteral;
	s->repeatEvery.body = body;
	return s;
}

Stmt * RepeatTimesStmtSemanticAction(int count, StmtList * body) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_REPEAT_TIMES;
	s->repeatTimes.count = count;
	s->repeatTimes.body = body;
	return s;
}

Stmt * WaitStmtSemanticAction(char * timeLiteral) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_WAIT;
	s->wait.timeLiteral = timeLiteral;
	return s;
}

Stmt * ForRangeStmtSemanticAction(char * varName, Expr * from, Expr * to, StmtList * body) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_FOR_RANGE;
	s->forRange.varName = varName;
	s->forRange.from = from;
	s->forRange.to = to;
	s->forRange.body = body;
	return s;
}

/* --- Arguments --- */

ArgList * SingleArgSemanticAction(Expr * expr) {
	_log(__FUNCTION__);
	ArgList * a = calloc(1, sizeof(ArgList));
	a->expr = expr;
	return a;
}

ArgList * AppendArgSemanticAction(ArgList * list, Expr * expr) {
	_log(__FUNCTION__);
	ArgList * node = calloc(1, sizeof(ArgList));
	node->expr = expr;
	if (list == NULL) return node;
	ArgList * tail = list;
	while (tail->next != NULL) tail = tail->next;
	tail->next = node;
	return list;
}

/* --- Expressions --- */

Expr * BinaryExprSemanticAction(Expr * left, Operator op, Expr * right) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_BINARY;
	e->binary.op = op;
	e->binary.left = left;
	e->binary.right = right;
	return e;
}

Expr * UnaryExprSemanticAction(Operator op, Expr * operand) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_UNARY;
	e->unary.op = op;
	e->unary.operand = operand;
	return e;
}

Expr * MemberExprSemanticAction(char * object, char * member) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_MEMBER;
	e->member.object = object;
	e->member.member = member;
	return e;
}

Expr * CallExprSemanticAction(char * object, char * method, ArgList * args) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_CALL;
	e->call.object = object;
	e->call.method = method;
	e->call.args = args;
	return e;
}

Expr * IdentifierExprSemanticAction(char * name) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_IDENTIFIER;
	e->string = name;
	return e;
}

Expr * IntegerExprSemanticAction(int value) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_INTEGER;
	e->integer = value;
	return e;
}

Expr * FloatExprSemanticAction(double value) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_FLOAT;
	e->number = value;
	return e;
}

Expr * StringExprSemanticAction(char * value) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_STRING;
	e->string = value;
	return e;
}

Expr * BoolExprSemanticAction(bool value) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_BOOL;
	e->boolean = value;
	return e;
}

Expr * TimeExprSemanticAction(char * timeLiteral) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_TIME;
	e->string = timeLiteral;
	return e;
}

/* --- Program --- */

Program * ProgramSemanticAction(HardwareBlock * hardware, RoutineBlock * routine) {
	_log(__FUNCTION__);
	Program * p = calloc(1, sizeof(Program));
	p->hardware = hardware;
	p->routine = routine;
	_compilerState->abstractSyntaxtTree = p;
	return p;
}

// EZduino semantic actions.

EzLiteral * EzIntLiteralSemanticAction(int value) {
	_log(__FUNCTION__);
	return createEzIntLiteral(value);
}

EzLiteral * EzFloatLiteralSemanticAction(double value) {
	_log(__FUNCTION__);
	return createEzFloatLiteral(value);
}

EzLiteral * EzBoolLiteralSemanticAction(bool value) {
	_log(__FUNCTION__);
	return createEzBoolLiteral(value);
}

EzLiteral * EzStringLiteralSemanticAction(char * value) {
	_log(__FUNCTION__);
	return createEzStringLiteral(value);
}

EzLiteral * EzTimeLiteralSemanticAction(EzTimeValue value) {
	_log(__FUNCTION__);
	return createEzTimeLiteral(value);
}
refactor: simplify logging in EZduino semantic actions
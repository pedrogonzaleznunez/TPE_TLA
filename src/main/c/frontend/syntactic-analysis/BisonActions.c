#include "BisonActions.h"

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

Pin DigitalPinSemanticAction(int number) {
	_log(__FUNCTION__);
	return (Pin){ .kind = PIN_DIGITAL, .number = number };
}

Pin AnalogPinSemanticAction(int number) {
	_log(__FUNCTION__);
	return (Pin){ .kind = PIN_ANALOG, .number = number };
}

HardwareDecl * SimpleHardwareDeclSemanticAction(HwDeviceType device, char * name, Pin pin) {
	_log(__FUNCTION__);
	HardwareDecl * d = calloc(1, sizeof(HardwareDecl));
	d->kind = HD_SIMPLE;
	d->name = name;
	d->simple.device = device;
	d->simple.pin = pin;
	return d;
}

HardwareDecl * UltrasonicHardwareDeclSemanticAction(char * name, Pin trig, Pin echo) {
	_log(__FUNCTION__);
	HardwareDecl * d = calloc(1, sizeof(HardwareDecl));
	d->kind = HD_ULTRASONIC;
	d->name = name;
	d->ultrasonic.trig = trig;
	d->ultrasonic.echo = echo;
	return d;
}

HardwareDecl * LcdHardwareDeclSemanticAction(char * name) {
	_log(__FUNCTION__);
	HardwareDecl * d = calloc(1, sizeof(HardwareDecl));
	d->kind = HD_LCD;
	d->name = name;
	return d;
}

HardwareDecl * AppendHardwareDeclSemanticAction(HardwareDecl * list, HardwareDecl * decl) {
	_log(__FUNCTION__);
	if (list == NULL) return decl;
	HardwareDecl * tail = list;
	while (tail->next != NULL) tail = tail->next;
	tail->next = decl;
	return list;
}

HardwareBlock * HardwareBlockSemanticAction(HardwareDecl * decls) {
	_log(__FUNCTION__);
	HardwareBlock * b = calloc(1, sizeof(HardwareBlock));
	b->decls = decls;
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

Stmt * RepeatEveryStmtSemanticAction(StmtList * body, int intervalMs) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_REPEAT_EVERY;
	s->repeatEvery.body = body;
	s->repeatEvery.intervalMs = intervalMs;
	return s;
}

Stmt * WaitStmtSemanticAction(int delayMs) {
	_log(__FUNCTION__);
	Stmt * s = calloc(1, sizeof(Stmt));
	s->type = STMT_WAIT;
	s->wait.delayMs = delayMs;
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

/* --- Expressions --- */

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

Expr * BinaryExprSemanticAction(Expr * left, Operator op, Expr * right) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_BINARY;
	e->binary.left = left;
	e->binary.op = op;
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

Expr * IdentifierExprSemanticAction(char * name) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_IDENTIFIER;
	e->identifier = name;
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

Expr * MemberExprSemanticAction(char * object, char * member) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_MEMBER;
	e->member.object = object;
	e->member.member = member;
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

Expr * TimeExprSemanticAction(int timeMs) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_TIME;
	e->timeMs = timeMs;
	return e;
}

Expr * AnalogPinExprSemanticAction(int pin) {
	_log(__FUNCTION__);
	Expr * e = calloc(1, sizeof(Expr));
	e->type = EXPR_ANALOG;
	e->analogPin = pin;
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

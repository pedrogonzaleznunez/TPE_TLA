#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdbool.h>
#include <stdlib.h>

ModuleDestructor initializeAbstractSyntaxTreeModule();

/* Forward declarations */
typedef struct HardwareBlock HardwareBlock;
typedef struct HardwareDecl HardwareDecl;
typedef struct RoutineBlock RoutineBlock;
typedef struct Stmt Stmt;
typedef struct StmtList StmtList;
typedef struct Expr Expr;
typedef struct ArgList ArgList;
typedef struct Program Program;

/* ------------------------------------------------------------------ */
/* Pin                                                                  */
/* ------------------------------------------------------------------ */

typedef enum { PIN_DIGITAL, PIN_ANALOG } PinKind;

typedef struct {
	PinKind kind;
	int number;
} Pin;

/* ------------------------------------------------------------------ */
/* Hardware                                                             */
/* ------------------------------------------------------------------ */

typedef enum {
	HW_LED, HW_BUZZER, HW_BUTTON, HW_POTENTIOMETER, HW_SERVO, HW_DHT11
} HwDeviceType;

typedef enum { HD_SIMPLE, HD_ULTRASONIC, HD_LCD } HwDeclKind;

struct HardwareDecl {
	HwDeclKind kind;
	char * name;
	union {
		struct { HwDeviceType device; Pin pin; } simple;
		struct { Pin trig; Pin echo; } ultrasonic;
		/* HD_LCD: only name is used */
	};
	HardwareDecl * next;
};

struct HardwareBlock {
	HardwareDecl * decls;
};

/* ------------------------------------------------------------------ */
/* Expressions                                                          */
/* ------------------------------------------------------------------ */

typedef enum {
	OP_ADD, OP_SUB, OP_MUL, OP_DIV,
	OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE,
	OP_AND, OP_OR, OP_NOT, OP_NEG,
} Operator;

typedef enum {
	EXPR_INTEGER, EXPR_FLOAT, EXPR_STRING, EXPR_BOOL, EXPR_TIME, EXPR_ANALOG,
	EXPR_IDENTIFIER, EXPR_MEMBER, EXPR_CALL, EXPR_BINARY, EXPR_UNARY,
} ExprType;

struct Expr {
	ExprType type;
	union {
		int integer;
		double number;
		char * string;
		bool boolean;
		int timeMs;
		int analogPin;
		char * identifier;
		struct { char * object; char * member; } member;
		struct { char * object; char * method; ArgList * args; } call;
		struct { Operator op; Expr * left; Expr * right; } binary;
		struct { Operator op; Expr * operand; } unary;
	};
};

/* ------------------------------------------------------------------ */
/* Argument list                                                        */
/* ------------------------------------------------------------------ */

struct ArgList {
	Expr * expr;
	ArgList * next;
};

/* ------------------------------------------------------------------ */
/* Statements                                                           */
/* ------------------------------------------------------------------ */

typedef enum {
	STMT_CALL, STMT_VAR, STMT_IF, STMT_REPEAT_EVERY, STMT_WAIT, STMT_FOR_RANGE,
} StmtType;

struct StmtList {
	Stmt * stmt;
	StmtList * next;
};

struct Stmt {
	StmtType type;
	union {
		struct { char * object; char * method; ArgList * args; } call;
		struct { char * name; Expr * value; } var;
		struct { Expr * cond; StmtList * thenBranch; StmtList * elseBranch; } if_;
		struct { StmtList * body; int intervalMs; } repeatEvery;
		struct { int delayMs; } wait;
		struct { char * varName; Expr * from; Expr * to; StmtList * body; } forRange;
	};
};

/* ------------------------------------------------------------------ */
/* Routine / Program                                                    */
/* ------------------------------------------------------------------ */

struct RoutineBlock {
	StmtList * stmts;
};

struct Program {
	HardwareBlock * hardware;
	RoutineBlock * routine;
};

/* ------------------------------------------------------------------ */
/* Destructors                                                          */
/* ------------------------------------------------------------------ */

void destroyHardwareDecl(HardwareDecl * decl);
void destroyHardwareBlock(HardwareBlock * block);
void destroyExpr(Expr * expr);
void destroyArgList(ArgList * list);
void destroyStmt(Stmt * stmt);
void destroyStmtList(StmtList * list);
void destroyRoutineBlock(RoutineBlock * block);
void destroyProgram(Program * program);

#endif

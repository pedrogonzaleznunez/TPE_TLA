#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdbool.h>
#include <stdlib.h>

ModuleDestructor initializeAbstractSyntaxTreeModule();

/* Forward declarations */
typedef struct Expr Expr;
typedef struct ArgList ArgList;
typedef struct Stmt Stmt;
typedef struct StmtList StmtList;
typedef struct PinSpecEntry PinSpecEntry;
typedef struct PinSpec PinSpec;
typedef struct HardwareDecl HardwareDecl;
typedef struct HardwareDeclList HardwareDeclList;
typedef struct HardwareBlock HardwareBlock;
typedef struct RoutineBlock RoutineBlock;
typedef struct Program Program;

/* ------------------------------------------------------------------ */
/* Expressions                                                          */
/* ------------------------------------------------------------------ */

typedef enum {
	OP_ADD, OP_SUB, OP_MUL, OP_DIV,
	OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE,
	OP_AND, OP_OR, OP_NOT, OP_NEG,
} Operator;

typedef enum {
	EXPR_INTEGER, EXPR_FLOAT, EXPR_STRING, EXPR_BOOL, EXPR_TIME,
	EXPR_IDENTIFIER, EXPR_MEMBER, EXPR_CALL, EXPR_BINARY, EXPR_UNARY,
} ExprType;

struct ArgList {
	Expr * expr;
	ArgList * next;
};

struct Expr {
	ExprType type;
	union {
		int integer;
		double number;
		char * string;	/* EXPR_STRING, EXPR_IDENTIFIER, EXPR_TIME */
		bool boolean;
		struct { char * object; char * member; } member;
		struct { char * object; char * method; ArgList * args; } call;
		struct { Operator op; Expr * left; Expr * right; } binary;
		struct { Operator op; Expr * operand; } unary;
	};
};

/* ------------------------------------------------------------------ */
/* Statements                                                           */
/* ------------------------------------------------------------------ */

typedef enum {
	STMT_CALL, STMT_VAR, STMT_ASSIGN, STMT_BLOCK, STMT_IF,
	STMT_REPEAT_EVERY, STMT_REPEAT_TIMES, STMT_WAIT, STMT_FOR_RANGE,
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
		struct { char * name; Expr * value; } assign;
		struct { StmtList * body; } block;
		struct { Expr * cond; StmtList * thenBranch; StmtList * elseBranch; } if_;
		struct { StmtList * body; char * timeLiteral; } repeatEvery;
		struct { int count; StmtList * body; } repeatTimes;
		struct { char * timeLiteral; } wait;
		struct { char * varName; Expr * from; Expr * to; StmtList * body; } forRange;
	};
};

/* ------------------------------------------------------------------ */
/* Hardware                  										  */
/* ------------------------------------------------------------------ */

typedef enum {
	COMP_LED, COMP_BUZZER, COMP_BUTTON, COMP_POTENTIOMETER,
	COMP_SERVO, COMP_ULTRASONIC, COMP_DHT11, COMP_LCD
} ComponentType;

typedef enum {
	PIN_SPEC_SINGLE_INT,
	PIN_SPEC_SINGLE_IDENTIFIER,
	PIN_SPEC_NAMED_LIST,
	PIN_SPEC_INT_LIST
} PinSpecType;

struct PinSpecEntry {
	char * name;
	int value;
	PinSpecEntry * next;
};

struct PinSpec {
	PinSpecType type;
	union {
		int singlePin;
		char * singleIdentifier;
		PinSpecEntry * namedPins;
		PinSpecEntry * intPins;
	};
};

struct HardwareDecl {
	ComponentType componentType;
	char * identifier;
	PinSpec * pinSpec;
};

struct HardwareDeclList {
	HardwareDecl * declaration;
	HardwareDeclList * next;
};

struct HardwareBlock {
	HardwareDeclList * declarations;
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

void destroyExpr(Expr * expr);
void destroyArgList(ArgList * list);
void destroyStmt(Stmt * stmt);
void destroyStmtList(StmtList * list);
void destroyPinSpecEntry(PinSpecEntry * entry);
void destroyPinSpecEntries(PinSpecEntry * entries);
void destroyPinSpec(PinSpec * pinSpec);
void destroyHardwareDecl(HardwareDecl * decl);
void destroyHardwareDeclList(HardwareDeclList * list);
void destroyHardwareBlock(HardwareBlock * block);
void destroyRoutineBlock(RoutineBlock * block);
void destroyProgram(Program * program);

#endif

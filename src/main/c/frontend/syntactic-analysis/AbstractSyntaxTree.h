#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeAbstractSyntaxTreeModule();

/**
 * This type definitions allows self-referencing types (e.g., an expression
 * that is made of another expressions, such as talking about you in 3rd
 * person, but without the madness).
 */

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION
};

enum FactorType {
	CONSTANT,
	EXPRESSION
};

struct Constant {
	int value;
};

struct Factor {
	union {
		Constant * constant;
		Expression * expression;
	};
	FactorType type;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
	};
	ExpressionType type;
};

struct Program {
	Expression * expression;
};

/**
 * Node recursive super-duper-trambolik-destructors.
 */

void destroyConstant(Constant * constant);
void destroyExpression(Expression * expression);
void destroyFactor(Factor * factor);
void destroyProgram(Program * program);

// EZduino AST types

typedef enum EzLiteralType EzLiteralType;
typedef enum EzTimeUnit EzTimeUnit;

typedef struct EzLiteral EzLiteral;

enum EzTimeUnit {
	EZ_TIME_MS,
	EZ_TIME_S
};

typedef struct {
	int value;
	EzTimeUnit unit;
} EzTimeValue;

enum EzLiteralType {
	EZ_LIT_INT,
	EZ_LIT_FLOAT,
	EZ_LIT_BOOL,
	EZ_LIT_STRING,
	EZ_LIT_TIME
};

struct EzLiteral {
	EzLiteralType type;
	union {
		int intValue;
		double floatValue;
		bool boolValue;
		char * stringValue;
		EzTimeValue timeValue;
	};
};

// EZduino AST constructors

EzLiteral * createEzIntLiteral(int value);
EzLiteral * createEzFloatLiteral(double value);
EzLiteral * createEzBoolLiteral(bool value);
EzLiteral * createEzStringLiteral(char * value);
EzLiteral * createEzTimeLiteral(EzTimeValue value);

// EZduino AST destructors

void destroyEzLiteral(EzLiteral * literal);

#endif

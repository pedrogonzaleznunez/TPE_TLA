#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: AbstractSyntaxTree...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntaxTree");
	return _shutdownAbstractSyntaxTreeModule;
}

/* PUBLIC FUNCTIONS */

void destroyConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void destroyExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				destroyExpression(expression->leftExpression);
				destroyExpression(expression->rightExpression);
				break;
			case FACTOR:
				destroyFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void destroyFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				destroyConstant(factor->constant);
				break;
			case EXPRESSION:
				destroyExpression(factor->expression);
				break;
		}
		free(factor);
	}
}

void destroyProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		destroyExpression(program->expression);
		free(program);
	}
}

// EZduino AST construnctors

EzLiteral * createEzIntLiteral(int value) {
	EzLiteral * literal = calloc(1, sizeof(EzLiteral));
	literal->type = EZ_LIT_INT;
	literal->intValue = value;
	return literal;
}

EzLiteral * createEzFloatLiteral(double value) {
	EzLiteral * literal = calloc(1, sizeof(EzLiteral));
	literal->type = EZ_LIT_FLOAT;
	literal->floatValue = value;
	return literal;
}

EzLiteral * createEzBoolLiteral(bool value) {
	EzLiteral * literal = calloc(1, sizeof(EzLiteral));
	literal->type = EZ_LIT_BOOL;
	literal->boolValue = value;
	return literal;
}

EzLiteral * createEzStringLiteral(char * value) {
	EzLiteral * literal = calloc(1, sizeof(EzLiteral));
	literal->type = EZ_LIT_STRING;
	literal->stringValue = value;
	return literal;
}

EzLiteral * createEzTimeLiteral(EzTimeValue value) {
	EzLiteral * literal = calloc(1, sizeof(EzLiteral));
	literal->type = EZ_LIT_TIME;
	literal->timeValue = value;
	return literal;
}

// EZduino AST destructors

void destroyEzLiteral(EzLiteral * literal) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (literal != NULL) {
		if (literal->type == EZ_LIT_STRING) {
			free(literal->stringValue);
		}
		free(literal);
	}
}
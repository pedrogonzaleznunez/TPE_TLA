#include "Generator.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/logging/Logger.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

typedef struct {
	FILE * out;
	int indent;
	int repeatEveryCounter;
	bool needsLiquidCrystal;
	bool needsDht;
	bool needsServo;
	bool needsUltrasonic;
} GeneratorContext;

/** Shutdown module's internal state. */
void _shutdownGeneratorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Generator...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeGeneratorModule() {
	_logger = createLogger("Generator");
	return _shutdownGeneratorModule;
}

/* HELPERS */

static void output(GeneratorContext * ctx, const char * format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(ctx->out, format, args);
	va_end(args);
}

static void indent(GeneratorContext * ctx) {
	for (int i = 0; i < ctx->indent; ++i) {
		output(ctx, "  ");
	}
}

static void formatPin(int pin, char * buf, size_t bufSize) {
	if (pin < 0) {
		snprintf(buf, bufSize, "A%d", -(pin + 1));
	}
	else {
		snprintf(buf, bufSize, "%d", pin);
	}
}

static char * strdupUpper(const char * name) {
	if (name == NULL) {
		return NULL;
	}
	size_t len = strlen(name);
	char * upper = (char *)malloc(len + 1);
	if (upper == NULL) {
		return NULL;
	}
	for (size_t i = 0; i < len; ++i) {
		upper[i] = (char)toupper((unsigned char)name[i]);
	}
	upper[len] = '\0';
	return upper;
}

static int parseTimeLiteralMs(const char * timeLiteral) {
	if (timeLiteral == NULL) {
		return 0;
	}
	char * end = NULL;
	long value = strtol(timeLiteral, &end, 10);
	if (end != NULL && strcmp(end, "s") == 0) {
		return (int)(value * 1000);
	}
	return (int)value;
}

static int extractSinglePin(PinSpec * pinSpec) {
	if (pinSpec == NULL) {
		return 0;
	}
	switch (pinSpec->type) {
		case PIN_SPEC_SINGLE_INT:
			return pinSpec->singlePin;
		case PIN_SPEC_SINGLE_IDENTIFIER: {
			const char * id = pinSpec->singleIdentifier;
			if (id != NULL && id[0] == 'A') {
				return -(atoi(id + 1) + 1);
			}
			return id != NULL ? atoi(id) : 0;
		}
		default:
			return 0;
	}
}

static int findNamedPin(PinSpec * pinSpec, const char * pinName) {
	if (pinSpec == NULL || pinSpec->type != PIN_SPEC_NAMED_LIST) {
		return 0;
	}
	for (PinSpecEntry * entry = pinSpec->namedPins; entry != NULL; entry = entry->next) {
		if (entry->name != NULL && strcmp(entry->name, pinName) == 0) {
			return entry->value;
		}
	}
	return 0;
}

static void scanHardwareNeeds(HardwareDeclList * decls, GeneratorContext * ctx) {
	for (HardwareDeclList * node = decls; node != NULL; node = node->next) {
		HardwareDecl * decl = node->declaration;
		if (decl == NULL) {
			continue;
		}
		switch (decl->componentType) {
			case COMP_LCD:
				ctx->needsLiquidCrystal = true;
				break;
			case COMP_DHT11:
				ctx->needsDht = true;
				break;
			case COMP_SERVO:
				ctx->needsServo = true;
				break;
			case COMP_ULTRASONIC:
				ctx->needsUltrasonic = true;
				break;
			default:
				break;
		}
	}
}

static void countRepeatEvery(StmtList * stmts, int * count) {
	for (StmtList * node = stmts; node != NULL; node = node->next) {
		Stmt * stmt = node->stmt;
		if (stmt == NULL) {
			continue;
		}
		switch (stmt->type) {
			case STMT_REPEAT_EVERY:
				(*count)++;
				countRepeatEvery(stmt->repeatEvery.body, count);
				break;
			case STMT_BLOCK:
				countRepeatEvery(stmt->block.body, count);
				break;
			case STMT_IF:
				countRepeatEvery(stmt->if_.thenBranch, count);
				countRepeatEvery(stmt->if_.elseBranch, count);
				break;
			case STMT_REPEAT_TIMES:
				countRepeatEvery(stmt->repeatTimes.body, count);
				break;
			case STMT_FOR_RANGE:
				countRepeatEvery(stmt->forRange.body, count);
				break;
			default:
				break;
		}
	}
}

/* FORWARD DECLARATIONS */

static void generateExpr(GeneratorContext * ctx, Expr * expr);
static void generateStmt(GeneratorContext * ctx, Stmt * stmt);
static void generateStmtList(GeneratorContext * ctx, StmtList * stmts);
static void generateMethodCall(
	GeneratorContext * ctx,
	const char * object,
	const char * method,
	ArgList * args,
	bool asStatement
);

/* EXPRESSIONS */

static void generateArgList(GeneratorContext * ctx, ArgList * args) {
	if (args == NULL) {
		return;
	}
	generateExpr(ctx, args->expr);
	for (ArgList * next = args->next; next != NULL; next = next->next) {
		output(ctx, ", ");
		generateExpr(ctx, next->expr);
	}
}

static void generateExpr(GeneratorContext * ctx, Expr * expr) {
	if (expr == NULL) {
		output(ctx, "0");
		return;
	}

	switch (expr->type) {
		case EXPR_INTEGER:
			output(ctx, "%d", expr->integer);
			break;
		case EXPR_FLOAT:
			output(ctx, "%g", expr->number);
			break;
		case EXPR_STRING:
			output(ctx, "\"%s\"", expr->string != NULL ? expr->string : "");
			break;
		case EXPR_BOOL:
			output(ctx, "%s", expr->boolean ? "true" : "false");
			break;
		case EXPR_TIME:
			output(ctx, "%d", parseTimeLiteralMs(expr->string));
			break;
		case EXPR_IDENTIFIER:
			output(ctx, "%s", expr->string);
			break;
		case EXPR_MEMBER: {
			char memberBuf[64];
			const char * method = expr->member.member;
			if (strcmp(method, "temperature") == 0) {
				strcpy(memberBuf, "read_temperature");
				method = memberBuf;
			}
			else if (strcmp(method, "humidity") == 0) {
				strcpy(memberBuf, "read_humidity");
				method = memberBuf;
			}
			else if (strcmp(method, "distance") == 0) {
				strcpy(memberBuf, "read_distance");
				method = memberBuf;
			}
			generateMethodCall(ctx, expr->member.object, method, NULL, false);
			break;
		}
		case EXPR_CALL:
			generateMethodCall(ctx, expr->call.object, expr->call.method, expr->call.args, false);
			break;
		case EXPR_BINARY: {
			const char * op = "?";
			switch (expr->binary.op) {
				case OP_ADD: op = "+"; break;
				case OP_SUB: op = "-"; break;
				case OP_MUL: op = "*"; break;
				case OP_DIV: op = "/"; break;
				case OP_EQ:  op = "=="; break;
				case OP_NE:  op = "!="; break;
				case OP_LT:  op = "<"; break;
				case OP_GT:  op = ">"; break;
				case OP_LE:  op = "<="; break;
				case OP_GE:  op = ">="; break;
				case OP_AND: op = "&&"; break;
				case OP_OR:  op = "||"; break;
				default: break;
			}
			if (expr->binary.op == OP_AND || expr->binary.op == OP_OR) {
				output(ctx, "(");
				generateExpr(ctx, expr->binary.left);
				output(ctx, " %s ", op);
				generateExpr(ctx, expr->binary.right);
				output(ctx, ")");
			}
			else {
				output(ctx, "(");
				generateExpr(ctx, expr->binary.left);
				output(ctx, " %s ", op);
				generateExpr(ctx, expr->binary.right);
				output(ctx, ")");
			}
			break;
		}
		case EXPR_UNARY:
			if (expr->unary.op == OP_NOT) {
				output(ctx, "(!");
				generateExpr(ctx, expr->unary.operand);
				output(ctx, ")");
			}
			else {
				output(ctx, "(-");
				generateExpr(ctx, expr->unary.operand);
				output(ctx, ")");
			}
			break;
		default:
			output(ctx, "0");
			break;
	}
}

static void generateMethodCall(
	GeneratorContext * ctx,
	const char * object,
	const char * method,
	ArgList * args,
	bool asStatement
) {
	char * upper = strdupUpper(object);
	char pinMacro[128];

	if (upper == NULL) {
		return;
	}

	snprintf(pinMacro, sizeof(pinMacro), "PIN_%s", upper);

	if (strcmp(method, "turn_on") == 0) {
		output(ctx, "digitalWrite(%s, HIGH)", pinMacro);
	}
	else if (strcmp(method, "turn_off") == 0) {
		output(ctx, "digitalWrite(%s, LOW)", pinMacro);
	}
	else if (strcmp(method, "on") == 0) {
		output(ctx, "analogWrite(%s, ", pinMacro);
		generateArgList(ctx, args);
		output(ctx, ")");
	}
	else if (strcmp(method, "toggle") == 0) {
		output(ctx, "digitalWrite(%s, !digitalRead(%s))", pinMacro, pinMacro);
	}
	else if (strcmp(method, "beep") == 0) {
		output(ctx, "tone(%s, ", pinMacro);
		generateArgList(ctx, args);
		output(ctx, ")");
	}
	else if (strcmp(method, "is_pressed") == 0) {
		output(ctx, "(digitalRead(%s) == HIGH)", pinMacro);
	}
	else if (strcmp(method, "read_value") == 0) {
		output(ctx, "analogRead(%s)", pinMacro);
	}
	else if (strcmp(method, "write") == 0 || strcmp(method, "move") == 0) {
		output(ctx, "%s.write(", object);
		generateArgList(ctx, args);
		output(ctx, ")");
	}
	else if (strcmp(method, "read_distance") == 0) {
		snprintf(pinMacro, sizeof(pinMacro), "PIN_%s_TRIG", upper);
		char echoMacro[128];
		snprintf(echoMacro, sizeof(echoMacro), "PIN_%s_ECHO", upper);
		output(ctx, "readUltrasonicDistance(%s, %s)", pinMacro, echoMacro);
	}
	else if (strcmp(method, "read_temperature") == 0 || strcmp(method, "temperature") == 0) {
		output(ctx, "%s.readTemperature()", object);
	}
	else if (strcmp(method, "read_humidity") == 0 || strcmp(method, "humidity") == 0) {
		output(ctx, "%s.readHumidity()", object);
	}
	else if (strcmp(method, "print") == 0) {
		output(ctx, "%s.print(", object);
		generateArgList(ctx, args);
		output(ctx, ")");
	}
	else if (strcmp(method, "clear") == 0) {
		output(ctx, "%s.clear()", object);
	}
	else if (asStatement) {
		output(ctx, "/* unknown method %s.%s */", object, method);
	}

	free(upper);
}

static const char * varTypeFromExpr(Expr * expr) {
	if (expr == NULL) {
		return "int";
	}
	switch (expr->type) {
		case EXPR_FLOAT:
			return "float";
		case EXPR_STRING:
			return "String";
		case EXPR_BOOL:
			return "bool";
		default:
			return "int";
	}
}

/* STATEMENTS */

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
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
	if (expr == NULL) return "int";
	switch (expr->resolvedType) {
		case TYPE_FLOAT:  return "float";
		case TYPE_STRING: return "String";
		case TYPE_BOOL:   return "bool";
		default:          return "int";
	}
}

/* STATEMENTS */

static void generateStmtList(GeneratorContext * ctx, StmtList * stmts) {
	for (StmtList * node = stmts; node != NULL; node = node->next) {
		if (node->stmt != NULL) {
			generateStmt(ctx, node->stmt);
		}
	}
}

static void generateStmt(GeneratorContext * ctx, Stmt * stmt) {
	if (stmt == NULL) {
		return;
	}

	switch (stmt->type) {
		case STMT_CALL:
			indent(ctx);
			generateMethodCall(ctx, stmt->call.object, stmt->call.method, stmt->call.args, true);
			output(ctx, ";\n");
			break;
		case STMT_VAR:
			indent(ctx);
			output(ctx, "%s %s = ", varTypeFromExpr(stmt->var.value), stmt->var.name);
			generateExpr(ctx, stmt->var.value);
			output(ctx, ";\n");
			break;
		case STMT_ASSIGN:
			indent(ctx);
			output(ctx, "%s = ", stmt->assign.name);
			generateExpr(ctx, stmt->assign.value);
			output(ctx, ";\n");
			break;
		case STMT_BLOCK:
			indent(ctx);
			output(ctx, "{\n");
			ctx->indent++;
			generateStmtList(ctx, stmt->block.body);
			ctx->indent--;
			indent(ctx);
			output(ctx, "}\n");
			break;
		case STMT_IF:
			indent(ctx);
			output(ctx, "if (");
			generateExpr(ctx, stmt->if_.cond);
			output(ctx, ") {\n");
			ctx->indent++;
			generateStmtList(ctx, stmt->if_.thenBranch);
			ctx->indent--;
			indent(ctx);
			output(ctx, "}");
			if (stmt->if_.elseBranch != NULL) {
				output(ctx, " else {\n");
				ctx->indent++;
				generateStmtList(ctx, stmt->if_.elseBranch);
				ctx->indent--;
				indent(ctx);
				output(ctx, "}");
			}
			output(ctx, "\n");
			break;
		case STMT_REPEAT_EVERY: {
			int id = ctx->repeatEveryCounter++;
			int timeoutMs = parseTimeLiteralMs(stmt->repeatEvery.timeLiteral);
			indent(ctx);
			output(ctx, "if (millis() - last_millis_%d >= %d) {\n", id, timeoutMs);
			ctx->indent++;
			indent(ctx);
			output(ctx, "last_millis_%d = millis();\n", id);
			generateStmtList(ctx, stmt->repeatEvery.body);
			ctx->indent--;
			indent(ctx);
			output(ctx, "}\n");
			break;
		}
		case STMT_REPEAT_TIMES:
			indent(ctx);
			output(ctx, "for (int __i = 0; __i < %d; __i++) {\n", stmt->repeatTimes.count);
			ctx->indent++;
			generateStmtList(ctx, stmt->repeatTimes.body);
			ctx->indent--;
			indent(ctx);
			output(ctx, "}\n");
			break;
		case STMT_WAIT:
			indent(ctx);
			output(ctx, "delay(%d);\n", parseTimeLiteralMs(stmt->wait.timeLiteral));
			break;
		case STMT_FOR_RANGE:
			indent(ctx);
			output(ctx, "for (int %s = ", stmt->forRange.varName);
			generateExpr(ctx, stmt->forRange.from);
			output(ctx, "; %s <= ", stmt->forRange.varName);
			generateExpr(ctx, stmt->forRange.to);
			output(ctx, "; %s++) {\n", stmt->forRange.varName);
			ctx->indent++;
			generateStmtList(ctx, stmt->forRange.body);
			ctx->indent--;
			indent(ctx);
			output(ctx, "}\n");
			break;
		default:
			break;
	}
}

/* HARDWARE GENERATION */

static void generatePinDefines(GeneratorContext * ctx, HardwareDecl * decl) {
	char pinBuf[16];
	char * upper = strdupUpper(decl->identifier);
	if (upper == NULL) {
		return;
	}

	switch (decl->componentType) {
		case COMP_ULTRASONIC: {
			int trig = findNamedPin(decl->pinSpec, "trig");
			int echo = findNamedPin(decl->pinSpec, "echo");
			formatPin(trig, pinBuf, sizeof(pinBuf));
			output(ctx, "#define PIN_%s_TRIG %s\n", upper, pinBuf);
			formatPin(echo, pinBuf, sizeof(pinBuf));
			output(ctx, "#define PIN_%s_ECHO %s\n", upper, pinBuf);
			break;
		}
		case COMP_LCD: {
			const char * names[] = { "rs", "en", "d4", "d5", "d6", "d7" };
			for (int i = 0; i < 6; ++i) {
				int pin = findNamedPin(decl->pinSpec, names[i]);
				formatPin(pin, pinBuf, sizeof(pinBuf));
				output(ctx, "#define PIN_%s_%s %s\n", upper, names[i], pinBuf);
			}
			break;
		}
		default: {
			int pin = extractSinglePin(decl->pinSpec);
			formatPin(pin, pinBuf, sizeof(pinBuf));
			output(ctx, "#define PIN_%s %s\n", upper, pinBuf);
			break;
		}
	}

	free(upper);
}

static void generateGlobalInstances(GeneratorContext * ctx, HardwareDecl * decl) {
	switch (decl->componentType) {
		case COMP_DHT11: {
			char pinBuf[16];
			char * upper = strdupUpper(decl->identifier);
			int pin = extractSinglePin(decl->pinSpec);
			formatPin(pin, pinBuf, sizeof(pinBuf));
			output(ctx, "DHT %s(PIN_%s, DHT11);\n", decl->identifier, upper);
			free(upper);
			break;
		}
		case COMP_SERVO:
			output(ctx, "Servo %s;\n", decl->identifier);
			break;
		case COMP_LCD: {
			char * upper = strdupUpper(decl->identifier);
			output(ctx,
				"LiquidCrystal %s(PIN_%s_rs, PIN_%s_en, PIN_%s_d4, PIN_%s_d5, PIN_%s_d6, PIN_%s_d7);\n",
				decl->identifier, upper, upper, upper, upper, upper, upper);
			free(upper);
			break;
		}
		default:
			break;
	}
}

static void generateSetupForDecl(GeneratorContext * ctx, HardwareDecl * decl) {
	char * upper = strdupUpper(decl->identifier);
	if (upper == NULL) {
		return;
	}

	switch (decl->componentType) {
		case COMP_LED:
		case COMP_BUZZER:
			indent(ctx);
			output(ctx, "pinMode(PIN_%s, OUTPUT);\n", upper);
			break;
		case COMP_BUTTON:
			indent(ctx);
			output(ctx, "pinMode(PIN_%s, INPUT);\n", upper);
			break;
		case COMP_SERVO:
			indent(ctx);
			output(ctx, "%s.attach(PIN_%s);\n", decl->identifier, upper);
			break;
		case COMP_ULTRASONIC:
			indent(ctx);
			output(ctx, "pinMode(PIN_%s_TRIG, OUTPUT);\n", upper);
			indent(ctx);
			output(ctx, "pinMode(PIN_%s_ECHO, INPUT);\n", upper);
			break;
		case COMP_DHT11:
			indent(ctx);
			output(ctx, "%s.begin();\n", decl->identifier);
			break;
		case COMP_LCD:
			indent(ctx);
			output(ctx, "%s.begin(16, 2);\n", decl->identifier);
			break;
		default:
			break;
	}

	free(upper);
}

static bool isLiteralExpr(Expr * expr) {
	if (expr == NULL) return true;
	switch (expr->type) {
		case EXPR_INTEGER:
		case EXPR_FLOAT:
		case EXPR_BOOL:
		case EXPR_STRING:
		case EXPR_TIME:
			return true;
		default:
			return false;
	}
}

static void generateRoutineGlobals(GeneratorContext * ctx, StmtList * stmts) {
	for (StmtList * node = stmts; node != NULL; node = node->next) {
		Stmt * stmt = node->stmt;
		if (stmt == NULL || stmt->type != STMT_VAR) continue;
		const char * type = varTypeFromExpr(stmt->var.value);
		if (isLiteralExpr(stmt->var.value)) {
			output(ctx, "%s %s = ", type, stmt->var.name);
			generateExpr(ctx, stmt->var.value);
			output(ctx, ";\n");
		} else {
			switch (stmt->var.value->resolvedType) {
				case TYPE_FLOAT:  output(ctx, "float %s = 0.0;\n", stmt->var.name); break;
				case TYPE_BOOL:   output(ctx, "bool %s = false;\n", stmt->var.name); break;
				case TYPE_STRING: output(ctx, "String %s = \"\";\n", stmt->var.name); break;
				default:          output(ctx, "int %s = 0;\n", stmt->var.name); break;
			}
		}
	}
}

static void generateLoopBody(GeneratorContext * ctx, StmtList * stmts) {
	for (StmtList * node = stmts; node != NULL; node = node->next) {
		Stmt * stmt = node->stmt;
		if (stmt == NULL) {
			continue;
		}
		if (stmt->type == STMT_VAR) {
			Expr * val = stmt->var.value;
			bool isRuntimeInit = (val != NULL && !isLiteralExpr(val));
			if (isRuntimeInit) {
				indent(ctx);
				output(ctx, "%s = ", stmt->var.name);
				generateExpr(ctx, val);
				output(ctx, ";\n");
			}
			continue;
		}
		generateStmt(ctx, stmt);
	}
}

static CompilationStatus generateProgram(GeneratorContext * ctx, Program * program) {
	if (program == NULL) {
		logError(_logger, "Generator: null program");
		return FAILED;
	}

	HardwareBlock * hardware = program->hardware;
	RoutineBlock * routine = program->routine;

	if (hardware != NULL) {
		scanHardwareNeeds(hardware->declarations, ctx);
	}

	if (ctx->needsLiquidCrystal) {
		output(ctx, "#include <LiquidCrystal.h>\n");
	}
	if (ctx->needsDht) {
		output(ctx, "#include <DHT.h>\n");
	}
	if (ctx->needsServo) {
		output(ctx, "#include <Servo.h>\n");
	}
	output(ctx, "\n");

	if (hardware != NULL) {
		for (HardwareDeclList * node = hardware->declarations; node != NULL; node = node->next) {
			if (node->declaration != NULL) {
				generatePinDefines(ctx, node->declaration);
			}
		}
		output(ctx, "\n");
	}

	if (ctx->needsUltrasonic) {
		output(ctx, "float readUltrasonicDistance(int trigPin, int echoPin) {\n");
		output(ctx, "  digitalWrite(trigPin, LOW);\n");
		output(ctx, "  delayMicroseconds(2);\n");
		output(ctx, "  digitalWrite(trigPin, HIGH);\n");
		output(ctx, "  delayMicroseconds(10);\n");
		output(ctx, "  digitalWrite(trigPin, LOW);\n");
		output(ctx, "  long duration = pulseIn(echoPin, HIGH);\n");
		output(ctx, "  return duration * 0.034 / 2;\n");
		output(ctx, "}\n\n");
	}

	if (hardware != NULL) {
		for (HardwareDeclList * node = hardware->declarations; node != NULL; node = node->next) {
			if (node->declaration != NULL) {
				generateGlobalInstances(ctx, node->declaration);
			}
		}
		if (hardware->declarations != NULL) {
			output(ctx, "\n");
		}
	}

	int repeatEveryCount = 0;
	if (routine != NULL) {
		countRepeatEvery(routine->stmts, &repeatEveryCount);
	}
	for (int i = 0; i < repeatEveryCount; ++i) {
		output(ctx, "unsigned long last_millis_%d = 0;\n", i);
	}

	if (routine != NULL) {
		generateRoutineGlobals(ctx, routine->stmts);
		if (routine->stmts != NULL) {
			output(ctx, "\n");
		}
	}

	output(ctx, "void setup() {\n");
	ctx->indent = 1;
	if (hardware != NULL) {
		for (HardwareDeclList * node = hardware->declarations; node != NULL; node = node->next) {
			if (node->declaration != NULL) {
				generateSetupForDecl(ctx, node->declaration);
			}
		}
	}
	ctx->indent = 0;
	output(ctx, "}\n\n");

	output(ctx, "void loop() {\n");
	ctx->indent = 1;
	if (routine != NULL) {
		generateLoopBody(ctx, routine->stmts);
	}
	output(ctx, "}\n");

	return SUCCEEDED;
}

/** PUBLIC FUNCTIONS */

CompilationStatus executeGenerator(CompilerState * compilerState) {
	if (compilerState == NULL) {
		return FAILED;
	}

	Program * program = (Program *)compilerState->abstractSyntaxtTree;
	FILE * outFile = stdout;
	bool closeFile = false;

	if (compilerState->outputPath != NULL) {
		outFile = fopen(compilerState->outputPath, "w");
		if (outFile == NULL) {
			logError(_logger, "Generator: cannot open output file '%s'", compilerState->outputPath);
			return FAILED;
		}
		closeFile = true;
	}

	GeneratorContext ctx = {
		.out = outFile,
		.indent = 0,
		.repeatEveryCounter = 0,
		.needsLiquidCrystal = false,
		.needsDht = false,
		.needsServo = false,
		.needsUltrasonic = false,
	};

	logDebugging(_logger, "Generating Arduino code...");
	CompilationStatus status = generateProgram(&ctx, program);

	if (closeFile) {
		fclose(outFile);
	}

	if (status == SUCCEEDED) {
		logDebugging(_logger, "Code generation completed.");
	}
	else {
		logError(_logger, "Code generation failed.");
	}

	return status;
}
#include "AbstractSyntaxTree.h"
#include <string.h>

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

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

/* PUBLIC FUNCTIONS — destroy only; allocation is done in BisonActions.c */

void destroyExpr(Expr * expr) {
	if (expr == NULL) return;
	switch (expr->type) {
		case EXPR_STRING:
		case EXPR_IDENTIFIER:
		case EXPR_TIME:
			free(expr->string);
			break;
		case EXPR_MEMBER:
			free(expr->member.object);
			free(expr->member.member);
			break;
		case EXPR_CALL:
			free(expr->call.object);
			free(expr->call.method);
			destroyArgList(expr->call.args);
			break;
		case EXPR_BINARY:
			destroyExpr(expr->binary.left);
			destroyExpr(expr->binary.right);
			break;
		case EXPR_UNARY:
			destroyExpr(expr->unary.operand);
			break;
		default:
			break;
	}
	free(expr);
}

void destroyArgList(ArgList * list) {
	while (list != NULL) {
		ArgList * current = list;
		list = list->next;
		destroyExpr(current->expr);
		free(current);
	}
}

void destroyStmt(Stmt * stmt) {
	if (stmt == NULL) return;
	switch (stmt->type) {
		case STMT_CALL:
			free(stmt->call.object);
			free(stmt->call.method);
			destroyArgList(stmt->call.args);
			break;
		case STMT_VAR:
			free(stmt->var.name);
			destroyExpr(stmt->var.value);
			break;
		case STMT_ASSIGN:
			free(stmt->assign.name);
			destroyExpr(stmt->assign.value);
			break;
		case STMT_BLOCK:
			destroyStmtList(stmt->block.body);
			break;
		case STMT_IF:
			destroyExpr(stmt->if_.cond);
			destroyStmtList(stmt->if_.thenBranch);
			destroyStmtList(stmt->if_.elseBranch);
			break;
		case STMT_REPEAT_EVERY:
			free(stmt->repeatEvery.timeLiteral);
			destroyStmtList(stmt->repeatEvery.body);
			break;
		case STMT_REPEAT_TIMES:
			destroyStmtList(stmt->repeatTimes.body);
			break;
		case STMT_WAIT:
			free(stmt->wait.timeLiteral);
			break;
		case STMT_FOR_RANGE:
			free(stmt->forRange.varName);
			destroyExpr(stmt->forRange.from);
			destroyExpr(stmt->forRange.to);
			destroyStmtList(stmt->forRange.body);
			break;
	}
	free(stmt);
}

void destroyStmtList(StmtList * list) {
	while (list != NULL) {
		StmtList * current = list;
		list = list->next;
		destroyStmt(current->stmt);
		free(current);
	}
}

void destroyPinSpecEntry(PinSpecEntry * entry) {
	if (entry == NULL) return;
	free(entry->name);
	free(entry);
}

void destroyPinSpecEntries(PinSpecEntry * entries) {
	while (entries != NULL) {
		PinSpecEntry * current = entries;
		entries = entries->next;
		destroyPinSpecEntry(current);
	}
}

void destroyPinSpec(PinSpec * pinSpec) {
	if (pinSpec == NULL) return;
	switch (pinSpec->type) {
		case PIN_SPEC_SINGLE_IDENTIFIER:
			free(pinSpec->singleIdentifier);
			break;
		case PIN_SPEC_NAMED_LIST:
			destroyPinSpecEntries(pinSpec->namedPins);
			break;
		case PIN_SPEC_INT_LIST:
			destroyPinSpecEntries(pinSpec->intPins);
			break;
		default:
			break;
	}
	free(pinSpec);
}

void destroyHardwareDecl(HardwareDecl * decl) {
	if (decl == NULL) return;
	free(decl->identifier);
	destroyPinSpec(decl->pinSpec);
	free(decl);
}

void destroyHardwareDeclList(HardwareDeclList * list) {
	while (list != NULL) {
		HardwareDeclList * current = list;
		list = list->next;
		destroyHardwareDecl(current->declaration);
		free(current);
	}
}

void destroyHardwareBlock(HardwareBlock * block) {
	if (block == NULL) return;
	destroyHardwareDeclList(block->declarations);
	free(block);
}

void destroyRoutineBlock(RoutineBlock * block) {
	if (block == NULL) return;
	destroyStmtList(block->stmts);
	free(block);
}

void destroyProgram(Program * program) {
	if (program == NULL) return;
	destroyHardwareBlock(program->hardware);
	destroyRoutineBlock(program->routine);
	free(program);
}

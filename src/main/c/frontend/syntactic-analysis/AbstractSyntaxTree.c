#include "AbstractSyntaxTree.h"

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

/* PUBLIC FUNCTIONS */

void destroyHardwareDecl(HardwareDecl * decl) {
	if (decl == NULL) return;
	free(decl->name);
	destroyHardwareDecl(decl->next);
	free(decl);
}

void destroyHardwareBlock(HardwareBlock * block) {
	if (block == NULL) return;
	destroyHardwareDecl(block->decls);
	free(block);
}

void destroyExpr(Expr * expr) {
	if (expr == NULL) return;
	switch (expr->type) {
		case EXPR_STRING:     free(expr->string); break;
		case EXPR_IDENTIFIER: free(expr->identifier); break;
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
		default: break;
	}
	free(expr);
}

void destroyArgList(ArgList * list) {
	if (list == NULL) return;
	destroyExpr(list->expr);
	destroyArgList(list->next);
	free(list);
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
		case STMT_IF:
			destroyExpr(stmt->if_.cond);
			destroyStmtList(stmt->if_.thenBranch);
			destroyStmtList(stmt->if_.elseBranch);
			break;
		case STMT_REPEAT_EVERY:
			destroyStmtList(stmt->repeatEvery.body);
			break;
		case STMT_WAIT:
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
	if (list == NULL) return;
	destroyStmt(list->stmt);
	destroyStmtList(list->next);
	free(list);
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

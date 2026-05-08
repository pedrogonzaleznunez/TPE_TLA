#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonParser.h"
#include <stdbool.h>
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState);

/* Hardware */
ComponentType ComponentTypeSemanticAction(TokenLabel token);
PinSpec * IntegerPinSpecSemanticAction(int pin);
PinSpec * IdentifierPinSpecSemanticAction(char * pinIdentifier);
PinSpec * NamedListPinSpecSemanticAction(PinSpecEntry * entries);
PinSpec * IntListPinSpecSemanticAction(PinSpecEntry * entries);
PinSpecEntry * NewPinSpecEntryListSemanticAction(PinSpecEntry * entry);
PinSpecEntry * AppendPinSpecEntryListSemanticAction(PinSpecEntry * list, PinSpecEntry * entry);
PinSpecEntry * NamedPinEntrySemanticAction(char * name, int value);
PinSpecEntry * IntPinEntrySemanticAction(int value);
HardwareDecl * HardwareDeclSemanticAction(ComponentType ct, char * identifier, PinSpec * pinSpec);
HardwareDeclList * NewHardwareDeclListSemanticAction(HardwareDecl * decl);
HardwareDeclList * AppendHardwareDeclListSemanticAction(HardwareDeclList * list, HardwareDecl * decl);
HardwareBlock * HardwareBlockSemanticAction(HardwareDeclList * decls);

/* Routine */
RoutineBlock * RoutineBlockSemanticAction(StmtList * stmts);

/* Statements */
StmtList * AppendStmtSemanticAction(StmtList * list, Stmt * stmt);
Stmt * CallStmtSemanticAction(char * object, char * method, ArgList * args);
Stmt * AssignStmtSemanticAction(char * name, Expr * value);
Stmt * VarStmtSemanticAction(char * name, Expr * value);
Stmt * BlockStmtSemanticAction(StmtList * body);
Stmt * IfStmtSemanticAction(Expr * cond, StmtList * thenBranch, StmtList * elseBranch);
Stmt * RepeatEveryStmtSemanticAction(char * timeLiteral, StmtList * body);
Stmt * RepeatTimesStmtSemanticAction(int count, StmtList * body);
Stmt * WaitStmtSemanticAction(char * timeLiteral);
Stmt * ForRangeStmtSemanticAction(char * varName, Expr * from, Expr * to, StmtList * body);

/* Arguments */
ArgList * SingleArgSemanticAction(Expr * expr);
ArgList * AppendArgSemanticAction(ArgList * list, Expr * expr);

/* Expressions */
Expr * BinaryExprSemanticAction(Expr * left, Operator op, Expr * right);
Expr * UnaryExprSemanticAction(Operator op, Expr * operand);
Expr * MemberExprSemanticAction(char * object, char * member);
Expr * CallExprSemanticAction(char * object, char * method, ArgList * args);
Expr * IdentifierExprSemanticAction(char * name);
Expr * IntegerExprSemanticAction(int value);
Expr * FloatExprSemanticAction(double value);
Expr * StringExprSemanticAction(char * value);
Expr * BoolExprSemanticAction(bool value);
Expr * TimeExprSemanticAction(char * timeLiteral);

/* Program */
Program * ProgramSemanticAction(HardwareBlock * hardware, RoutineBlock * routine);

// EZduino semantic actions.

EzLiteral * EzIntLiteralSemanticAction(int value);
EzLiteral * EzFloatLiteralSemanticAction(double value);
EzLiteral * EzBoolLiteralSemanticAction(bool value);
EzLiteral * EzStringLiteralSemanticAction(char * value);
EzLiteral * EzTimeLiteralSemanticAction(EzTimeValue value);

#endif

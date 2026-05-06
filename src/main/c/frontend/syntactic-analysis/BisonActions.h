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

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState);

/* Hardware */
Pin DigitalPinSemanticAction(int number);
Pin AnalogPinSemanticAction(int number);
HardwareDecl * SimpleHardwareDeclSemanticAction(HwDeviceType device, char * name, Pin pin);
HardwareDecl * UltrasonicHardwareDeclSemanticAction(char * name, Pin trig, Pin echo);
HardwareDecl * LcdHardwareDeclSemanticAction(char * name);
HardwareDecl * AppendHardwareDeclSemanticAction(HardwareDecl * list, HardwareDecl * decl);
HardwareBlock * HardwareBlockSemanticAction(HardwareDecl * decls);

/* Routine */
RoutineBlock * RoutineBlockSemanticAction(StmtList * stmts);

/* Statements */
StmtList * AppendStmtSemanticAction(StmtList * list, Stmt * stmt);
Stmt * CallStmtSemanticAction(char * object, char * method, ArgList * args);
Stmt * VarStmtSemanticAction(char * name, Expr * value);
Stmt * IfStmtSemanticAction(Expr * cond, StmtList * thenBranch, StmtList * elseBranch);
Stmt * RepeatEveryStmtSemanticAction(StmtList * body, int intervalMs);
Stmt * WaitStmtSemanticAction(int delayMs);
Stmt * ForRangeStmtSemanticAction(char * varName, Expr * from, Expr * to, StmtList * body);

/* Expressions */
ArgList * SingleArgSemanticAction(Expr * expr);
ArgList * AppendArgSemanticAction(ArgList * list, Expr * expr);
Expr * BinaryExprSemanticAction(Expr * left, Operator op, Expr * right);
Expr * UnaryExprSemanticAction(Operator op, Expr * operand);
Expr * IdentifierExprSemanticAction(char * name);
Expr * MemberExprSemanticAction(char * object, char * member);
Expr * CallExprSemanticAction(char * object, char * method, ArgList * args);
Expr * IntegerExprSemanticAction(int value);
Expr * FloatExprSemanticAction(double value);
Expr * StringExprSemanticAction(char * value);
Expr * BoolExprSemanticAction(bool value);
Expr * TimeExprSemanticAction(int timeMs);
Expr * AnalogPinExprSemanticAction(int pin);

/* Program */
Program * ProgramSemanticAction(HardwareBlock * hardware, RoutineBlock * routine);

#endif

%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 */
void yyerror(const YYLTYPE * location, const char * message) {
	if (location != NULL) {
		fprintf(stderr, "Syntax error at line %d: %s\n", location->first_line, message);
	}
	else {
		fprintf(stderr, "Syntax error: %s\n", message);
	}
}

%}

/**
 * %code requires is emitted into both BisonParser.c and BisonParser.h,
 * so that AST types are visible wherever BisonParser.h is included.
 */
%code requires {
	#include "AbstractSyntaxTree.h"
}

// You touch this, and you die.
%define api.pure full
%define api.push-pull push
%define api.value.union.name SemanticValue
%define parse.error detailed
%locations

%union {
	/** Terminals. */
	signed int integer;
	double decimal;
	char * string;
	TokenLabel token;

	/** Non-terminals. */
	Program * program;
	HardwareBlock * hardwareBlock;
	HardwareDeclList * hardwareDeclList;
	HardwareDecl * hardwareDecl;
	ComponentType componentType;
	PinSpec * pinSpec;
	PinSpecEntry * pinSpecEntry;
	RoutineBlock * routineBlock;
	StmtList * stmtList;
	Stmt * stmt;
	Expr * expr;
	ArgList * argList;
}

/**
 * Destructors run on grammar error or on symbols popped during error recovery.
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { destroyHardwareBlock($$); } <hardwareBlock>
%destructor { destroyHardwareDecl($$); } <hardwareDecl>
%destructor { destroyHardwareDeclList($$); } <hardwareDeclList>
%destructor { destroyPinSpec($$); } <pinSpec>
%destructor { destroyPinSpecEntries($$); } <pinSpecEntry>
%destructor { destroyRoutineBlock($$); } <routineBlock>
%destructor { destroyStmt($$); } <stmt>
%destructor { destroyStmtList($$); } <stmtList>
%destructor { destroyExpr($$); } <expr>
%destructor { destroyArgList($$); } <argList>
%destructor { free($$); } <string>

/** Terminals. */
%token <integer> INTEGER
%token <decimal> FLOAT
%token <string>  STRING
%token <string>  TIME_LITERAL
%token <string>  IDENTIFIER

%token <token> HARDWARE ROUTINE VAR IF ELSE FOR FROM TO REPEAT EVERY WAIT TIMES ON
%token <token> AND OR NOT TRUE FALSE BREAK CONTINUE
%token <token> LED BUZZER BUTTON POTENTIOMETER SERVO ULTRASONIC DHT11 LCD

%token <token> EQ NEQ LTE GTE LT GT ASSIGN
%token <token> PLUS MINUS STAR SLASH PERCENT

%token <token> OPEN_BRACE CLOSE_BRACE OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token <token> SEMICOLON DOT COMMA COLON OPEN_BRACKET CLOSE_BRACKET

%token <token> IGNORED UNKNOWN

/** Non-terminals. */
%type <program> program
%type <hardwareBlock> hardware_block
%type <hardwareDeclList> hardware_decl_list
%type <hardwareDecl> hardware_decl
%type <componentType> component_type
%type <pinSpec> pin_spec
%type <pinSpecEntry> named_pin_list named_pin int_list int_pin
%type <routineBlock> routine_block
%type <stmtList> stmts
%type <stmt> stmt
%type <expr> expr
%type <argList> args arg_list
%type <string> time_literal method_name

/**
 * Operator precedence, lowest to highest.
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left OR
%left AND
%left EQ NEQ
%left LT GT LTE GTE
%left PLUS MINUS
%left STAR SLASH
%right NOT UNARY_MINUS

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program
	: hardware_block routine_block					{ $$ = ProgramSemanticAction($1, $2); }
	;

/* ------------------------------------------------------------------ */
/* Hardware block                                                       */
/* ------------------------------------------------------------------ */

hardware_block
	: HARDWARE OPEN_BRACE hardware_decl_list CLOSE_BRACE	{ $$ = HardwareBlockSemanticAction($3); }
	;

hardware_decl_list
	: hardware_decl								{ $$ = NewHardwareDeclListSemanticAction($1); }
	| hardware_decl_list hardware_decl				{ $$ = AppendHardwareDeclListSemanticAction($1, $2); }
	;

hardware_decl
	: component_type IDENTIFIER ON pin_spec SEMICOLON		{ $$ = HardwareDeclSemanticAction($1, $2, $4); }
	;

component_type
	: LED										{ $$ = ComponentTypeSemanticAction($1); }
	| BUZZER									{ $$ = ComponentTypeSemanticAction($1); }
	| BUTTON									{ $$ = ComponentTypeSemanticAction($1); }
	| POTENTIOMETER								{ $$ = ComponentTypeSemanticAction($1); }
	| SERVO										{ $$ = ComponentTypeSemanticAction($1); }
	| ULTRASONIC								{ $$ = ComponentTypeSemanticAction($1); }
	| DHT11										{ $$ = ComponentTypeSemanticAction($1); }
	| LCD										{ $$ = ComponentTypeSemanticAction($1); }
	;

pin_spec
	: INTEGER									{ $$ = IntegerPinSpecSemanticAction($1); }
	| IDENTIFIER								{ $$ = IdentifierPinSpecSemanticAction($1); }
	| OPEN_PARENTHESIS named_pin_list CLOSE_PARENTHESIS	{ $$ = NamedListPinSpecSemanticAction($2); }
	| OPEN_PARENTHESIS int_list CLOSE_PARENTHESIS		{ $$ = IntListPinSpecSemanticAction($2); }
	;

named_pin_list
	: named_pin									{ $$ = NewPinSpecEntryListSemanticAction($1); }
	| named_pin_list COMMA named_pin				{ $$ = AppendPinSpecEntryListSemanticAction($1, $3); }
	;

named_pin
	: IDENTIFIER COLON INTEGER					{ $$ = NamedPinEntrySemanticAction($1, $3); }
	;

int_list
	: int_pin									{ $$ = NewPinSpecEntryListSemanticAction($1); }
	| int_list COMMA int_pin						{ $$ = AppendPinSpecEntryListSemanticAction($1, $3); }
	;

int_pin
	: INTEGER									{ $$ = IntPinEntrySemanticAction($1); }
	;

/* ------------------------------------------------------------------ */
/* Routine block                                                        */
/* ------------------------------------------------------------------ */

routine_block
	: ROUTINE OPEN_BRACE stmts CLOSE_BRACE				{ $$ = RoutineBlockSemanticAction($3); }
	;

stmts
	: stmts stmt								{ $$ = AppendStmtSemanticAction($1, $2); }
	| %empty									{ $$ = NULL; }
	;

stmt
	: IDENTIFIER DOT method_name OPEN_PARENTHESIS args CLOSE_PARENTHESIS SEMICOLON
												{ $$ = CallStmtSemanticAction($1, $3, $5); }
	| VAR IDENTIFIER ASSIGN expr SEMICOLON				{ $$ = VarStmtSemanticAction($2, $4); }
	| IF expr OPEN_BRACE stmts CLOSE_BRACE				{ $$ = IfStmtSemanticAction($2, $4, NULL); }
	| IF expr OPEN_BRACE stmts CLOSE_BRACE ELSE OPEN_BRACE stmts CLOSE_BRACE
												{ $$ = IfStmtSemanticAction($2, $4, $8); }
	| REPEAT EVERY time_literal OPEN_BRACE stmts CLOSE_BRACE
												{ $$ = RepeatEveryStmtSemanticAction($3, $5); }
	| REPEAT INTEGER TIMES OPEN_BRACE stmts CLOSE_BRACE	{ $$ = RepeatTimesStmtSemanticAction($2, $5); }
	| WAIT time_literal SEMICOLON					{ $$ = WaitStmtSemanticAction($2); }
	| FOR IDENTIFIER FROM expr TO expr OPEN_BRACE stmts CLOSE_BRACE
												{ $$ = ForRangeStmtSemanticAction($2, $4, $6, $8); }
	;

/* ------------------------------------------------------------------ */
/* Arguments                                                            */
/* ------------------------------------------------------------------ */

args
	: arg_list									{ $$ = $1; }
	| %empty									{ $$ = NULL; }
	;

arg_list
	: expr										{ $$ = SingleArgSemanticAction($1); }
	| arg_list COMMA expr						{ $$ = AppendArgSemanticAction($1, $3); }
	;

/* ------------------------------------------------------------------ */
/* Expressions                                                          */
/* ------------------------------------------------------------------ */

expr
	: expr PLUS expr							{ $$ = BinaryExprSemanticAction($1, OP_ADD, $3); }
	| expr MINUS expr							{ $$ = BinaryExprSemanticAction($1, OP_SUB, $3); }
	| expr STAR expr							{ $$ = BinaryExprSemanticAction($1, OP_MUL, $3); }
	| expr SLASH expr							{ $$ = BinaryExprSemanticAction($1, OP_DIV, $3); }
	| expr EQ expr								{ $$ = BinaryExprSemanticAction($1, OP_EQ, $3); }
	| expr NEQ expr								{ $$ = BinaryExprSemanticAction($1, OP_NE, $3); }
	| expr LT expr								{ $$ = BinaryExprSemanticAction($1, OP_LT, $3); }
	| expr GT expr								{ $$ = BinaryExprSemanticAction($1, OP_GT, $3); }
	| expr LTE expr								{ $$ = BinaryExprSemanticAction($1, OP_LE, $3); }
	| expr GTE expr								{ $$ = BinaryExprSemanticAction($1, OP_GE, $3); }
	| expr AND expr								{ $$ = BinaryExprSemanticAction($1, OP_AND, $3); }
	| expr OR expr								{ $$ = BinaryExprSemanticAction($1, OP_OR, $3); }
	| NOT expr									{ $$ = UnaryExprSemanticAction(OP_NOT, $2); }
	| MINUS expr %prec UNARY_MINUS				{ $$ = UnaryExprSemanticAction(OP_NEG, $2); }
	| OPEN_PARENTHESIS expr CLOSE_PARENTHESIS	{ $$ = $2; }
	| IDENTIFIER DOT method_name OPEN_PARENTHESIS args CLOSE_PARENTHESIS
												{ $$ = CallExprSemanticAction($1, $3, $5); }
	| IDENTIFIER DOT method_name				{ $$ = MemberExprSemanticAction($1, $3); }
	| IDENTIFIER								{ $$ = IdentifierExprSemanticAction($1); }
	| INTEGER									{ $$ = IntegerExprSemanticAction($1); }
	| FLOAT										{ $$ = FloatExprSemanticAction($1); }
	| STRING									{ $$ = StringExprSemanticAction($1); }
	| TRUE										{ $$ = BoolExprSemanticAction(true); }
	| FALSE										{ $$ = BoolExprSemanticAction(false); }
	| TIME_LITERAL								{ $$ = TimeExprSemanticAction($1); }
	;

/* ------------------------------------------------------------------ */
/* Method name: IDENTIFIER or keywords used as method names            */
/* ------------------------------------------------------------------ */

method_name
	: IDENTIFIER								{ $$ = $1; }
	| ON										{ $$ = strdup("on"); }
	;

/* ------------------------------------------------------------------ */
/* Time literal                                                         */
/* ------------------------------------------------------------------ */

time_literal
	: TIME_LITERAL								{ $$ = $1; }
	;

%%

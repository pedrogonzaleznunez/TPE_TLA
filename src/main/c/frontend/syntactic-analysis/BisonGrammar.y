%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 */
void yyerror(const YYLTYPE * location, const char * message) {}

%}

/**
 * %code requires is emitted into both BisonParser.c and BisonParser.h,
 * so that Pin and HwDeviceType are visible wherever BisonParser.h is included.
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
	double number;
	int timeMs;
	int analogPin;
	char * string;
	TokenLabel token;

	/** Non-terminals. */
	Program * program;
	HardwareBlock * hardwareBlock;
	RoutineBlock * routineBlock;
	HardwareDecl * hardwareDecl;
	StmtList * stmtList;
	Stmt * stmt;
	Expr * expr;
	ArgList * argList;
	Pin pin;
	HwDeviceType hwDeviceType;
}

/**
 * Destructors run on grammar error or on symbols popped during error recovery.
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { destroyHardwareBlock($$); } <hardwareBlock>
%destructor { destroyHardwareDecl($$); } <hardwareDecl>
%destructor { destroyRoutineBlock($$); } <routineBlock>
%destructor { destroyStmt($$); } <stmt>
%destructor { destroyStmtList($$); } <stmtList>
%destructor { destroyExpr($$); } <expr>
%destructor { destroyArgList($$); } <argList>
%destructor { free($$); } <string>

/** Terminals. */
%token <token> HARDWARE ROUTINE VAR IF ELSE REPEAT EVERY WAIT FOR FROM TO TIMES ON
%token <token> KW_LED KW_BUZZER KW_BUTTON KW_POTENTIOMETER KW_SERVO KW_ULTRASONIC KW_DHT11 KW_LCD
%token <token> KW_TRIG KW_ECHO
%token <token> TRUE FALSE
%token <token> AND OR NOT
%token <token> PLUS MINUS STAR SLASH ASSIGN
%token <token> EQ NE LT GT LE GE
%token <token> LBRACE RBRACE LPAREN RPAREN SEMI COMMA COLON DOT
%token <integer> INTEGER
%token <number> FLOAT
%token <timeMs> TIME
%token <analogPin> ANALOG_PIN
%token <string> IDENTIFIER STRING
%token <token> IGNORED UNKNOWN

/** Non-terminals. */
%type <program> program
%type <hardwareBlock> hardware_block
%type <routineBlock> routine_block
%type <hardwareDecl> hardware_decls hardware_decl
%type <stmtList> stmts
%type <stmt> stmt
%type <expr> expr
%type <argList> args arg_list
%type <pin> pin
%type <hwDeviceType> hw_type
%type <string> method_name

/**
 * Operator precedence, lowest to highest.
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left STAR SLASH
%right NOT UNARY_MINUS

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program
	: hardware_block routine_block						{ $$ = ProgramSemanticAction($1, $2); }
	;

/* ------------------------------------------------------------------ */
/* Hardware block                                                       */
/* ------------------------------------------------------------------ */

hardware_block
	: HARDWARE LBRACE hardware_decls RBRACE				{ $$ = HardwareBlockSemanticAction($3); }
	;

hardware_decls
	: hardware_decls hardware_decl						{ $$ = AppendHardwareDeclSemanticAction($1, $2); }
	| %empty											{ $$ = NULL; }
	;

hardware_decl
	: hw_type IDENTIFIER ON pin SEMI					{ $$ = SimpleHardwareDeclSemanticAction($1, $2, $4); }
	| KW_ULTRASONIC IDENTIFIER KW_TRIG pin KW_ECHO pin SEMI
														{ $$ = UltrasonicHardwareDeclSemanticAction($2, $4, $6); }
	| KW_LCD IDENTIFIER SEMI							{ $$ = LcdHardwareDeclSemanticAction($2); }
	;

hw_type
	: KW_LED											{ $$ = HW_LED; }
	| KW_BUZZER											{ $$ = HW_BUZZER; }
	| KW_BUTTON											{ $$ = HW_BUTTON; }
	| KW_POTENTIOMETER									{ $$ = HW_POTENTIOMETER; }
	| KW_SERVO											{ $$ = HW_SERVO; }
	| KW_DHT11											{ $$ = HW_DHT11; }
	;

pin
	: INTEGER											{ $$ = DigitalPinSemanticAction($1); }
	| ANALOG_PIN										{ $$ = AnalogPinSemanticAction($1); }
	;

/* ------------------------------------------------------------------ */
/* Routine block                                                        */
/* ------------------------------------------------------------------ */

routine_block
	: ROUTINE LBRACE stmts RBRACE						{ $$ = RoutineBlockSemanticAction($3); }
	;

stmts
	: stmts stmt										{ $$ = AppendStmtSemanticAction($1, $2); }
	| %empty											{ $$ = NULL; }
	;

stmt
	: IDENTIFIER DOT method_name LPAREN args RPAREN SEMI
														{ $$ = CallStmtSemanticAction($1, $3, $5); }
	| VAR IDENTIFIER ASSIGN expr SEMI					{ $$ = VarStmtSemanticAction($2, $4); }
	| IF LPAREN expr RPAREN LBRACE stmts RBRACE
														{ $$ = IfStmtSemanticAction($3, $6, NULL); }
	| IF LPAREN expr RPAREN LBRACE stmts RBRACE ELSE LBRACE stmts RBRACE
														{ $$ = IfStmtSemanticAction($3, $6, $10); }
	| REPEAT LBRACE stmts RBRACE EVERY TIME SEMI		{ $$ = RepeatEveryStmtSemanticAction($3, $6); }
	| WAIT TIME SEMI									{ $$ = WaitStmtSemanticAction($2); }
	| FOR IDENTIFIER FROM expr TO expr LBRACE stmts RBRACE
														{ $$ = ForRangeStmtSemanticAction($2, $4, $6, $8); }
	;

/* ------------------------------------------------------------------ */
/* Arguments                                                            */
/* ------------------------------------------------------------------ */

args
	: arg_list											{ $$ = $1; }
	| %empty											{ $$ = NULL; }
	;

arg_list
	: expr												{ $$ = SingleArgSemanticAction($1); }
	| arg_list COMMA expr								{ $$ = AppendArgSemanticAction($1, $3); }
	;

/* ------------------------------------------------------------------ */
/* Expressions                                                          */
/* ------------------------------------------------------------------ */

expr
	: expr PLUS expr									{ $$ = BinaryExprSemanticAction($1, OP_ADD, $3); }
	| expr MINUS expr									{ $$ = BinaryExprSemanticAction($1, OP_SUB, $3); }
	| expr STAR expr									{ $$ = BinaryExprSemanticAction($1, OP_MUL, $3); }
	| expr SLASH expr									{ $$ = BinaryExprSemanticAction($1, OP_DIV, $3); }
	| expr EQ expr										{ $$ = BinaryExprSemanticAction($1, OP_EQ, $3); }
	| expr NE expr										{ $$ = BinaryExprSemanticAction($1, OP_NE, $3); }
	| expr LT expr										{ $$ = BinaryExprSemanticAction($1, OP_LT, $3); }
	| expr GT expr										{ $$ = BinaryExprSemanticAction($1, OP_GT, $3); }
	| expr LE expr										{ $$ = BinaryExprSemanticAction($1, OP_LE, $3); }
	| expr GE expr										{ $$ = BinaryExprSemanticAction($1, OP_GE, $3); }
	| expr AND expr										{ $$ = BinaryExprSemanticAction($1, OP_AND, $3); }
	| expr OR expr										{ $$ = BinaryExprSemanticAction($1, OP_OR, $3); }
	| NOT expr											{ $$ = UnaryExprSemanticAction(OP_NOT, $2); }
	| MINUS expr %prec UNARY_MINUS						{ $$ = UnaryExprSemanticAction(OP_NEG, $2); }
	| LPAREN expr RPAREN								{ $$ = $2; }
	| IDENTIFIER DOT method_name LPAREN args RPAREN		{ $$ = CallExprSemanticAction($1, $3, $5); }
	| IDENTIFIER DOT method_name						{ $$ = MemberExprSemanticAction($1, $3); }
	| IDENTIFIER										{ $$ = IdentifierExprSemanticAction($1); }
	| INTEGER											{ $$ = IntegerExprSemanticAction($1); }
	| FLOAT												{ $$ = FloatExprSemanticAction($1); }
	| STRING											{ $$ = StringExprSemanticAction($1); }
	| TRUE												{ $$ = BoolExprSemanticAction(true); }
	| FALSE												{ $$ = BoolExprSemanticAction(false); }
	| TIME												{ $$ = TimeExprSemanticAction($1); }
	| ANALOG_PIN										{ $$ = AnalogPinExprSemanticAction($1); }
	;

/* ------------------------------------------------------------------ */
/* Method name: IDENTIFIER or keywords used as method names            */
/* ------------------------------------------------------------------ */

method_name
	: IDENTIFIER										{ $$ = $1; }
	| ON												{ $$ = strdup("on"); }
	;

%%

%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * The error reporting function for Bison parser.
 *
 * @todo Add location to the grammar and "pushToken" API function.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
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

// You touch this, and you die.
%define api.pure full
%define api.push-pull push
%define api.value.union.name SemanticValue
%define parse.error detailed
%locations

%union {
	/** Terminals. */

	int integer;
	double floatValue;
	bool boolean;
	char * string;
	EzTimeValue timeValue;

	/** Non-terminals. */

	EzProgram * program;
	EzHardwareBlock * hardwareBlock;
	EzHardwareDecl * hardwareDecl;
	EzHardwareDeclList * hardwareDeclList;
	EzRoutineBlock * routineBlock;
	EzStatement * statement;
	EzStatementList * statementList;
	EzBlock * block;
	EzExpression * expression;
	EzLiteral * literal;
	EzArgumentList * argumentList;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parsing succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { free($$); } <string>
%destructor { destroyEzHardwareBlock($$); } <hardwareBlock>
%destructor { destroyEzHardwareDecl($$); } <hardwareDecl>
%destructor { destroyEzHardwareDeclList($$); } <hardwareDeclList>
%destructor { destroyEzRoutineBlock($$); } <routineBlock>
%destructor { destroyEzStatement($$); } <statement>
%destructor { destroyEzStatementList($$); } <statementList>
%destructor { destroyEzBlock($$); } <block>
%destructor { destroyEzExpression($$); } <expression>
%destructor { destroyEzLiteral($$); } <literal>
%destructor { destroyEzArgumentList($$); } <argumentList>

/** Terminals. */
%token <integer> INTEGER
%token <floatValue> FLOAT
%token <boolean> BOOLEAN
%token <string> STRING
%token <string> IDENTIFIER
%token <timeValue> TIME

%token HARDWARE ROUTINE VAR IF ELSE REPEAT EVERY WAIT TIMES FOR FROM TO ON
%token TRIG ECHO_KW
%token LED BUZZER BUTTON POTENTIOMETER SERVO ULTRASONIC DHT11 LCD

%token ADD SUB MUL DIV
%token LT GT LE GE EQ NEQ
%token AND OR NOT
%token ASSIGN

%token OPEN_BRACE CLOSE_BRACE OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token SEMICOLON COMMA DOT COLON

/** Non-terminals. */
%type <program> program
%type <hardwareBlock> hardware_block
%type <hardwareDecl> hardware_decl
%type <hardwareDeclList> hardware_decl_list hardware_decl_list_opt
%type <routineBlock> routine_block
%type <statement> statement
%type <statementList> statement_list statement_list_opt
%type <block> block
%type <expression> expression logical_or logical_and equality relational additive multiplicative unary primary
%type <string> method_name
%type <literal> literal
%type <argumentList> argument_list argument_list_opt

/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left OR
%left AND
%right NOT
%nonassoc EQ NEQ LT LE GT GE
%left ADD SUB
%left MUL DIV
%right UMINUS

%%

program: hardware_block routine_block					{ $$ = EzProgramSemanticAction($1, $2); }
	;

hardware_block: HARDWARE OPEN_BRACE hardware_decl_list_opt CLOSE_BRACE
														{ $$ = EzHardwareBlockSemanticAction($3); }
	;

hardware_decl_list_opt: %empty							{ $$ = NULL; }
	| hardware_decl_list								{ $$ = $1; }
	;

hardware_decl_list: hardware_decl_list hardware_decl	{ $$ = EzHardwareDeclListSemanticAction($1, $2); }
	| hardware_decl										{ $$ = EzHardwareDeclListSemanticAction(NULL, $1); }
	;

hardware_decl: LED IDENTIFIER ON INTEGER SEMICOLON
														{ $$ = EzHardwareSimpleDeclSemanticAction(EZ_HW_LED, $2, $4); }
	| BUZZER IDENTIFIER ON INTEGER SEMICOLON
														{ $$ = EzHardwareSimpleDeclSemanticAction(EZ_HW_BUZZER, $2, $4); }
	| BUTTON IDENTIFIER ON INTEGER SEMICOLON
														{ $$ = EzHardwareSimpleDeclSemanticAction(EZ_HW_BUTTON, $2, $4); }
	| POTENTIOMETER IDENTIFIER ON INTEGER SEMICOLON
														{ $$ = EzHardwareSimpleDeclSemanticAction(EZ_HW_POTENTIOMETER, $2, $4); }
	| SERVO IDENTIFIER ON INTEGER SEMICOLON
														{ $$ = EzHardwareSimpleDeclSemanticAction(EZ_HW_SERVO, $2, $4); }
	| DHT11 IDENTIFIER ON INTEGER SEMICOLON
														{ $$ = EzHardwareSimpleDeclSemanticAction(EZ_HW_DHT11, $2, $4); }
	| LCD IDENTIFIER ON INTEGER SEMICOLON
														{ $$ = EzHardwareSimpleDeclSemanticAction(EZ_HW_LCD, $2, $4); }
	| ULTRASONIC IDENTIFIER ON OPEN_PARENTHESIS TRIG COLON INTEGER COMMA ECHO_KW COLON INTEGER CLOSE_PARENTHESIS SEMICOLON
														{ $$ = EzHardwareUltrasonicDeclSemanticAction($2, $7, $11); }
	;

routine_block: ROUTINE OPEN_BRACE statement_list_opt CLOSE_BRACE
														{ $$ = EzRoutineBlockSemanticAction($3); }
	;

statement_list_opt: %empty								{ $$ = NULL; }
	| statement_list									{ $$ = $1; }
	;

statement_list: statement_list statement				{ $$ = EzStatementListSemanticAction($1, $2); }
	| statement											{ $$ = EzStatementListSemanticAction(NULL, $1); }
	;

statement: VAR IDENTIFIER ASSIGN expression SEMICOLON
														{ $$ = EzVarDeclSemanticAction($2, $4); }
	| IDENTIFIER ASSIGN expression SEMICOLON
														{ $$ = EzAssignmentSemanticAction($1, $3); }
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS block
														{ $$ = EzIfSemanticAction($3, $5, NULL); }
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS block ELSE block
														{ $$ = EzIfSemanticAction($3, $5, $7); }
	| REPEAT EVERY TIME block
														{ $$ = EzRepeatEverySemanticAction($3, $4); }
	| REPEAT expression TIMES block
														{ $$ = EzRepeatTimesSemanticAction($2, $4); }
	| WAIT TIME SEMICOLON
														{ $$ = EzWaitSemanticAction($2); }
	| FOR IDENTIFIER FROM expression TO expression block
														{ $$ = EzForRangeSemanticAction($2, $4, $6, $7); }
	| IDENTIFIER DOT method_name OPEN_PARENTHESIS argument_list_opt CLOSE_PARENTHESIS SEMICOLON
														{ $$ = EzMethodCallSemanticAction($1, $3, $5); }
	| block
														{ $$ = EzBlockStatementSemanticAction($1); }
	;

block: OPEN_BRACE statement_list_opt CLOSE_BRACE
														{ $$ = EzBlockSemanticAction($2); }
	;

argument_list_opt: %empty								{ $$ = NULL; }
	| argument_list										{ $$ = $1; }
	;

argument_list: argument_list COMMA expression			{ $$ = EzArgumentListSemanticAction($1, $3); }
	| expression										{ $$ = EzArgumentListSemanticAction(NULL, $1); }
	;

expression: logical_or									{ $$ = $1; }
	;

logical_or: logical_or OR logical_and					{ $$ = EzBinaryExprSemanticAction(EZ_BIN_OR, $1, $3); }
	| logical_and										{ $$ = $1; }
	;

logical_and: logical_and AND equality					{ $$ = EzBinaryExprSemanticAction(EZ_BIN_AND, $1, $3); }
	| equality											{ $$ = $1; }
	;

equality: equality EQ relational						{ $$ = EzBinaryExprSemanticAction(EZ_BIN_EQ, $1, $3); }
	| equality NEQ relational							{ $$ = EzBinaryExprSemanticAction(EZ_BIN_NEQ, $1, $3); }
	| relational										{ $$ = $1; }
	;

relational: relational LT additive						{ $$ = EzBinaryExprSemanticAction(EZ_BIN_LT, $1, $3); }
	| relational GT additive							{ $$ = EzBinaryExprSemanticAction(EZ_BIN_GT, $1, $3); }
	| relational LE additive							{ $$ = EzBinaryExprSemanticAction(EZ_BIN_LE, $1, $3); }
	| relational GE additive							{ $$ = EzBinaryExprSemanticAction(EZ_BIN_GE, $1, $3); }
	| additive											{ $$ = $1; }
	;

additive: additive ADD multiplicative					{ $$ = EzBinaryExprSemanticAction(EZ_BIN_ADD, $1, $3); }
	| additive SUB multiplicative						{ $$ = EzBinaryExprSemanticAction(EZ_BIN_SUB, $1, $3); }
	| multiplicative									{ $$ = $1; }
	;

multiplicative: multiplicative MUL unary				{ $$ = EzBinaryExprSemanticAction(EZ_BIN_MUL, $1, $3); }
	| multiplicative DIV unary							{ $$ = EzBinaryExprSemanticAction(EZ_BIN_DIV, $1, $3); }
	| unary												{ $$ = $1; }
	;

unary: NOT unary										{ $$ = EzUnaryExprSemanticAction(EZ_UNARY_NOT, $2); }
	| SUB unary %prec UMINUS							{ $$ = EzUnaryExprSemanticAction(EZ_UNARY_NEGATE, $2); }
	| primary											{ $$ = $1; }
	;

method_name: IDENTIFIER									{ $$ = $1; }
	| ON												{ $$ = strdup("on"); }
	;

primary: literal										{ $$ = EzLiteralExprSemanticAction($1); }
	| IDENTIFIER										{ $$ = EzIdentifierExprSemanticAction($1); }
	| IDENTIFIER DOT method_name OPEN_PARENTHESIS argument_list_opt CLOSE_PARENTHESIS
														{ $$ = EzMethodCallExprSemanticAction($1, $3, $5); }
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS		{ $$ = $2; }
	;

literal: INTEGER										{ $$ = EzIntLiteralSemanticAction($1); }
	| FLOAT												{ $$ = EzFloatLiteralSemanticAction($1); }
	| BOOLEAN											{ $$ = EzBoolLiteralSemanticAction($1); }
	| STRING											{ $$ = EzStringLiteralSemanticAction($1); }
	| TIME												{ $$ = EzTimeLiteralSemanticAction($1); }
	;

%%
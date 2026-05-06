%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

#include <stdlib.h>

/**
 * The error reporting function for Bison parser.
 *
 * @todo Add location to the grammar and "pushToken" API function.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
 */
void yyerror(const YYLTYPE * location, const char * message) {}

%}

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

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parsing succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { destroyConstant($$); } <constant>
%destructor { destroyExpression($$); } <expression>
%destructor { destroyFactor($$); } <factor>
%destructor { free($$); } <string>

/** Terminals. */
%token <token> HARDWARE ROUTINE VAR IF ELSE REPEAT EVERY WAIT FOR FROM TO TIMES ON

%token <token> KW_LED KW_BUZZER KW_BUTTON KW_POTENTIOMETER KW_SERVO KW_ULTRASONIC KW_DHT11 KW_LCD
%token <token> KW_TRIG KW_ECHO

%token <token> TRUE FALSE
%token <token> AND OR NOT

%token <token> PLUS MINUS STAR SLASH
%token <token> ASSIGN
%token <token> EQ NE LT GT LE GE

%token <token> LBRACE RBRACE LPAREN RPAREN SEMI COMMA COLON DOT

%token <integer> INTEGER
%token <number> FLOAT
%token <timeMs> TIME
%token <analogPin> ANALOG_PIN
%token <string> IDENTIFIER
%token <string> STRING

%token <token> IGNORED
%token <token> UNKNOWN

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left PLUS MINUS
%left STAR SLASH

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: expression											{ $$ = ExpressionProgramSemanticAction($1); }
	;

expression: expression[left] PLUS expression[right]		{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] SLASH expression[right]			{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] STAR expression[right]			{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] MINUS expression[right]			{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor												{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: LPAREN expression RPAREN							{ $$ = ExpressionFactorSemanticAction($2); }
	| constant												{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER											{ $$ = IntegerConstantSemanticAction($1); }
	;

%%

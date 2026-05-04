%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include <stdbool.h>
}

%{

/**
 * @see https://westes.github.io/flex/manual/Definitions-Section.html#Definitions-Section
 */

extern CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label);
extern CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context);
extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus IntegerLexemeAction();
extern CompilationStatus FloatLexemeAction();
extern CompilationStatus IdentifierLexemeAction();
extern CompilationStatus StringLexemeAction();
extern CompilationStatus BooleanLexemeAction(const bool value);
extern CompilationStatus TimeLexemeAction();
extern CompilationStatus LeaveImportExpressionLexemeAction();
extern CompilationStatus LeaveMultilineCommentLexemeAction();
extern CompilationStatus ParenthesisLexemeAction(TokenLabel label);
extern CompilationStatus SubexpressionLexemeAction();
extern CompilationStatus SimpleTokenLexemeAction(TokenLabel label);
extern CompilationStatus UnknownLexemeAction();

%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x IMPORT_EXPRESSION
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[[:space:]]+
digit				[0-9]
identifier			[a-zA-Z_][a-zA-Z0-9_]*
integer				{digit}+
float				{digit}+\.{digit}+
time				{digit}+(ms|s)
string				\"([^\\\"]|\\.)*\"

%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

"/*"							{ return EnterMultilineCommentLexemeAction(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/"			{ return LeaveMultilineCommentLexemeAction(); }
<MULTILINE_COMMENT>[^*]+		{ return IgnoredLexemeAction(); }
<MULTILINE_COMMENT>.			{ return IgnoredLexemeAction(); }

"hardware"						{ return SimpleTokenLexemeAction(HARDWARE); }
"routine"						{ return SimpleTokenLexemeAction(ROUTINE); }
"var"							{ return SimpleTokenLexemeAction(VAR); }
"if"							{ return SimpleTokenLexemeAction(IF); }
"else"					    	{ return SimpleTokenLexemeAction(ELSE); }
"repeat"						{ return SimpleTokenLexemeAction(REPEAT); }
"every"						    { return SimpleTokenLexemeAction(EVERY); }
"wait"						    { return SimpleTokenLexemeAction(WAIT); }
"times"						    { return SimpleTokenLexemeAction(TIMES); }
"for"						    { return SimpleTokenLexemeAction(FOR); }
"from"					    	{ return SimpleTokenLexemeAction(FROM); }
"to"							{ return SimpleTokenLexemeAction(TO); }
"on"							{ return SimpleTokenLexemeAction(ON); }
"trig"					    	{ return SimpleTokenLexemeAction(TRIG); }
"echo"					       	{ return SimpleTokenLexemeAction(ECHO_KW); }

"led"							{ return SimpleTokenLexemeAction(LED); }
"buzzer"						{ return SimpleTokenLexemeAction(BUZZER); }
"button"						{ return SimpleTokenLexemeAction(BUTTON); }
"potentiometer"			    	{ return SimpleTokenLexemeAction(POTENTIOMETER); }
"servo"					    	{ return SimpleTokenLexemeAction(SERVO); }
"ultrasonic"					{ return SimpleTokenLexemeAction(ULTRASONIC); }
"dht11"					    	{ return SimpleTokenLexemeAction(DHT11); }
"lcd"							{ return SimpleTokenLexemeAction(LCD); }

"and"							{ return SimpleTokenLexemeAction(AND); }
"or"							{ return SimpleTokenLexemeAction(OR); }
"not"					    	{ return SimpleTokenLexemeAction(NOT); }

"true"					    	{ return BooleanLexemeAction(true); }
"false"						    { return BooleanLexemeAction(false); }

"=="						    { return SimpleTokenLexemeAction(EQ); }
"!="					    	{ return SimpleTokenLexemeAction(NEQ); }
"<="						    { return SimpleTokenLexemeAction(LE); }
">="						    { return SimpleTokenLexemeAction(GE); }
"<"							    { return SimpleTokenLexemeAction(LT); }
">"							    { return SimpleTokenLexemeAction(GT); }
"="							    { return SimpleTokenLexemeAction(ASSIGN); }

"+"							    { return ArithmeticOperatorLexemeAction(ADD); }
"-"							    { return ArithmeticOperatorLexemeAction(SUB); }
"*"							    { return ArithmeticOperatorLexemeAction(MUL); }
"/"							    { return ArithmeticOperatorLexemeAction(DIV); }

"("							    { return ParenthesisLexemeAction(OPEN_PARENTHESIS); }
")"							    { return ParenthesisLexemeAction(CLOSE_PARENTHESIS); }
"{" 						    { return SimpleTokenLexemeAction(OPEN_BRACE); }
"}"							    { return SimpleTokenLexemeAction(CLOSE_BRACE); }
";"							    { return SimpleTokenLexemeAction(SEMICOLON); }
","							    { return SimpleTokenLexemeAction(COMMA); }
"."							    { return SimpleTokenLexemeAction(DOT); }
":"							    { return SimpleTokenLexemeAction(COLON); }

{time}						    { return TimeLexemeAction(); }
{float}						    { return FloatLexemeAction(); }
{integer}					    { return IntegerLexemeAction(); }
{string}					    { return StringLexemeAction(); }
{identifier}				    { return IdentifierLexemeAction(); }

{whitespace}				    { return IgnoredLexemeAction(); }
.							    { return UnknownLexemeAction(); }

<<EOF>>				        	{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"

}

%{

/**
 * @see https://westes.github.io/flex/manual/Definitions-Section.html#Definitions-Section
 */

extern CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label);
extern CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context);
extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus IdentifierLexemeAction();
extern CompilationStatus IntegerLexemeAction();
extern CompilationStatus LeaveMultilineCommentLexemeAction();
extern CompilationStatus FloatLexemeAction();
extern CompilationStatus StringLexemeAction();
extern CompilationStatus TimeLexemeAction();
extern CompilationStatus AnalogPinLexemeAction();
extern CompilationStatus TokenLexemeAction(TokenLabel label);
extern CompilationStatus UnknownLexemeAction();

%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[[:space:]]+
identifier			[_a-zA-Z][_a-zA-Z0-9]*
integer				[[:digit:]]+
float				[[:digit:]]+\.[[:digit:]]+
analog_pin			A[0-5]
time_ms				[[:digit:]]+ms
time_s				[[:digit:]]+s
string				\"([^\\\n\"]|\\.)*\"

%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

"//"[^\n]*\n						{ return IgnoredLexemeAction(); }

"{"									{ return TokenLexemeAction(LBRACE); }
"}"									{ return TokenLexemeAction(RBRACE); }

"/*"								{ return EnterMultilineCommentLexemeAction(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/"				{ return LeaveMultilineCommentLexemeAction(); }
<MULTILINE_COMMENT>.				{ /* ignore */ }
<MULTILINE_COMMENT>\n				{ /* ignore */ }

"hardware"							{ return TokenLexemeAction(HARDWARE); }
"routine"							{ return TokenLexemeAction(ROUTINE); }
"var"								{ return TokenLexemeAction(VAR); }
"if"								{ return TokenLexemeAction(IF); }
"else"								{ return TokenLexemeAction(ELSE); }
"repeat"							{ return TokenLexemeAction(REPEAT); }
"every"								{ return TokenLexemeAction(EVERY); }
"wait"								{ return TokenLexemeAction(WAIT); }
"for"								{ return TokenLexemeAction(FOR); }
"from"								{ return TokenLexemeAction(FROM); }
"to"								{ return TokenLexemeAction(TO); }
"times"								{ return TokenLexemeAction(TIMES); }
"on"								{ return TokenLexemeAction(ON); }

"led"								{ return TokenLexemeAction(KW_LED); }
"buzzer"							{ return TokenLexemeAction(KW_BUZZER); }
"button"							{ return TokenLexemeAction(KW_BUTTON); }
"potentiometer"						{ return TokenLexemeAction(KW_POTENTIOMETER); }
"servo"								{ return TokenLexemeAction(KW_SERVO); }
"ultrasonic"						{ return TokenLexemeAction(KW_ULTRASONIC); }
"dht11"								{ return TokenLexemeAction(KW_DHT11); }
"lcd"								{ return TokenLexemeAction(KW_LCD); }

"trig"								{ return TokenLexemeAction(KW_TRIG); }
"echo"								{ return TokenLexemeAction(KW_ECHO); }

"true"								{ return TokenLexemeAction(TRUE); }
"false"								{ return TokenLexemeAction(FALSE); }

"and"								{ return TokenLexemeAction(AND); }
"or"								{ return TokenLexemeAction(OR); }
"not"								{ return TokenLexemeAction(NOT); }

"=="								{ return TokenLexemeAction(EQ); }
"!="								{ return TokenLexemeAction(NE); }
"<="								{ return TokenLexemeAction(LE); }
">="								{ return TokenLexemeAction(GE); }
"<"									{ return TokenLexemeAction(LT); }
">"									{ return TokenLexemeAction(GT); }
"="									{ return TokenLexemeAction(ASSIGN); }

"+"									{ return TokenLexemeAction(PLUS); }
"-"									{ return TokenLexemeAction(MINUS); }
"*"									{ return TokenLexemeAction(STAR); }
"/"									{ return TokenLexemeAction(SLASH); }

"("									{ return TokenLexemeAction(LPAREN); }
")"									{ return TokenLexemeAction(RPAREN); }
";"									{ return TokenLexemeAction(SEMI); }
","									{ return TokenLexemeAction(COMMA); }
":"									{ return TokenLexemeAction(COLON); }
"."									{ return TokenLexemeAction(DOT); }

{float}									{ return FloatLexemeAction(); }
{time_ms}								{ return TimeLexemeAction(); }
{time_s}									{ return TimeLexemeAction(); }
{analog_pin}							{ return AnalogPinLexemeAction(); }
{string}									{ return StringLexemeAction(); }
{integer}								{ return IntegerLexemeAction(); }
{identifier}							{ return IdentifierLexemeAction(); }

{whitespace}						{ return IgnoredLexemeAction(); }
.									{ return UnknownLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"

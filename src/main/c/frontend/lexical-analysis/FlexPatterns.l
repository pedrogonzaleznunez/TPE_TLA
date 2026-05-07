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

extern CompilationStatus EOFLexemeAction();
extern CompilationStatus FloatLexemeAction();
extern CompilationStatus IdentifierLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus IntegerLexemeAction();
extern CompilationStatus KeywordLexemeAction(TokenLabel label);
extern CompilationStatus OperatorLexemeAction(TokenLabel label);
extern CompilationStatus StringLexemeAction();
extern CompilationStatus SymbolLexemeAction(TokenLabel label);
extern CompilationStatus TimeLiteralLexemeAction();
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
comment				"//"[^\n]*

%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

{comment}							{ return IgnoredLexemeAction(); }
"/*"								{ BEGIN(MULTILINE_COMMENT); return IgnoredLexemeAction(); }
<MULTILINE_COMMENT>"*/"				{ BEGIN(INITIAL); return IgnoredLexemeAction(); }
<MULTILINE_COMMENT>\n				{ return IgnoredLexemeAction(); }
<MULTILINE_COMMENT>.				{ return IgnoredLexemeAction(); }

"hardware"							{ return KeywordLexemeAction(HARDWARE); }
"routine"							{ return KeywordLexemeAction(ROUTINE); }
"var"								{ return KeywordLexemeAction(VAR); }
"if"								{ return KeywordLexemeAction(IF); }
"else"								{ return KeywordLexemeAction(ELSE); }
"for"								{ return KeywordLexemeAction(FOR); }
"from"								{ return KeywordLexemeAction(FROM); }
"to"								{ return KeywordLexemeAction(TO); }
"repeat"							{ return KeywordLexemeAction(REPEAT); }
"every"								{ return KeywordLexemeAction(EVERY); }
"wait"								{ return KeywordLexemeAction(WAIT); }
"times"								{ return KeywordLexemeAction(TIMES); }
"on"								{ return KeywordLexemeAction(ON); }
"and"								{ return KeywordLexemeAction(AND); }
"or"								{ return KeywordLexemeAction(OR); }
"not"								{ return KeywordLexemeAction(NOT); }
"true"								{ return KeywordLexemeAction(TRUE); }
"false"								{ return KeywordLexemeAction(FALSE); }
"break"								{ return KeywordLexemeAction(BREAK); }
"continue"							{ return KeywordLexemeAction(CONTINUE); }

"led"								{ return KeywordLexemeAction(LED); }
"buzzer"							{ return KeywordLexemeAction(BUZZER); }
"button"							{ return KeywordLexemeAction(BUTTON); }
"potentiometer"						{ return KeywordLexemeAction(POTENTIOMETER); }
"servo"								{ return KeywordLexemeAction(SERVO); }
"ultrasonic"						{ return KeywordLexemeAction(ULTRASONIC); }
"dht11"								{ return KeywordLexemeAction(DHT11); }
"lcd"								{ return KeywordLexemeAction(LCD); }

"=="								{ return OperatorLexemeAction(EQ); }
"!="								{ return OperatorLexemeAction(NEQ); }
"<="								{ return OperatorLexemeAction(LTE); }
">="								{ return OperatorLexemeAction(GTE); }
"<"									{ return OperatorLexemeAction(LT); }
">"									{ return OperatorLexemeAction(GT); }
"="									{ return OperatorLexemeAction(ASSIGN); }
"+"									{ return OperatorLexemeAction(PLUS); }
"-"									{ return OperatorLexemeAction(MINUS); }
"*"									{ return OperatorLexemeAction(STAR); }
"/"									{ return OperatorLexemeAction(SLASH); }
"%"									{ return OperatorLexemeAction(PERCENT); }

"{"									{ return SymbolLexemeAction(OPEN_BRACE); }
"}"									{ return SymbolLexemeAction(CLOSE_BRACE); }
"("									{ return SymbolLexemeAction(OPEN_PARENTHESIS); }
")"									{ return SymbolLexemeAction(CLOSE_PARENTHESIS); }
";"									{ return SymbolLexemeAction(SEMICOLON); }
"."									{ return SymbolLexemeAction(DOT); }
","									{ return SymbolLexemeAction(COMMA); }
":"									{ return SymbolLexemeAction(COLON); }
"["									{ return SymbolLexemeAction(OPEN_BRACKET); }
"]"									{ return SymbolLexemeAction(CLOSE_BRACKET); }

[0-9]+(ms|us|s)						{ return TimeLiteralLexemeAction(); }
[0-9]+"."[0-9]+						{ return FloatLexemeAction(); }
[0-9]+								{ return IntegerLexemeAction(); }
\"[^\"]*\"							{ return StringLexemeAction(); }
[a-zA-Z_][a-zA-Z0-9_]*				{ return IdentifierLexemeAction(); }

{whitespace}						{ return IgnoredLexemeAction(); }
.									{ return UnknownLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"

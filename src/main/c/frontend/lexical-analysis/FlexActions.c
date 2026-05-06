#include "FlexActions.h"

#include <ctype.h>
#include <errno.h>
#include <string.h>

/* MODULE INTERNAL STATE */

static bool _logIgnoredLexemes = true;
static InputBuffer * _inputBuffer = NULL;
static LexicalAnalyzer * _lexicalAnalyzer = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownFlexActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: FlexActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	if (_inputBuffer != NULL) {
		destroyInputBuffer(_inputBuffer);
		_inputBuffer = NULL;
	}
	_lexicalAnalyzer = NULL;
}

ModuleDestructor initializeFlexActionsModule(LexicalAnalyzer * lexicalAnalyzer) {
	_inputBuffer = NULL;
	_lexicalAnalyzer = lexicalAnalyzer;
	_logger = createLogger("FlexActions");
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	return _shutdownFlexActionsModule;
}

/* PRIVATE FUNCTIONS */

static void _logTokenAction(const char * actionName, Token * token);

static char * _copyString(const char * source);
static char * _unquoteStringLiteral(const char * lexeme);
static int _parseTimeMs(const char * lexeme);

/**
 * Logs a lexical-analyzer action over a token in DEBUGGING level.
 */
static void _logTokenAction(const char * actionName, Token * token) {
	char * _lexeme = escape(token->lexeme);
	logDebugging(_logger, WARNING_COLOR "%s" DEFAULT_COLOR ": Token(context=%d, label=%d, length=%d, lexeme=%s\"%s\"%s, line=%d, semanticValue=%p)",
		actionName,
		token->context,
		token->label,
		token->length,
		INFORMATION_COLOR, _lexeme, DEFAULT_COLOR,
		token->line,
		token->semanticValue);
	free(_lexeme);
	_lexeme = NULL;
}

static char * _copyString(const char * source) {
	if (source == NULL) {
		return NULL;
	}
	size_t length = strlen(source);
	char * copy = (char *) calloc(length + 1, sizeof(char));
	if (copy == NULL) {
		return NULL;
	}
	memcpy(copy, source, length);
	copy[length] = '\0';
	return copy;
}

static char * _unquoteStringLiteral(const char * lexeme) {
	if (lexeme == NULL) {
		return NULL;
	}
	size_t length = strlen(lexeme);
	if (length < 2 || lexeme[0] != '"' || lexeme[length - 1] != '"') {
		return _copyString(lexeme);
	}
	// Minimal unquote (keeps escapes as-is for now).
	size_t innerLength = length - 2;
	char * unquoted = (char *) calloc(innerLength + 1, sizeof(char));
	if (unquoted == NULL) {
		return NULL;
	}
	memcpy(unquoted, lexeme + 1, innerLength);
	unquoted[innerLength] = '\0';
	return unquoted;
}

static int _parseTimeMs(const char * lexeme) {
	if (lexeme == NULL) {
		return 0;
	}
	size_t length = strlen(lexeme);
	if (length < 2) {
		return 0;
	}

	// Supports: <digits>ms, <digits>s
	if (length >= 3 && strcmp(lexeme + (length - 2), "ms") == 0) {
		return atoi(lexeme);
	}
	if (lexeme[length - 1] == 's') {
		int seconds = atoi(lexeme);
		return seconds * 1000;
	}
	return 0;
}

/* PUBLIC FUNCTIONS */

CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context) {
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus EOFLexemeAction() {
	CompilationStatus status = IN_PROGRESS;
	Token * token = createToken(_lexicalAnalyzer, 0);
	_logTokenAction(__FUNCTION__, token);
	if (!popInputBuffer(_lexicalAnalyzer)) {
		status = pushToken(_lexicalAnalyzer, token);
		FlexContext context = currentLexicalAnalyzerContext(_lexicalAnalyzer);
		if (0 < context) {
			logError(_logger, "The final context is not closed (context=%d).", context);
			status = FAILED;
		}
	}
	destroyToken(token);
	return status;
}

CompilationStatus IgnoredLexemeAction() {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, IGNORED);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	return IN_PROGRESS;
}

CompilationStatus IntegerLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, INTEGER);
	token->semanticValue->integer = atoi(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus FloatLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, FLOAT);
	errno = 0;
	token->semanticValue->number = strtod(token->lexeme, NULL);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus IdentifierLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, IDENTIFIER);
	// Copy lexeme because the token is destroyed after pushing.
	token->semanticValue->string = _copyString(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus StringLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, STRING);
	token->semanticValue->string = _unquoteStringLiteral(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus TimeLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, TIME);
	token->semanticValue->timeMs = _parseTimeMs(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus AnalogPinLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, ANALOG_PIN);
	// Expects lexeme like "A0".."A5".
	int pin = 0;
	if (token->lexeme != NULL && strlen(token->lexeme) >= 2) {
		pin = token->lexeme[1] - '0';
	}
	token->semanticValue->analogPin = pin;
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus TokenLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus LeaveMultilineCommentLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	return IN_PROGRESS;
}

CompilationStatus UnknownLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, UNKNOWN);
	_logTokenAction(__FUNCTION__, token);
	destroyToken(token);
	return FAILED;
}

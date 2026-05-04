#include "FlexActions.h"
#include <ctype.h>
#include <stdlib.h>
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
static char * _duplicateText(const char * text);
static char * _unescapeStringLiteral(const char * text);
static EzTimeValue _parseTimeValue(const char * text);

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

static char * _duplicateText(const char * text) {
	const size_t length = strlen(text);
	char * copy = calloc(length + 1, sizeof(char));
	strcpy(copy, text);
	return copy;
}

static char * _unescapeStringLiteral(const char * text) {
	size_t length = strlen(text);
	if (length < 2) {
		return _duplicateText("");
	}
	char * output = calloc(length + 1, sizeof(char));
	size_t outIndex = 0;
	for (size_t i = 1; i + 1 < length; ++i) {
		char current = text[i];
		if (current == '\\' && i + 1 < length - 1) {
			char next = text[i + 1];
			switch (next) {
				case 'n': output[outIndex++] = '\n'; break;
				case 't': output[outIndex++] = '\t'; break;
				case 'r': output[outIndex++] = '\r'; break;
				case '\\': output[outIndex++] = '\\'; break;
				case '\"': output[outIndex++] = '\"'; break;
				default: output[outIndex++] = next; break;
			}
			++i;
		}
		else {
			output[outIndex++] = current;
		}
	}
	output[outIndex] = '\0';
	return output;
}

static EzTimeValue _parseTimeValue(const char * text) {
	EzTimeValue value = { .value = 0, .unit = EZ_TIME_MS };
	const size_t length = strlen(text);
	if (length >= 2 && text[length - 1] == 's' && text[length - 2] == 'm') {
		value.unit = EZ_TIME_MS;
		value.value = atoi(text);
	}
	else if (length >= 1 && text[length - 1] == 's') {
		value.unit = EZ_TIME_S;
		value.value = atoi(text);
	}
	return value;
}

/* PUBLIC FUNCTIONS */

CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus SimpleTokenLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus EnterImportExpressionLexemeAction(FlexContext context) {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, OPEN_BRACE);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	enterLexicalAnalyzerContext(_lexicalAnalyzer, context);
	return IN_PROGRESS;
}

CompilationStatus EnterMultilineCommentLexemeAction(FlexContext context) {
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, 0);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
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
		Token * token = createToken(_lexicalAnalyzer, 0);
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
	token->semanticValue->floatValue = strtod(token->lexeme, NULL);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus IdentifierLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, IDENTIFIER);
	token->semanticValue->string = _duplicateText(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus StringLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, STRING);
	token->semanticValue->string = _unescapeStringLiteral(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus BooleanLexemeAction(const bool value) {
	Token * token = createToken(_lexicalAnalyzer, BOOLEAN);
	token->semanticValue->boolean = value;
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus TimeLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, TIME);
	token->semanticValue->timeValue = _parseTimeValue(token->lexeme);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus LeaveImportExpressionLexemeAction() {
	pushInputBuffer(_inputBuffer);
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, CLOSE_BRACE);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	return IN_PROGRESS;
}

CompilationStatus LeaveMultilineCommentLexemeAction() {
	leaveLexicalAnalyzerContext(_lexicalAnalyzer);
	if (_logIgnoredLexemes) {
		Token * token = createToken(_lexicalAnalyzer, 0);
		_logTokenAction(__FUNCTION__, token);
		destroyToken(token);
	}
	return IN_PROGRESS;
}

CompilationStatus ParenthesisLexemeAction(TokenLabel label) {
	Token * token = createToken(_lexicalAnalyzer, label);
	_logTokenAction(__FUNCTION__, token);
	CompilationStatus status = pushToken(_lexicalAnalyzer, token);
	destroyToken(token);
	return status;
}

CompilationStatus SubexpressionLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, 0);
	_inputBuffer = createInputBuffer(_lexicalAnalyzer, token->lexeme);
	if (_logIgnoredLexemes) {
		_logTokenAction(__FUNCTION__, token);
	}
	destroyToken(token);
	return IN_PROGRESS;
}

CompilationStatus UnknownLexemeAction() {
	Token * token = createToken(_lexicalAnalyzer, 0);
	_logTokenAction(__FUNCTION__, token);
	destroyToken(token);
	return FAILED;
}

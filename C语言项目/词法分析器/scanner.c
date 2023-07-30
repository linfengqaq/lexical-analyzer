//scanner.c
#include "scanner.h"
#include <stdbool.h>

typedef struct {
	const char* start;	//��¼Token ����ʼλ��
	const char* current;	//��¼Token ��ĩβλ��+1
	int line;			// ������Դ�������һ����
}Scanner;

//ȫ�ֱ���
Scanner scanner;

void initScanner(const char* source) {
	scanner.start = source;	//��ʼλ��Ϊ0
	scanner.current = source;	//ĩβλ��Ϊ0
	scanner.line = 1;	//��ʼ����Ϊ0
}

static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||	//������isapha�滻��
		c == '_';
}

static bool isDigit(char c) {
	return  c >= '0' && c <= '9';
}

static bool isAtEnd() {
	return *scanner.current == '\0';
}
//�ȷ��ص�ǰ����ĸ����current++
static char advance() {
	return *scanner.current++;
}

static char peek() {
	return *scanner.current;
}

static char peekNext() {
	if (isAtEnd()) return '\0';
	return *(scanner.current + 1);
}

//��Token����
static bool match(char expected) {
	if (isAtEnd()) return false;
	if (peek() != expected) return false;
	scanner.current++;
	return true;
}

//����TokenType��������Ӧ���͵�Token��������
static Token makeToken(TokenType type) {
	Token token;
	//ͬ������
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	token.line = scanner.line;
	return token;
}

// �������ܽ��������ʱ�����Ǵ���һ��ERROR Token. ���磺����@��$�ȷ���ʱ�������ַ������ַ�û�ж�Ӧ��������ʱ��
static Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;
	return token;
}

static void skipWhitespace() {
	//�������հ��ַ�
	while (peek() == ' ' || peek() == '\r' || peek() == '\t' || peek() == '\n') {
		if (peek() == '\n') scanner.line++;//�������з�++
		advance();//��ַǰ��
	}//peek() != ' ' || peek() != '\r' || peek() != '\t' || peek() != '\n'
	//���������е�ע��
	if (peek() == '/'&& peekNext() == '/') {
		while (advance() != '\n');//*sanner.current == '\n',	current����һ��
		scanner.line++;
	}
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
	int len = (int)(scanner.current - scanner.start);
	if (start + length == len && memcmp(scanner.start + start, rest, length) == 0) {
		return type;
	}
	return TOKEN_IDENTIFIER;
}
//�жϵ�ǰToken�����Ǳ�ʶ�����ǹؼ���
static TokenType identifierType() {
	char c = scanner.start[0];
	//��switch���ʵ��Trie��
	switch (c)
	{
		case 'b':	return checkKeyword(1, 4, "reak", TOKEN_BREAK);
		case 'c':
			if (checkKeyword(1, 3, "ase", TOKEN_CASE)) return checkKeyword(1, 3, "ase", TOKEN_CASE);
			else if (checkKeyword(1, 3, "har", TOKEN_CHAR)) return checkKeyword(1, 3, "har", TOKEN_CHAR);
			else if (checkKeyword(1, 4, "onst", TOKEN_CONST)) return checkKeyword(1, 4, "onst", TOKEN_CONST);
			else if (checkKeyword(1, 7, "ontinue", TOKEN_CONTINUE)) return checkKeyword(1, 7, "ontinue", TOKEN_CONTINUE);
			else return TOKEN_IDENTIFIER;
		case 'd':
			if (checkKeyword(1, 5, "ouble", TOKEN_DOUBLE)) return checkKeyword(1, 5, "ouble", TOKEN_DOUBLE);
			else if (checkKeyword(1, 6, "efault", TOKEN_DEFAULT)) return checkKeyword(1, 6, "efault", TOKEN_DEFAULT);
			else return TOKEN_IDENTIFIER;
		case 'e':
			if (checkKeyword(1, 3, "lse", TOKEN_ELSE)) return checkKeyword(1, 3, "lse", TOKEN_ELSE);
			else if (checkKeyword(1, 3, "num", TOKEN_ENUM)) return checkKeyword(1, 3, "num", TOKEN_ENUM);
			else return TOKEN_IDENTIFIER;
		case 'f':
			if (checkKeyword(1, 2, "or", TOKEN_FOR)) return checkKeyword(1, 2, "or", TOKEN_FOR);
			else if (checkKeyword(1, 4, "loat", TOKEN_FLOAT)) return checkKeyword(1, 4, "loat", TOKEN_FLOAT);
			else return TOKEN_IDENTIFIER;
		case 'g':	return checkKeyword(1, 3, "oto", TOKEN_GOTO);
		case 'i':
			if (checkKeyword(1, 1, "f", TOKEN_IF)) return checkKeyword(1, 1, "f", TOKEN_IF);
			else if (checkKeyword(1, 2, "nt", TOKEN_INT)) return checkKeyword(1, 2, "nt", TOKEN_INT);
			else return TOKEN_IDENTIFIER;
		case 'l':	return checkKeyword(1, 3, "ong", TOKEN_LONG);
		case 'r':	return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
		case 's':
			if (checkKeyword(1, 4, "hort", TOKEN_SHORT)) return checkKeyword(1, 4, "hort", TOKEN_SHORT);
			else if (checkKeyword(1, 5, "truct", TOKEN_STRUCT)) return checkKeyword(1, 5, "truct", TOKEN_STRUCT);
			else if (checkKeyword(1, 5, "witch", TOKEN_SWITCH)) return checkKeyword(1, 5, "witch", TOKEN_SWITCH);
			else if (checkKeyword(1, 5, "igned", TOKEN_SIGNED)) return checkKeyword(1, 5, "igned", TOKEN_SIGNED);
			else if (checkKeyword(1, 5, "izeof", TOKEN_SIZEOF)) return checkKeyword(1, 5, "izeof", TOKEN_SIZEOF);
			else return TOKEN_IDENTIFIER;
		case 't':	return checkKeyword(1, 6, "ypedef", TOKEN_TYPEDEF);
		case 'u':
			if (checkKeyword(1, 4, "nion", TOKEN_UNION)) return checkKeyword(1, 4, "nion", TOKEN_UNION);
			else if (checkKeyword(1, 7, "nsigned", TOKEN_UNSIGNED)) return checkKeyword(1, 7, "nsigned", TOKEN_UNSIGNED);
			else return TOKEN_IDENTIFIER;
		case 'v':	return checkKeyword(1, 3, "oid", TOKEN_VOID);
		case 'w':	return checkKeyword(1, 4, "hile", TOKEN_WHILE);
	}
	return TOKEN_IDENTIFIER;
}

static Token identifier()
{
	// IDENTIFIER����: ��ĸ�����ֺ��»���
	while (isAlpha(peek()) || isDigit(peek())) {
		advance();
	}//*current != isAlpha || != isDigit
	// ������Token�����Ǳ�ʶ��, Ҳ�����ǹؼ���, identifierType()������ȷ��Token���͵�
	return makeToken(identifierType());
}

static Token number() {
	while (isDigit(peek()) || peek() == '.') {
		if (peek()=='.' && !isDigit(peekNext())) return errorToken("illegal number");
		advance();
	}
	return makeToken(TOKEN_NUMBER);
}

static Token string() {
	while (peek() != '"' && peek() != '\n') {
		advance();
	}
	if (peek() == '\n') {
		return errorToken("illegal string.");
	}
	advance();
	return makeToken(TOKEN_STRING);
}

static Token character() {
	while (peek() != '\n') {
		if (advance() == '\'')
			return makeToken(TOKEN_CHARACTER);
	}//peek() == '\n'
	return errorToken("illegal character.");
}

/***************************************************************************************
 *                                   	�ִ�											  *
 ***************************************************************************************/
Token scanToken() {
	// ����ǰ�ÿհ��ַ���ע��
	skipWhitespace();
	// ��¼��һ��Token����ʼλ��
	scanner.start = scanner.current;

	if (isAtEnd()) return makeToken(TOKEN_EOF);

	char c = advance();
	if (isAlpha(c)) return identifier();
	if (isDigit(c)) return number();

	switch (c) {
		// single-character tokens
	case '(': return makeToken(TOKEN_LEFT_PAREN);
	case ')': return makeToken(TOKEN_RIGHT_PAREN);
	case '[': return makeToken(TOKEN_LEFT_BRACKET);
	case ']': return makeToken(TOKEN_RIGHT_BRACKET);
	case '{': return makeToken(TOKEN_LEFT_BRACE);
	case '}': return makeToken(TOKEN_RIGHT_BRACE);
	case ',': return makeToken(TOKEN_COMMA);
	case '.': return makeToken(TOKEN_DOT);
	case ';': return makeToken(TOKEN_SEMICOLON);
	case '~': return makeToken(TOKEN_TILDE);
	case '#': return makeToken(TOKEN_HASH);

		// one or two characters tokens
	case '+':
		if (match('+')) return makeToken(TOKEN_PLUS_PLUS);
		else if (match('=')) return makeToken(TOKEN_PLUS_EQUAL);
		else return makeToken(TOKEN_PLUS);

	case '-':
		if (match('-')) return makeToken(TOKEN_MINUS_MINUS);
		else if (match('=')) return makeToken(TOKEN_MINUS_EQUAL);
		else if (match('>')) return makeToken(TOKEN_MINUS_GREATER);
		else return makeToken(TOKEN_MINUS);

	case '*':
		if (match('=')) return makeToken(TOKEN_STAR_EQUAL);
		else return makeToken(TOKEN_STAR);

	case '/':
		if (match('=')) return makeToken(TOKEN_SLASH_EQUAL);
		else return makeToken(TOKEN_SLASH);

	case '%':
		if (match('=')) return makeToken(TOKEN_PERCENT_EQUAL);
		else return makeToken(TOKEN_PERCENT);

	case '&':
		if (match('&')) return makeToken(TOKEN_AMPER_AMPER);
		else if (match('=')) return makeToken(TOKEN_AMPER_EQUAL);
		else return makeToken(TOKEN_AMPER);

	case '|':
		if (match('|')) return makeToken(TOKEN_PIPE_PIPE);
		else if (match('=')) return makeToken(TOKEN_PIPE_EQUAL);
		else return makeToken(TOKEN_PIPE);

	case '^':
		if (match('=')) return makeToken(TOKEN_HAT_EQUAL);
		else return makeToken(TOKEN_HAT);

	case '=':
		if (match('=')) return makeToken(TOKEN_EQUAL_EQUAL);
		else return makeToken(TOKEN_EQUAL);

	case '!':
		if (match('=')) return makeToken(TOKEN_BANG_EQUAL);
		else return makeToken(TOKEN_BANG);

	case '<':
		if (match('<')) return makeToken(TOKEN_LESS_LESS);
		else if (match('=')) return makeToken(TOKEN_LESS_EQUAL);
		else return makeToken(TOKEN_LESS);

	case '>':
		if (match('>')) return makeToken(TOKEN_GREATER_GREATER);
		else if (match('=')) return makeToken(TOKEN_GREATER_EQUAL);
		else return makeToken(TOKEN_GREATER);


		// various-character tokens
	case '"': return string();
	case '\'': return character();
	}


	return errorToken("Unexpected character.");
}

#include "../header/lexer.hpp"

#include <ctype.h>


// Utility functions

bool isLetter(char curChar)
{
	return isalpha(curChar) || curChar == '_';
}

bool isDigit(char curChar)
{
	// check
	return isdigit(curChar);
}


void Lexer::New(std::string &input)
{
	this->input = input;
	readPos = 0;
	readChar();
}

void Lexer::readChar()
{
	if (readPos >= input.size())
		curChar = 0; // 0 -> end of file
	else
		curChar = input[readPos];

	pos = readPos;
	readPos++;
}

char Lexer::peekChar()
{
	if (readPos >= input.size())
		return 0;
	else
		return input[readPos];
}

std::string Lexer::readIdentifier()
{
	int start = pos;

	while (isLetter(curChar))
		readChar();

	return input.substr(start, pos - start);
}

std::string Lexer::readNumber()
{
	int start = pos;

	while (isDigit(curChar))
		readChar();

	return input.substr(start, pos - start); // check
}

std::string Lexer::readString()
{
	int start = pos + 1;

	do
	{
		readChar();
	} while (curChar != '"' && curChar != 0);

	return input.substr(start, pos - start);
}

void Lexer::skipWhitespace()
{
	while (curChar == ' ' || curChar == '\t' || curChar == '\r' || curChar == '\n')
		readChar();
}

Token Lexer::nextToken()
{
	Token token;

	skipWhitespace();

	switch (curChar)
	{
	case '=':
		if (peekChar() == '=')
		{
			token = Token(EQ, "==");
			readChar();
		}
		else
			token = Token(ASSIGN, curChar);

		readChar();
		break;

	case '!':
		if (peekChar() == '=')
		{
			token = Token(NEQ, "!=");
			readChar();
		}
		else
			token = Token(BANG, curChar);

		readChar();
		break;

	case '<':
		if (peekChar() == '=')
		{
			token = Token(LTE, "<=");
			readChar();
		}
		else
			token = Token(LT, curChar);

		readChar();
		break;

	case '>':
		if (peekChar() == '=')
		{
			token = Token(GTE, ">=");
			readChar();
		}
		else
			token = Token(GT, curChar);

		readChar();
		break;

	case '+':
		token = Token(PLUS, curChar);
		readChar();
		break;

	case '-':
		token = Token(MINUS, curChar);
		readChar();
		break;

	case '*':
		token = Token(ASTERISK, curChar);
		readChar();
		break;

	case '/':
		token = Token(SLASH, curChar);
		readChar();
		break;

	case '%':
		token = Token(MODULO, curChar);
		readChar();
		break;

	case ',':
		token = Token(COMMA, curChar);
		readChar();
		break;

	case ';':
		token = Token(SEMICOLON, curChar);
		readChar();
		break;

	case ':':
		token = Token(COLON, curChar);
		readChar();
		break;

	case '(':
		token = Token(LPAREN, curChar);
		readChar();
		break;

	case ')':
		token = Token(RPAREN, curChar);
		readChar();
		break;

	case '{':
		token = Token(LBRACE, curChar);
		readChar();
		break;

	case '}':
		token = Token(RBRACE, curChar);
		readChar();
		break;

	case '[':
		token = Token(LBRACKET, curChar);
		readChar();
		break;

	case ']':
		token = Token(RBRACKET, curChar);
		readChar();
		break;

	case '"':
		token = Token(STRING, readString());
		readChar();
		break;

	case 0:
		token = Token(END, "");
		readChar();
		break;

	default:
		if (isLetter(curChar))
		{
			token.literal = readIdentifier();
			token.type = LookupIdentifier(token.literal);
			return token;
		}
		else if (isDigit(curChar))
		{
			token.type = INT;
			token.literal = readNumber();
			return token;
		}
		else
		{
			token = Token(ILLEGAL, curChar);
			readChar();
		}
		break;
	}

	return token;
}
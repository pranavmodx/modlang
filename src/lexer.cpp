#include "../header/lexer.hpp"

#include <ctype.h>


// Utility functions

bool is_letter(char cur_char)
{
	return isalpha(cur_char) || cur_char == '_';
}

bool is_digit(char cur_char)
{
	return isdigit(cur_char);
}

// Public functions

void Lexer::New(std::string &input)
{
	this->input = input;
	read_pos = 0;
	read_char();
}

Token Lexer::next_token()
{
	Token token;

	skip_whitespace();

	switch (cur_char)
	{
	case '=':
		if (peek_char() == '=')
		{
			token = Token(EQ, "==");
			read_char();
		}
		else
			token = Token(ASSIGN, cur_char);

		read_char();
		break;

	case '!':
		if (peek_char() == '=')
		{
			token = Token(NEQ, "!=");
			read_char();
		}
		else
			token = Token(BANG, cur_char);

		read_char();
		break;

	case '<':
		if (peek_char() == '=')
		{
			token = Token(LTE, "<=");
			read_char();
		}
		else
			token = Token(LT, cur_char);

		read_char();
		break;

	case '>':
		if (peek_char() == '=')
		{
			token = Token(GTE, ">=");
			read_char();
		}
		else
			token = Token(GT, cur_char);

		read_char();
		break;

	case '+':
		token = Token(PLUS, cur_char);
		read_char();
		break;

	case '-':
		token = Token(MINUS, cur_char);
		read_char();
		break;

	case '*':
		token = Token(ASTERISK, cur_char);
		read_char();
		break;

	case '/':
		token = Token(SLASH, cur_char);
		read_char();
		break;

	case '%':
		token = Token(MODULO, cur_char);
		read_char();
		break;

	case ',':
		token = Token(COMMA, cur_char);
		read_char();
		break;

	case ';':
		token = Token(SEMICOLON, cur_char);
		read_char();
		break;

	case '(':
		token = Token(LPAREN, cur_char);
		read_char();
		break;

	case ')':
		token = Token(RPAREN, cur_char);
		read_char();
		break;

	case '{':
		token = Token(LBRACE, cur_char);
		read_char();
		break;

	case '}':
		token = Token(RBRACE, cur_char);
		read_char();
		break;

	case '[':
		token = Token(LBRACKET, cur_char);
		read_char();
		break;

	case ']':
		token = Token(RBRACKET, cur_char);
		read_char();
		break;

	case '"':
		token = Token(STRING, read_string());
		read_char();
		break;

	case 0:
		token = Token(END, "");
		read_char();
		break;

	default:
		if (is_letter(cur_char))
		{
			token.literal = read_identifier();
			token.type = LookupIdentifier(token.literal);
		}
		else if (is_digit(cur_char))
		{
			token.type = INT;
			token.literal = read_number();
		}
		else
		{
			token = Token(ILLEGAL, cur_char);
			read_char();
		}
		break;
	}
	return token;
}

// Private functions

void Lexer::read_char()
{
	if (read_pos >= input.size())
		cur_char = 0;
	else
		cur_char = input[read_pos];

	pos = read_pos;
	read_pos++;
}

char Lexer::peek_char()
{
	if (read_pos >= input.size())
		return 0;
	else
		return input[read_pos];
}

std::string Lexer::read_identifier()
{
	int start = pos;

	while (is_letter(cur_char))
		read_char();

	return input.substr(start, pos - start);
}

std::string Lexer::read_number()
{
	int start = pos;

	while (is_digit(cur_char))
		read_char();

	return input.substr(start, pos - start);
}

std::string Lexer::read_string()
{
	int start = pos + 1;

	do
	{
		read_char();
	} while (cur_char != '"' && cur_char != 0);

	return input.substr(start, pos - start);
}

void Lexer::skip_whitespace()
{
	while (cur_char == ' ' || cur_char == '\t' || cur_char == '\r' || cur_char == '\n')
		read_char();
}
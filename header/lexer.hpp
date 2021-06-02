#pragma once

#include "token.hpp"

#include <string>


class Lexer
{
private:
	std::string input;
	int pos; // current position in input (points to current char)
	int readPos; // current reading position in input (after current char)
	char curChar; // current char under examination

	void skipWhitespace();
	void readChar();
	char peekChar();
	std::string readIdentifier();
	std::string readNumber();
	std::string readString();

public:
	void New(std::string &input);
	Token nextToken();
};

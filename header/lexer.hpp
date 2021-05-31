#pragma once

#include "token.hpp"

#include <string>

class Lexer
{
public:
	void New(std::string &input);
	Token next_token();

private:
	std::string input;
	int pos; // current position in input (points to current char)
	int read_pos; // current reading position in input (after current char)
	char cur_char; // current char under examination

	void read_char();
	char peek_char();
	std::string read_identifier();
	std::string read_number();
	std::string read_string();
	void skip_whitespace();
};

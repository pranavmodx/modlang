#pragma once

#include "../header/token.hpp"
#include "../header/lexer.hpp"
#include "../header/ast.hpp"

#include <vector>


class Parser {
private:
	Lexer lexer;
	Token curToken;
	Token peekToken;
	std::vector<std::string> errors;

	Statement* parseStatement();
	LetStatement* parseLetStatement();
	ReturnStatement* parseReturnStatement();

	bool expectPeek(const TokenType &tokenType);
	void peekError(const TokenType &tokenType);

public:
	void New(Lexer &lexer);
	void nextToken();
	std::vector<std::string> Errors();
	void resetErrors();

	Program* ParseProgram();
};
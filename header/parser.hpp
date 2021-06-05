#pragma once

#include "../header/token.hpp"
#include "../header/lexer.hpp"
#include "../header/ast.hpp"

#include <vector>
#include <unordered_map>


enum Precedence
{
	LOWEST,
	EQUALS,		 // ==
	LESSGREATER, // > or <
	SUM,		 // +
	PRODUCT,	 // *
	PREFIX,		 // -X or !X
	CALL,		 // myFunction(X)
	INDEX,		 // []
};

class Parser;

typedef Expression *(Parser::*PrefixParseFn)(); // Pointer to function returning Expression* with no parameters
typedef Expression *(Parser::*InfixParseFn)(Expression *); // Pointer to function returning Expression* which takes in Expression* as parameter
// // We can now say -> PrefixParseFn funcPtr = funcToPointTo;

class Parser
{
private:
	Lexer lexer;
	Token curToken;
	Token peekToken;
	std::vector<std::string> errors;

	std::unordered_map<TokenType, Precedence> precedences;

	std::unordered_map<TokenType, PrefixParseFn> prefixParseFns;
	std::unordered_map<TokenType, InfixParseFn> infixParseFns;

	Precedence curPrecedence();
	Precedence peekPrecedence();

	Statement *parseStatement();

	LetStatement *parseLetStatement();
	ReturnStatement *parseReturnStatement();

	ExpressionStatement *parseExpressionStatement();
	Expression *parseExpression(Precedence precedence);
	Expression *parseIdentifier();
	Expression *parseIntegerLiteral();
	Expression *parsePrefixExpression();
	Expression *parseInfixExpression(Expression *);

	bool expectPeek(const TokenType &tokenType);
	void peekError(const TokenType &tokenType);

public:
	Parser();
	void New(Lexer &lexer);
	void nextToken();
	std::vector<std::string> Errors();
	void resetErrors();

	Program *ParseProgram();
};
#pragma once

#include <vector>
#include <unordered_map>

#include "../header/token.hpp"
#include "../header/lexer.hpp"
#include "../header/ast.hpp"

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

typedef Expression *(Parser::*PrefixParseFn)();			   // Pointer to function returning Expression* with no parameters
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
	AssignStatement *parseAssignStatement();
	ReturnStatement *parseReturnStatement();
	BlockStatement *parseBlockStatement();
	ExpressionStatement *parseExpressionStatement();

	std::vector<Identifier *> parseFunctionParameters();
	std::vector<Expression *> parseExpressionList();

	Expression *parseExpression(Precedence precedence);

	Expression *parsePrefixExpression();
	Expression *parseInfixExpression(Expression *);

	Expression *parseGroupedExpression();
	Expression *parseIfExpression();
	Expression *parseWhileExpression();
	Expression *parseCallExpression(Expression *function);
	Expression *parseIndexExpression(Expression *array);

	Expression *parseIdentifier();

	Expression *parseIntegerLiteral();
	Expression *parseBooleanLiteral();
	Expression *parseStringLiteral();

	Expression *parseFunctionLiteral();

	Expression *parseArrayLiteral();
	Expression *parseHashMapLiteral();
	Expression *parseHashSetLiteral();
	Expression *parseStackLiteral();
	Expression *parseQueueLiteral();
	Expression *parseDequeLiteral();
	Expression *parseMaxHeapLiteral();
	Expression *parseMinHeapLiteral();

	bool expectPeek(const TokenType &tokenType);
	void peekError(const TokenType &tokenType);
	void peekTypeError();
	void curTypeError(TokenType type);
	void noPrefixParseFnError(TokenType type);

public:
	Parser();
	void New(Lexer &lexer);
	void nextToken();
	std::vector<std::string> Errors();
	void resetErrors();

	Program *ParseProgram();
};
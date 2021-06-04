#include "../header/parser.hpp"

#include <stdexcept>

std::unordered_map<TokenType, Precedence> precedences({
	{EQ, EQUALS},
	{NEQ, EQUALS},
	{LT, LESSGREATER},
	{GT, LESSGREATER},
	{LTE, LESSGREATER},
	{GTE, LESSGREATER},
	{PLUS, SUM},
	{MINUS, SUM},
	{SLASH, PRODUCT},
	{ASTERISK, PRODUCT},
	{MODULO, PRODUCT},
	{LPAREN, CALL},
	{LBRACKET, INDEX},
});

void Parser::New(Lexer &lexer)
{
	this->lexer = lexer;

	nextToken();
	nextToken();

	prefixParseFns[IDENT] = &Parser::parseIdentifier; // address of method
	prefixParseFns[INT] = &Parser::parseIntegerLiteral;
}

Expression *Parser::parseIdentifier()
{
	Identifier *ident = new Identifier(curToken, curToken.literal);

	return ident;
}

void Parser::nextToken()
{
	curToken = peekToken;
	peekToken = lexer.nextToken(); // note that it's lexer's nextToken() and not parser's
}

Program *Parser::ParseProgram()
{
	Program *program = new Program();

	while (curToken.type != END)
	{
		Statement *stmt = parseStatement();

		if (stmt != nullptr)
			program->statements.push_back(stmt);

		nextToken();
	}

	return program;
}

Statement *Parser::parseStatement()
{
	TokenType curTokenType = curToken.type;

	// switch takes only int or enum val, so used if
	if (curTokenType == LET)
		return parseLetStatement();
	else if (curTokenType == RETURN)
		return parseReturnStatement();
	else
		return parseExpressionStatement();

	return nullptr;
}

LetStatement *Parser::parseLetStatement()
{
	LetStatement *stmt = new LetStatement();
	stmt->token = curToken;

	if (!expectPeek(IDENT))
		return nullptr;

	stmt->name.token = curToken;
	stmt->name.value = curToken.literal;

	if (!expectPeek(ASSIGN))
		return nullptr;

	while (curToken.type != SEMICOLON)
		nextToken();

	// if (peekToken.type == SEMICOLON)
	// nextToken();

	return stmt;
}

bool Parser::expectPeek(const TokenType &tokenType)
{
	if (peekToken.type == tokenType)
	{
		nextToken();
		return true;
	}
	else
	{
		peekError(tokenType);
		return false;
	}
}

std::vector<std::string> Parser::Errors()
{
	return errors;
}

void Parser::resetErrors()
{
	errors.clear();
}

void Parser::peekError(const TokenType &tokenType)
{
	std::string msg = "expected token to be " + tokenType + ", got " + peekToken.type + " instead";

	errors.push_back(msg);
}

ReturnStatement *Parser::parseReturnStatement()
{
	ReturnStatement *stmt = new ReturnStatement();
	stmt->token = curToken;

	nextToken();

	while (curToken.type != SEMICOLON)
		nextToken();

	return stmt;
}

ExpressionStatement *Parser::parseExpressionStatement()
{
	ExpressionStatement *stmt = new ExpressionStatement();
	stmt->token = curToken;
	stmt->expression = parseExpression(LOWEST);

	if (peekToken.type == SEMICOLON)
		nextToken();

	return stmt;
}

Expression *Parser::parseExpression(Precedence precedence)
{
	if (prefixParseFns.find(curToken.type) == prefixParseFns.end())
		return nullptr;

	PrefixParseFn prefix = prefixParseFns[curToken.type];

	Expression *leftExp = (this->*prefix)(); // ->* operator is used to invoke method pointer

	return leftExp;
}

Expression *Parser::parseIntegerLiteral()
{
	IntegerLiteral *intLit = new IntegerLiteral();
	intLit->token = curToken;

	try
	{
		intLit->value = std::stoi(curToken.literal);
	}
	catch (const std::invalid_argument e)
	{
		errors.push_back("cannot parse " + curToken.literal + " to int");
		return nullptr;
	}
	catch (const std::out_of_range e)
	{
		errors.push_back("the integer " + curToken.literal + " is out of range");
		return nullptr;
	}

	return intLit;
}
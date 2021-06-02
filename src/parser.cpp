#include "../header/parser.hpp"


void Parser::New(Lexer &lexer)
{
	this->lexer = lexer;

	nextToken();
	nextToken();
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

void Parser::resetErrors() {
	errors.clear();
}

void Parser::peekError(const TokenType &tokenType)
{
	std::string msg = "expected token to be " + tokenType + ", got " + peekToken.type + " instead";

	errors.push_back(msg);
}

ReturnStatement* Parser::parseReturnStatement() {
  ReturnStatement* stmt = new ReturnStatement();
  stmt->token = curToken;

  nextToken();

  while (peekToken.type != SEMICOLON)
    nextToken();

  return stmt;
}
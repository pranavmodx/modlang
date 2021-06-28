#include "../header/parser.hpp"

#include <stdexcept>

Parser::Parser()
{
	precedences =
		{
			{EQ, EQUALS},
			{NEQ, EQUALS},
			{LTEQ, EQUALS},
			{GTEQ, EQUALS},
			{LT, LESSGREATER},
			{GT, LESSGREATER},
			{PLUS, SUM},
			{MINUS, SUM},
			{SLASH, PRODUCT},
			{ASTERISK, PRODUCT},
			{MODULO, PRODUCT},
			{LPAREN, CALL},
			{LBRACKET, INDEX},
		};
}

void Parser::New(Lexer &lexer)
{
	this->lexer = lexer;

	nextToken();
	nextToken();

	// prefix parse functions
	prefixParseFns[IDENT] = &Parser::parseIdentifier; // address of method
	prefixParseFns[INT] = &Parser::parseIntegerLiteral;

	prefixParseFns[BANG] = &Parser::parsePrefixExpression;
	prefixParseFns[MINUS] = &Parser::parsePrefixExpression;

	prefixParseFns[TRUE] = &Parser::parseBooleanLiteral;
	prefixParseFns[FALSE] = &Parser::parseBooleanLiteral;

	prefixParseFns[STRING] = &Parser::parseStringLiteral;

	prefixParseFns[IF] = &Parser::parseIfExpression;
	prefixParseFns[WHILE]  = &Parser::parseWhileExpression;
	prefixParseFns[FUNCTION] = &Parser::parseFunctionLiteral;
	prefixParseFns[HASHSET] = &Parser::parseHashSetLiteral;

	// brackets
	prefixParseFns[LPAREN] = &Parser::parseGroupedExpression;
	prefixParseFns[LBRACKET] = &Parser::parseArrayLiteral;
	prefixParseFns[LBRACE] = &Parser::parseHashMapLiteral;

	// infix parse functions
	infixParseFns[PLUS] = &Parser::parseInfixExpression;
	infixParseFns[MINUS] = &Parser::parseInfixExpression;
	infixParseFns[SLASH] = &Parser::parseInfixExpression;
	infixParseFns[ASTERISK] = &Parser::parseInfixExpression;
	infixParseFns[MODULO] = &Parser::parseInfixExpression;
	infixParseFns[EQ] = &Parser::parseInfixExpression;
	infixParseFns[NEQ] = &Parser::parseInfixExpression;
	infixParseFns[LTEQ] = &Parser::parseInfixExpression;
	infixParseFns[GTEQ] = &Parser::parseInfixExpression;
	infixParseFns[LT] = &Parser::parseInfixExpression;
	infixParseFns[GT] = &Parser::parseInfixExpression;

	// brackets
	infixParseFns[LPAREN] = &Parser::parseCallExpression;
	infixParseFns[LBRACKET] = &Parser::parseIndexExpression;
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

	nextToken();

	stmt->value = parseExpression(LOWEST);

	if (peekToken.type == SEMICOLON)
		nextToken();

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

void Parser::noPrefixParseFnError(TokenType type)
{
	errors.push_back("error: invalid symbol used in statement : " + type);
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
	std::string msg = "error: expected token to be " + tokenType + " got " + peekToken.type + " instead";

	errors.push_back(msg);
}

ReturnStatement *Parser::parseReturnStatement()
{
	ReturnStatement *stmt = new ReturnStatement();
	stmt->token = curToken;

	nextToken();

	stmt->returnValue = parseExpression(LOWEST);

	if (peekToken.type == SEMICOLON)
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
	{
		noPrefixParseFnError(curToken.type);
		return nullptr;
	}

	PrefixParseFn prefix = prefixParseFns[curToken.type];

	Expression *leftExp = (this->*prefix)(); // ->* operator is used to invoke method pointer

	while (peekToken.type != SEMICOLON && precedence < peekPrecedence())
	{
		if (infixParseFns.find(peekToken.type) == infixParseFns.end())
			return leftExp;

		InfixParseFn infix = infixParseFns[peekToken.type];
		if (infix == nullptr)
			return leftExp;

		nextToken();

		leftExp = (this->*infix)(leftExp);
	}

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
		errors.push_back("error: cannot parse " + curToken.literal + " to int");
		return nullptr;
	}
	catch (const std::out_of_range e)
	{
		errors.push_back("error: the integer " + curToken.literal + " is out of range");
		return nullptr;
	}

	return intLit;
}

Expression *Parser::parsePrefixExpression()
{
	PrefixExpression *exp = new PrefixExpression();
	exp->token = curToken;
	exp->operand = curToken.literal;

	nextToken();

	exp->right = parseExpression(PREFIX);
	return exp;
}

Expression *Parser::parseInfixExpression(Expression *left)
{
	InfixExpression *exp = new InfixExpression();
	exp->token = curToken;
	exp->operand = curToken.literal;
	exp->left = left;

	Precedence precedence = curPrecedence();

	nextToken();

	exp->right = parseExpression(precedence);

	return exp;
}

Precedence Parser::curPrecedence()
{
	if (precedences.find(curToken.type) != precedences.end())
		return precedences[curToken.type];
	return LOWEST;
}

Precedence Parser::peekPrecedence()
{
	if (precedences.find(peekToken.type) != precedences.end())
		return precedences[peekToken.type];
	return LOWEST;
}

Expression *Parser::parseBooleanLiteral()
{
	BooleanLiteral *ident = new BooleanLiteral();
	ident->token = curToken;
	ident->value = curToken.type == TRUE;
	return ident;
}

Expression *Parser::parseStringLiteral()
{
	StringLiteral *s = new StringLiteral();
	s->token = curToken;
	s->value = curToken.literal;
	return s;
}

Expression *Parser::parseGroupedExpression()
{
	nextToken(); // pass "("

	Expression *exp = parseExpression(LOWEST);

	if (!expectPeek(RPAREN))
	{
		delete exp;
		return nullptr;
	}

	return exp;
}

Expression *Parser::parseIfExpression()
{
	IfExpression *exp = new IfExpression();
	exp->token = curToken;

	// condition
	if (!expectPeek(LPAREN))
	{
		delete exp;
		return nullptr;
	}
	nextToken();
	exp->condition = parseExpression(LOWEST);
	if (!expectPeek(RPAREN))
	{
		delete exp;
		return nullptr;
	}

	// consequence
	if (!expectPeek(LBRACE))
	{
		delete exp;
		return nullptr;
	}
	exp->consequence = parseBlockStatement();

	// else
	if (peekToken.type == ELSE)
	{
		nextToken();
		if (!expectPeek(LBRACE))
		{
			delete exp;
			return nullptr;
		}
		exp->alternative = parseBlockStatement();
	}
	return exp;
}

Expression *Parser::parseWhileExpression()
{
	WhileExpression *exp = new WhileExpression();
	exp->token = curToken;

	if (!expectPeek(LPAREN))
		return nullptr;

	nextToken();

	exp->condition = parseExpression(LOWEST);

	if (!expectPeek(RPAREN))
		return nullptr;

	if (!expectPeek(LBRACE))
		return nullptr;

	exp->consequence = parseBlockStatement();

	return exp;
}

BlockStatement *Parser::parseBlockStatement()
{
	BlockStatement *block = new BlockStatement();
	block->token = curToken;

	nextToken();

	while (curToken.type != RBRACE && curToken.type != END)
	{
		Statement *stmt = parseStatement();
		if (stmt != nullptr)
			block->statements.push_back(stmt);

		nextToken();
	}

	return block;
}

Expression *Parser::parseFunctionLiteral()
{
	FunctionLiteral *fn = new FunctionLiteral();
	fn->token = curToken;

	if (!expectPeek(LPAREN))
	{ // in this function, only deal with "("
		delete fn;
		return nullptr;
	}

	fn->parameters = parseFunctionParameters();

	if (!expectPeek(LBRACE))
	{
		delete fn;
		return nullptr;
	}

	fn->body = parseBlockStatement();
	return fn;
}

std::vector<Identifier *> Parser::parseFunctionParameters()
{
	std::vector<Identifier *> parameters;

	if (peekToken.type == RPAREN)
	{ // no parameters
		nextToken();
		return parameters;
	}

	nextToken(); // pass "," or "("

	Identifier *ident = new Identifier();
	ident->token = curToken;
	ident->value = curToken.literal;
	parameters.push_back(ident);

	while (peekToken.type == COMMA)
	{
		nextToken();
		nextToken();
		Identifier *ident = new Identifier();
		ident->token = curToken;
		ident->value = curToken.literal;
		parameters.push_back(ident);
	}

	if (!expectPeek(RPAREN))
		return std::vector<Identifier *>();

	return parameters;
}

Expression *Parser::parseCallExpression(Expression *function)
{
	CallExpression *exp = new CallExpression();
	exp->token = curToken;
	exp->function = function;

	if (peekToken.type == RPAREN)
	{
		nextToken();
		return exp;
	}

	exp->arguments = parseExpressionList();
	if (!expectPeek(RPAREN))
		exp->arguments = std::vector<Expression *>();

	return exp;
}

std::vector<Expression *> Parser::parseExpressionList()
{
	std::vector<Expression *> arguments;

	nextToken();
	arguments.push_back(parseExpression(LOWEST));

	while (peekToken.type == COMMA)
	{
		nextToken();
		nextToken();
		arguments.push_back(parseExpression(LOWEST));
	}

	return arguments;
}

Expression *Parser::parseArrayLiteral()
{
	ArrayLiteral *exp = new ArrayLiteral();
	exp->token = curToken;

	if (peekToken.type == RBRACKET)
	{
		nextToken();
		return exp;
	}

	exp->elements = parseExpressionList();

	if (!expectPeek(RBRACKET))
		//exp->elements = std::vector<Expression *>();
		return nullptr;

	return exp;
}

Expression *Parser::parseIndexExpression(Expression *array)
{
	IndexExpression *exp = new IndexExpression();
	exp->token = curToken;
	exp->array = array;

	nextToken();

	exp->index = parseExpression(LOWEST);

	if (!expectPeek(RBRACKET))
		return nullptr;

	return exp;
}

Expression *Parser::parseHashMapLiteral()
{
	HashMapLiteral *exp = new HashMapLiteral();
	exp->token = curToken;

	while (peekToken.type != RBRACE)
	{
		nextToken();

		Expression *key = parseExpression(LOWEST);

		if (!expectPeek(COLON))
			return nullptr;

		nextToken();

		Expression *value = parseExpression(LOWEST);

		exp->pairs.push_back({key, value});

		if (peekToken.type != RBRACE && !expectPeek(COMMA))
			return nullptr;
	}

	if (!expectPeek(RBRACE))
		// exp->pairs = std::vector<std::pair<Expression*, Expression*>();
		return nullptr;

	return exp;
}

Expression *Parser::parseHashSetLiteral()
{
	HashSetLiteral *exp = new HashSetLiteral();
	exp->token = curToken;

	if (!expectPeek(LT))
		return nullptr;

	if (!expectPeek(GT))
		return nullptr;

	if (!expectPeek(LBRACE))
		return nullptr;

	while (peekToken.type != RBRACE)
	{
		nextToken();

		Expression *value = parseExpression(LOWEST);

		exp->pairs.push_back(value);

		if (peekToken.type != RBRACE && !expectPeek(COMMA))
			return nullptr;
	}

	if (!expectPeek(RBRACE))
		return nullptr;

	return exp;
}
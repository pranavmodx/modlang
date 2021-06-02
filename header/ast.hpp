#pragma once

#include <string>
#include <vector>

#include "token.hpp"


class Node
{
public:
	// pure virtual function
	virtual ~Node() {};
	virtual std::string tokenLiteral() = 0;
	virtual std::string getStringRepr() = 0;
	virtual std::string nodeType() = 0;
};

class Statement : public Node
{
public:
	virtual void statementNode() = 0;
	virtual std::string tokenLiteral() = 0;
	virtual std::string getStringRepr() = 0;
	virtual std::string nodeType() = 0;
};

class Program : public Node
{
public:
	std::vector<Statement *> statements;

	std::string tokenLiteral();
	std::string getStringRepr();
	std::string nodeType() { return "Program"; }
};

class Expression : public Node
{
public:
	virtual void expressionNode() = 0;
	virtual std::string tokenLiteral() = 0;
	virtual std::string getStringRepr() = 0;
	virtual std::string nodeType() = 0;
};

class Identifier : public Expression
{
public:
	Token token;
	std::string value;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr() { return value; }
	std::string nodeType() { return "Identifier"; }
};

class LetStatement : public Statement
{
public:
	Token token; // token LET
	Identifier name;
	Expression *value;

	void statementNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "Let Statement"; }
};
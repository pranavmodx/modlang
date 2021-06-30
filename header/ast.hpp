#pragma once

#include <string>
#include <vector>
#include <utility>

#include "token.hpp"

class Node
{
public:
	// pure virtual function
	virtual std::string tokenLiteral() = 0;
	virtual std::string getStringRepr() = 0;
	virtual std::string nodeType() = 0;

	virtual ~Node() {}
};

class Statement : public Node
{
public:
	virtual void statementNode() = 0;
	virtual std::string tokenLiteral() = 0;
	virtual std::string getStringRepr() = 0;
	virtual std::string nodeType() = 0;

	virtual ~Statement() {}
};

class Program : public Node
{
public:
	std::vector<Statement *> statements;

	std::string tokenLiteral();
	std::string getStringRepr();
	std::string nodeType() { return "Program"; }

	virtual ~Program() {}
};

class Expression : public Node
{
public:
	virtual void expressionNode() = 0;
	virtual std::string tokenLiteral() = 0;
	virtual std::string getStringRepr() = 0;
	virtual std::string nodeType() = 0;

	virtual ~Expression() {}
};

class Identifier : public Expression
{
public:
	Token token;
	std::string value;

	Identifier() {} // if parameterized constructor (below) is specified then this must be specified too
	Identifier(Token token, std::string value)
	{
		this->token = token;
		this->value = value;
	}

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
	std::string nodeType() { return "LetStatement"; }
};

class ReturnStatement : public Statement
{
public:
	Token token; // token RETURN
	Expression *returnValue;

	void statementNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "ReturnStatement"; }
};

class ExpressionStatement : public Statement
{
public:
	Token token;
	Expression *expression;

	void statementNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "ExpressionStatement"; }
};

class IntegerLiteral : public Expression
{
public:
	Token token;
	int value;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr() { return std::to_string(value); }
	std::string nodeType() { return "IntegerLiteral"; }
};

class PrefixExpression : public Expression
{
public:
	Token token;
	std::string operand;
	Expression *right;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "PrefixExpression"; }
};

class InfixExpression : public Expression
{
public:
	Token token;
	Expression *left;
	std::string operand;
	Expression *right;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "InfixExpression"; }
};

class BooleanLiteral : public Expression
{
public:
	Token token;
	bool value;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr() { return value ? "true" : "false"; }
	std::string nodeType() { return "BooleanLiteral"; }
};

class StringLiteral : public Expression
{
public:
	Token token;
	std::string value;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr() { return value; }
	std::string nodeType() { return "StringLiteral"; }
};

class BlockStatement : public Statement
{
public:
	Token token; // "{"
	std::vector<Statement *> statements;

	void statementNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "BlockStatement"; }
};

class IfExpression : public Expression
{
public:
	Token token; // if
	Expression *condition;
	BlockStatement *consequence;
	BlockStatement *alternative;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "IfExpression"; }
};

class WhileExpression : public Expression
{
public:
	Token token;
	Expression *condition;
	BlockStatement *consequence;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "WhileExpression"; }

};

class FunctionLiteral : public Expression
{
public:
	Token token;
	std::vector<Identifier *> parameters;
	BlockStatement *body;

	void expressionNode() {}
	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "FunctionLiteral"; }
};

class CallExpression : public Expression
{
public:
	Token token;
	Expression *function;
	std::vector<Expression *> arguments;

	void expressionNode() {}

	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "CallExpression"; }
};

class ArrayLiteral : public Expression
{
public:
	Token token;
	std::vector<Expression *> elements;

	~ArrayLiteral()
	{
		for (auto elem : elements)
			delete elem;
	}

	void expressionNode() {}

	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "ArrayLiteral"; }
};

class IndexExpression : public Expression
{
public:
	Token token;
	Expression *array;
	Expression *index;

	void expressionNode() {}

	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "IndexExpression"; }
};

struct HashMapPair
{
	Expression *key;
	Expression *value;
};

class HashMapLiteral : public Expression
{
public:
	Token token;
	std::vector<HashMapPair> pairs;

	void expressionNode() {}

	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "HashMapLiteral"; }
};

class HashSetLiteral : public Expression
{
public:
	Token token;
	std::vector<Expression *> pairs;

	void expressionNode() {}

	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "HashSetLiteral"; }
};

class StackLiteral : public Expression
{
public:
	Token token;
	std::vector<Expression *> elements;

	void expressionNode() {}

	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "StackLiteral"; }
};

class QueueLiteral : public Expression
{
public:
	Token token;
	std::vector<Expression *> elements;

	void expressionNode() {}

	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "QueueLiteral"; }
};

class DequeLiteral : public Expression
{
public:
	Token token;
	std::vector<Expression *> elements;

	void expressionNode() {}

	std::string tokenLiteral() { return token.literal; }
	std::string getStringRepr();
	std::string nodeType() { return "DequeLiteral"; }
};
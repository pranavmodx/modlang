#include "../header/ast.hpp"

#include <iostream>

std::string Program::tokenLiteral()
{
	if (statements.size())
		return statements[0]->tokenLiteral();

	return "";
}

std::string Program::getStringRepr()
{
	std::string res;

	for (auto stmt : statements)
		res += stmt->getStringRepr() + "\n";

	return res;
}

std::string LetStatement::getStringRepr()
{
	std::string res = tokenLiteral() + " " + name.getStringRepr() + " = " + value->getStringRepr();

	res.push_back(';');

	return res;
}

std::string ReturnStatement::getStringRepr()
{
	std::string res = tokenLiteral() + " " + returnValue->getStringRepr();

	res.push_back(';');

	return res;
}

std::string ExpressionStatement::getStringRepr()
{
	std::string res = "";

	if (expression != nullptr)
	{
		res += expression->getStringRepr();
	}

	res.push_back(';');

	return res;
}

std::string PrefixExpression::getStringRepr()
{
	std::string res = "(" + operand + right->getStringRepr() + ")";

	return res;
}

std::string InfixExpression::getStringRepr()
{
	std::string res = "(" + left->getStringRepr() + operand + right->getStringRepr() + ")";

	return res;
}

std::string BlockStatement::getStringRepr()
{
	std::string res = "{ ";
	for (auto stmt : statements)
	{
		res += stmt->getStringRepr() + " ";
	}
	res += "}";

	return res;
}

std::string IfExpression::getStringRepr()
{
	std::string res = "if " + std::string("(") + condition->getStringRepr() + ")" + " " + consequence->getStringRepr();
	if (alternative != nullptr)
		res += " else " + alternative->getStringRepr();

	return res;
}

std::string FunctionLiteral::getStringRepr()
{
	std::string res = tokenLiteral() + "(";
	for (auto param : parameters)
		res += param->getStringRepr() + ",";

	res.pop_back();
	res.pop_back();

	res.push_back(')');
	res += body->getStringRepr();

	return res;
}

std::string CallExpression::getStringRepr()
{
	std::string res = function->getStringRepr() + "(";

	for (auto arg : arguments)
		res += arg->getStringRepr() + ",";

	res.pop_back();
	res.push_back(')');

	return res;
}

std::string ArrayLiteral::getStringRepr()
{
	std::string res = "[";

	for (auto elem : elements)
		res += elem->getStringRepr() + ", ";

	if (res.length() != 1)
	{
		res.pop_back();
		res.pop_back();
	}

	res.push_back(']');

	return res;
}

std::string IndexExpression::getStringRepr()
{
	return "(" + array->getStringRepr() + "[" + index->getStringRepr() + "])";
}

std::string HashLiteral::getStringRepr()
{
	std::string res = "{";

	for (auto pair : pairs)
		res += pair.key->getStringRepr() + " : " + pair.value->getStringRepr() + ", ";

	if (res.length() != 1)
	{
		res.pop_back();
		res.pop_back();
	}

	res.push_back('}');

	return res;
}
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
	std::string res = tokenLiteral() + " " + name.getStringRepr() + " = ";

	res.push_back(';');

	return res;
}

std::string ReturnStatement::getStringRepr() {
  std::string res = tokenLiteral() + " ";

  res.push_back(';');

  return res;
}
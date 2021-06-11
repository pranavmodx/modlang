#include "../header/evaluator.hpp"

Object *Evaluator::Eval(Node *node)
{
	std::string nodeType = node->nodeType();

	// Statements
	if (nodeType == "Program")
		return evalStatements(((Program *)node)->statements);
	else if (nodeType == "ExpressionStatement")
		return Eval(((ExpressionStatement *)node)->expression);

	// Expressions
	else if (nodeType == "IntegerLiteral")
	{
		Integer *integer = new Integer(((IntegerLiteral *)node)->value);
		return integer;
	}

	return nullptr;
}

Object* Evaluator::evalStatements(std::vector<Statement*> statements) {
	Object *result;

	for (Statement* stmt: statements)
		result = Eval(stmt);

	return result;
}
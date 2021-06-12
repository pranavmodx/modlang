#pragma once

#include "../header/ast.hpp"
#include "../header/object.hpp"

class Evaluator
{
private:
	Object* evalProgram(Program* program);
	Object* evalBlockStatement(BlockStatement *blockStmt);

	Object *evalPrefixExpression(std::string operand, Object *right);
	Object *evalBangPrefixExpression(Object *right);
	Object *evalMinusPrefixExpression(Object *right);

	Object *evalInfixExpression(std::string operand, Object *left, Object *right);
	Object *evalIntegerInfixExpression(std::string operand, Object *left, Object *right);

	Object *evalIfExpression(IfExpression *ifExpr);

public:
	Object *Eval(Node *node);
};
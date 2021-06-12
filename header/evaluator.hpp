#pragma once

#include "../header/ast.hpp"
#include "../header/object.hpp"
#include "../header/environment.hpp"

class Evaluator
{
private:
	Object *evalProgram(Program *program, Environment *env);
	Object *evalBlockStatement(BlockStatement *blockStmt, Environment *env);

	Object *evalPrefixExpression(std::string operand, Object *right);
	Object *evalBangPrefixExpression(Object *right);
	Object *evalMinusPrefixExpression(Object *right);

	Object *evalInfixExpression(std::string operand, Object *left, Object *right);
	Object *evalIntegerInfixExpression(std::string operand, Object *left, Object *right);

	Object *evalIfExpression(IfExpression *ifExpr, Environment *env);
	Object *evalIdentifier(Identifier *ident, Environment *env);

public:
	Object *Eval(Node *node, Environment *env);
};
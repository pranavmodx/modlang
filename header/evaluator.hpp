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
	Object *evalStringInfixExpression(std::string operand, Object *left, Object *right);

	Object *evalIfExpression(IfExpression *ifExpr, Environment *env);
	Object *evalIdentifier(Identifier *ident, Environment *env);
	Object *evalCallExpression(Object *fn, std::vector<Object *> &args);

	Object *evalIndexExpression(Object *left, Object *index, Environment *env);
	Object *evalStringIndexExpression(String *string, Integer *index);
	Object *evalArrayIndexExpression(Array *array, Integer *index);
	Object *evalHashIndexExpression(HashMap *hashMap, Object *index);

	Object *evalHashLiteral(HashLiteral *hashLiteral, Environment *env);

	Environment *extendFunctionEnv(Function *fn, std::vector<Object *> &args, Environment *outer);

public:
	Object *Eval(Node *node, Environment *env);
};
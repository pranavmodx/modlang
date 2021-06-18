#include "../header/evaluator.hpp"

bool isError(Object *obj)
{
	if (obj != nullptr)
		return obj->type() == ERROR_OBJ;

	return false;
}

bool isTruthy(Object *condition)
{
	if (condition == __TRUE)
		return true;
	else if (condition == __FALSE)
		return false;
	else if (condition == __NULL)
		return false;
	else if (condition->type() == INTEGER_OBJ && ((Integer *)condition)->value == 0)
		return false;
	else
		return true;
}

Object *Evaluator::Eval(Node *node, Environment *env)
{
	std::string nodeType = node->nodeType();

	// Statements
	if (nodeType == "Program")
		return evalProgram(((Program *)node), env);

	else if (nodeType == "ExpressionStatement")
		return Eval(((ExpressionStatement *)node)->expression, env);

	else if (nodeType == "BlockStatement")
		return evalBlockStatement(((BlockStatement *)node), env);

	else if (nodeType == "ReturnStatement")
	{
		Object *value = Eval(((ReturnStatement *)node)->returnValue, env);
		if (isError(value))
			return value;

		ReturnValue *returnValue = new ReturnValue(value);
		return returnValue;
	}

	else if (nodeType == "LetStatement")
	{
		Object *value = Eval(((LetStatement *)node)->value, env);
		if (isError(value))
			return value;

		env->Set((((LetStatement *)node)->name).value, value);
	}

	// Expressions
	else if (nodeType == "IntegerLiteral")
	{
		Integer *integer = new Integer(((IntegerLiteral *)node)->value);
		return integer;
	}

	else if (nodeType == "BooleanLiteral")
	{
		return ((BooleanLiteral *)node)->value ? __TRUE : __FALSE;
	}

	else if (nodeType == "StringLiteral")
	{
		String *str = new String(((StringLiteral *)node)->value);
		return str;
	}

	else if (nodeType == "PrefixExpression")
	{
		Object *right = Eval(((PrefixExpression *)node)->right, env);
		if (isError(right))
			return right;

		return evalPrefixExpression(((PrefixExpression *)node)->operand, right);
	}

	else if (nodeType == "InfixExpression")
	{
		Object *left = Eval(((InfixExpression *)node)->left, env);
		if (isError(left))
			return left;

		Object *right = Eval(((InfixExpression *)node)->right, env);
		if (isError(right))
			return right;

		return evalInfixExpression(((InfixExpression *)node)->operand, left, right);
	}

	else if (nodeType == "IfExpression")
		return evalIfExpression((IfExpression *)node, env);

	else if (nodeType == "Identifier")
		return evalIdentifier((Identifier *)node, env);

	else if (nodeType == "FunctionLiteral")
	{
		Object *fn = new Function(((FunctionLiteral *)node)->parameters, ((FunctionLiteral *)node)->body, env);

		return fn;
	}

	else if (nodeType == "CallExpression")
	{
		Object *fn = Eval(((CallExpression *)node)->function, env);

		if (isError(fn))
			return fn;

		std::vector<Object *> args;

		for (auto *argument : ((CallExpression *)node)->arguments)
		{
			Object *arg = Eval(argument, env);

			if (isError(arg))
				return arg;

			args.push_back(arg);
		}

		return evalCallExpression(fn, args);
	}

	else if (nodeType == "ArrayLiteral")
	{
		std::vector<Object *> elems;

		for (auto *element : ((ArrayLiteral *)node)->elements)
		{
			Object *elem = Eval(element, env);

			if (isError(elem))
				return elem;

			elems.push_back(elem);
		}

		return new Array(elems);
	}

	else if (nodeType == "IndexExpression")
	{
		Object *array = Eval(((IndexExpression *)node)->array, env);

		if (isError(array))
			return array;

		env->Set(std::to_string((intptr_t)array), array);

		Object *index = Eval(((IndexExpression *)node)->index, env);
		env->store.erase(std::to_string((intptr_t)array));

		if (isError(index))
			return index;

		return evalIndexExpression(array, index, env);
	}

	return __NULL;
}

Object *Evaluator::evalProgram(Program *program, Environment *env)
{
	Object *result;

	for (Statement *stmt : program->statements)
	{
		result = Eval(stmt, env);

		if (result->type() == RETURN_VALUE_OBJ || result->type() == ERROR_OBJ)
			return ((ReturnValue *)result)->value;

		else if (result->type() == ERROR_OBJ)
			return result;
	}

	return result;
}

Object *Evaluator::evalBlockStatement(BlockStatement *blockStmt, Environment *env)
{
	Object *result;

	for (Statement *stmt : blockStmt->statements)
	{
		result = Eval(stmt, env);
		if (result->type() == "RETURN_VALUE" || result->type() == "ERROR")
			return (ReturnValue *)result;
	}

	return result;
}

Object *Evaluator::evalBangPrefixExpression(Object *right)
{
	if (right == __TRUE)
		return __FALSE;

	else if (right == __FALSE)
		return __TRUE;
	else if (right == __NULL)
		return __TRUE;

	else if (right->type() == INTEGER_OBJ)
	{
		if (((Integer *)right)->value == 0)
			return __TRUE;
		else
			return __FALSE;
	}

	return __FALSE;
}

Object *Evaluator::evalMinusPrefixExpression(Object *right)
{
	if (right->type() == INTEGER_OBJ)
	{
		Integer *intObj = new Integer(-((Integer *)right)->value);
		return intObj;
	}

	Object *error = new Error("unknown operator: -" + right->type());
	return error;
}

Object *Evaluator::evalPrefixExpression(std::string operand, Object *right)
{
	if (operand == "!")
		return evalBangPrefixExpression(right);
	else if (operand == "-")
		return evalMinusPrefixExpression(right);

	Object *error = new Error("unknown operator: " + operand + right->type());
	return error;
}

Object *Evaluator::evalInfixExpression(std::string operand, Object *left, Object *right)
{
	Object *error;

	if (left->type() == INTEGER_OBJ && right->type() == INTEGER_OBJ)
		return evalIntegerInfixExpression(operand, left, right);

	else if (left->type() == STRING_OBJ && right->type() == STRING_OBJ)
		return evalStringInfixExpression(operand, left, right);

	else if (left == __NULL || right == __NULL)
		return __NULL;

	else if (left->type() != right->type())
	{
		error = new Error("type mismatch: " + left->type() + " " + operand + " " + right->type());
		return error;
	}

	else if (operand == "==")
		return left == right ? __TRUE : __FALSE;
	else if (operand == "!=")
		return left != right ? __TRUE : __FALSE;

	else
	{
		error = new Error("unknown operator: " + left->type() + " " + operand + " " + right->type());

		return error;
	}
}

Object *Evaluator::evalIntegerInfixExpression(std::string operand, Object *left, Object *right)
{
	int leftVal = ((Integer *)left)->value;
	int rightVal = ((Integer *)right)->value;

	Object *res;

	if (operand == "+")
		res = new Integer(leftVal + rightVal);
	else if (operand == "-")
		res = new Integer(leftVal - rightVal);
	else if (operand == "*")
		res = new Integer(leftVal * rightVal);
	else if (operand == "/" && rightVal != 0)
		res = new Integer(leftVal / rightVal);
	else if (operand == "%" && rightVal != 0)
		res = new Integer(leftVal % rightVal);

	else if (operand == "==")
		return leftVal == rightVal ? __TRUE : __FALSE;
	else if (operand == "!=")
		return leftVal != rightVal ? __TRUE : __FALSE;
	else if (operand == ">")
		return leftVal > rightVal ? __TRUE : __FALSE;
	else if (operand == "<")
		return leftVal < rightVal ? __TRUE : __FALSE;
	else if (operand == ">=")
		return leftVal >= rightVal ? __TRUE : __FALSE;
	else if (operand == "<=")
		return leftVal <= rightVal ? __TRUE : __FALSE;

	else
	{
		Object *error = new Error("unknown operator: " + left->type() + " " + operand + " " + right->type());
		return error;
	}

	return res;
}

Object *Evaluator::evalStringInfixExpression(std::string operand, Object *left, Object *right)
{
	std::string leftVal = ((String *)left)->value;
	std::string rightVal = ((String *)right)->value;

	Object *res;

	if (operand == "+")
		res = new String(leftVal + rightVal);
	else
		return new Error("unknown operator: " + left->type() + " " + operand + " " + right->type());

	return res;
}

Object *Evaluator::evalIfExpression(IfExpression *ifExpr, Environment *env)
{
	Object *condition = Eval(ifExpr->condition, env);

	if (isTruthy(condition))
		return Eval(ifExpr->consequence, env);

	else if (ifExpr->alternative != nullptr)
		return Eval(ifExpr->alternative, env);

	return __NULL;
}

Object *Evaluator::evalIdentifier(Identifier *ident, Environment *env)
{
	Object *obj = env->Get(ident->value);
	return obj;
}

Object *Evaluator::evalCallExpression(Object *fn, std::vector<Object *> &args)
{
	if (fn->type() != FUNCTION_OBJ)
		return new Error("not a function: " + fn->type());

	if (((Function *)fn)->parameters.size() != args.size())
		return new Error(
			"argument length(" + std::to_string(args.size()) +
			") not equal to parameter length (" + std::to_string(((Function *)fn)->parameters.size()) + ")");

	Environment *extendedEnv = extendFunctionEnv((Function *)fn, args, ((Function *)fn)->env);

	Object *evaluated = Eval(((Function *)fn)->body, extendedEnv);

	if (evaluated->type() == RETURN_VALUE_OBJ)
		return ((ReturnValue *)evaluated)->value;

	return evaluated;
}

Environment *Evaluator::extendFunctionEnv(Function *fn, std::vector<Object *> &args, Environment *outer)
{
	Environment *env = outer->NewEnclosed();

	for (int i = 0; i < args.size(); i++)
		env->Set(fn->parameters[i]->value, args[i]);

	return env;
}

Object *Evaluator::evalIndexExpression(Object *array, Object *index, Environment *env)
{
	if (array->type() == ARRAY_OBJ && index->type() == INTEGER_OBJ)
		return evalArrayIndexExpression((Array *)array, (Integer *)index);

	if (array->type() == STRING_OBJ && index->type() == INTEGER_OBJ)
		return evalStringIndexExpression((String *)array, (Integer *)index);

	else
		return new Error("index operator not supported: " + array->type());
}

Object *Evaluator::evalArrayIndexExpression(Array *array, Integer *index)
{
	int i = index->value;
	auto arr = array->elements;

	try
	{
		return arr[i];
	}
	catch (const std::out_of_range e)
	{
		return new Error("index " + index->inspect() + " out of range");
	}
}

Object *Evaluator::evalStringIndexExpression(String *array, Integer *index)
{
	int i = index->value;
	auto arr = array->value;

	try
	{
		return new String(std::string(1, arr[i]));
	}
	catch (const std::out_of_range e)
	{
		return new Error("index " + index->inspect() + " out of range");
	}
}
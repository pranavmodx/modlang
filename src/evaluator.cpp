#include "../header/builtins.hpp"
#include "../header/evaluator.hpp"

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
#include <iostream>
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

		if (value->type() == ERROR_OBJ)
			return value;

		ReturnValue *returnValue = new ReturnValue(value);
		return returnValue;
	}

	else if (nodeType == "LetStatement")
	{
		Object *value = Eval(((LetStatement *)node)->value, env);

		if (value->type() == ERROR_OBJ)
			return value;

		env->Set((((LetStatement *)node)->name).value, value);
	}

	else if (nodeType == "AssignStatement")
	{
		Object *value = Eval(((AssignStatement *)node)->value, env);

		if (value->type() == ERROR_OBJ)
			return value;

		Object *obj = env->Get((((AssignStatement *)node)->name).value);

		if (obj->type() == ERROR_OBJ)
			return obj;

		env->Set((((AssignStatement *)node)->name).value, value);
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
		if (right->type() == ERROR_OBJ)
			return right;

		return evalPrefixExpression(((PrefixExpression *)node)->operand, right);
	}

	else if (nodeType == "InfixExpression")
	{
		Object *left = Eval(((InfixExpression *)node)->left, env);
		if (left->type() == ERROR_OBJ)
			return left;

		Object *right = Eval(((InfixExpression *)node)->right, env);
		if (right->type() == ERROR_OBJ)
			return right;

		return evalInfixExpression(((InfixExpression *)node)->operand, left, right);
	}

	else if (nodeType == "IfExpression")
		return evalIfExpression((IfExpression *)node, env);

	else if (nodeType == "WhileExpression")
	{
		while (true)
		{
			Object *condition = Eval(((WhileExpression *)node)->condition, env);

			if (condition->type() == ERROR_OBJ)
				return condition;

			if (!isTruthy(condition))
				return __NULL;

			Object *result = Eval(((WhileExpression *)node)->consequence, env);

			if (result->type() == ERROR_OBJ ||
				result->type() == RETURN_VALUE_OBJ)
				return result;
		}
	}

	else if (nodeType == "Identifier")
	{
		return evalIdentifier((Identifier *)node, env);
	}

	else if (nodeType == "FunctionLiteral")
	{
		Object *fn = new Function(((FunctionLiteral *)node)->parameters, ((FunctionLiteral *)node)->body, env);

		return fn;
	}

	else if (nodeType == "CallExpression")
	{
		Object *fn = Eval(((CallExpression *)node)->function, env);

		if (fn->type() == ERROR_OBJ)
			return fn;

		std::vector<Object *> args;

		for (auto *argument : ((CallExpression *)node)->arguments)
		{
			Object *arg = Eval(argument, env);

			if (arg->type() == ERROR_OBJ)
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

			if (elem->type() == ERROR_OBJ)
				return elem;

			elems.push_back(elem);
		}

		return new Array(elems);
	}

	else if (nodeType == "IndexExpression")
	{
		Object *array = Eval(((IndexExpression *)node)->array, env);

		if (array->type() == ERROR_OBJ)
			return array;

		Object *index = Eval(((IndexExpression *)node)->index, env);

		if (index->type() == ERROR_OBJ)
			return index;

		return evalIndexExpression(array, index, env);
	}

	else if (nodeType == "HashMapLiteral")
		return evalHashMapLiteral((HashMapLiteral *)node, env);

	else if (nodeType == "HashSetLiteral")
		return evalHashSetLiteral((HashSetLiteral *)node, env);

	else if (nodeType == "StackLiteral")
		return evalStackLiteral((StackLiteral *)node, env);

	else if (nodeType == "QueueLiteral")
		return evalQueueLiteral((QueueLiteral *)node, env);

	else if (nodeType == "DequeLiteral")
		return evalDequeLiteral((DequeLiteral *)node, env);

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

	Object *error = new Error("error : unknown operator for " + right->type() + " -> -");
	return error;
}

Object *Evaluator::evalPrefixExpression(std::string operand, Object *right)
{
	if (operand == "!")
		return evalBangPrefixExpression(right);
	else if (operand == "-")
		return evalMinusPrefixExpression(right);

	Object *error = new Error("error : unknown operator for " + right->type() + " -> " + operand);
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
		error = new Error("error: type mismatch -> " + left->type() + " " + operand + " " + right->type());
		return error;
	}

	else if (operand == "==")
		return left == right ? __TRUE : __FALSE;
	else if (operand == "!=")
		return left != right ? __TRUE : __FALSE;

	else
	{
		error = new Error("error: unknown operator -> " + left->type() + " " + operand + " " + right->type());

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
		Object *error = new Error("error: unknown operator -> " + left->type() + " " + operand + " " + right->type());
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
		return new Error("error: unknown operator -> " + left->type() + " " + operand + " " + right->type());

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

	if (obj->type() != ERROR_OBJ &&
		builtin.find(ident->value) != builtin.end())
		return builtin[ident->value];

	return obj;
}

Object *Evaluator::evalCallExpression(Object *fn, std::vector<Object *> &args)
{
	if (fn->type() != FUNCTION_OBJ && fn->type() != BUILTIN_OBJ)
		return new Error("error: not a function -> " + fn->type());

	if (fn->type() == BUILTIN_OBJ)
		return ((Builtin *)fn)->function(args);

	if (((Function *)fn)->parameters.size() != args.size())
		return new Error(
			"error: argument length (" + std::to_string(args.size()) +
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

Object *Evaluator::evalIndexExpression(Object *left, Object *index, Environment *env)
{
	if (left->type() == STRING_OBJ && index->type() == INTEGER_OBJ)
		return evalStringIndexExpression((String *)left, (Integer *)index);

	else if (left->type() == ARRAY_OBJ && index->type() == INTEGER_OBJ)
		return evalArrayIndexExpression((Array *)left, (Integer *)index);

	else if (left->type() == HASHMAP_OBJ)
		return evalHashMapIndexExpression((HashMap *)left, index);

	else
		return new Error("error: index operator [] not supported for -> " + left->type());
}

Object *Evaluator::evalArrayIndexExpression(Array *array, Integer *index)
{
	int i = index->value;
	auto arr = array->elements;

	if (arr.size() <= i)
		return new Error("error: index " + index->inspect() + " out of range");

	return arr[i];
}

Object *Evaluator::evalStringIndexExpression(String *string, Integer *index)
{
	int i = index->value;
	auto arr = string->value;

	if (arr.size() <= i)
		return new Error("error: index " + index->inspect() + " out of range");

	return new String(std::string(1, arr[i]));
}

Object *Evaluator::evalHashMapLiteral(HashMapLiteral *hashMapLiteral, Environment *env)
{
	HashMap *hashMap = new HashMap();

	for (auto pair : hashMapLiteral->pairs)
	{
		Object *key = Eval(pair.key, env);

		if (key->type() == ERROR_OBJ)
			return key;

		Object *value = Eval(pair.value, env);

		if (value->type() == ERROR_OBJ)
		{
			delete hashMap;
			return value;
		}

		HashKey hashKey(key->type(), key->inspect());
		HashMapPairObj hashMapPairObj(key, value);

		hashMap->pairs[hashKey] = hashMapPairObj;
	}

	return hashMap;
}

Object *Evaluator::evalHashMapIndexExpression(HashMap *hashMap, Object *index)
{
	HashKey hashKey(index->type(), index->inspect());

	if (hashMap->pairs.find(hashKey) != hashMap->pairs.end())
		return hashMap->pairs[hashKey].value;

	return new Error("error: key not present -> " + index->inspect());
}

Object *Evaluator::evalHashSetLiteral(HashSetLiteral *hashSetLiteral, Environment *env)
{
	HashSet *hashSet = new HashSet();

	for (auto pair : hashSetLiteral->pairs)
	{
		Object *key = Eval(pair, env);

		if (key->type() == ERROR_OBJ)
		{
			delete hashSet;
			return key;
		}

		HashKey hashKey(key->type(), key->inspect());
		hashSet->pairs[hashKey] = key;
	}

	return hashSet;
}

Object *Evaluator::evalStackLiteral(StackLiteral *stackLiteral, Environment *env)
{
	Stack *stack = new Stack();

	for (auto elem : stackLiteral->elements)
	{
		Object *obj = Eval(elem, env);

		if (obj->type() == ERROR_OBJ)
		{
			delete stack;
			return obj;
		}

		stack->elements.push(obj);
	}

	return stack;
}

Object *Evaluator::evalQueueLiteral(QueueLiteral *queueLiteral, Environment *env)
{
	Queue *queue = new Queue();

	for (auto elem : queueLiteral->elements)
	{
		Object *obj = Eval(elem, env);

		if (obj->type() == ERROR_OBJ)
		{
			delete queue;
			return obj;
		}

		queue->elements.push(obj);
	}

	return queue;
}

Object *Evaluator::evalDequeLiteral(DequeLiteral *queueLiteral, Environment *env)
{
	Deque *deque = new Deque();

	for (auto elem : queueLiteral->elements)
	{
		Object *obj = Eval(elem, env);

		if (obj->type() == ERROR_OBJ)
		{
			delete deque;
			return obj;
		}

		deque->elements.push_back(obj);
	}

	return deque;
}
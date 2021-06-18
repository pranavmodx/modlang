#pragma once

#include "ast.hpp"

typedef std::string ObjectType;

const ObjectType INTEGER_OBJ = "INTEGER";
const ObjectType BOOLEAN_OBJ = "BOOLEAN";
const ObjectType STRING_OBJ = "STRING";
const ObjectType NULL_OBJ = "NULL";
const ObjectType RETURN_VALUE_OBJ = "RETURN_VALUE";
const ObjectType ERROR_OBJ = "ERROR";
const ObjectType FUNCTION_OBJ = "FUNCTION";
const ObjectType ARRAY_OBJ = "ARRAY";
const ObjectType BUILTIN_OBJ = "BUILTIN";

class Object
{
public:
	virtual ObjectType type() = 0;
	virtual std::string inspect() = 0;
};

class Integer : public Object
{
public:
	int value;

	Integer(int v) : Object(), value(v) {}
	ObjectType type() { return INTEGER_OBJ; }
	std::string inspect() { return std::to_string(value); }
};

class Boolean : public Object
{
public:
	bool value;

	Boolean(bool b) : Object(), value(b) {}
	ObjectType type() { return BOOLEAN_OBJ; }
	std::string inspect() { return value ? "true" : "false"; }
};

class String : public Object
{
public:
	std::string value;

	String(std::string s) : Object(), value(s) {}
	ObjectType type() { return STRING_OBJ; }
	std::string inspect() { return value; }
};

class Null : public Object
{
public:
	ObjectType type() { return NULL_OBJ; }
	std::string inspect() { return "NULL"; }
};

class ReturnValue : public Object
{
public:
	Object *value;

	ReturnValue(Object *v) : Object(), value(v) {}
	ObjectType type() { return RETURN_VALUE_OBJ; }
	std::string inspect() { return value->inspect(); }
};

class Error : public Object
{
public:
	std::string message;

	Error(std::string msg) : Object(), message(msg) {}
	ObjectType type() { return ERROR_OBJ; }
	std::string inspect() { return "ERROR: " + message; }
};

class Environment;

class Function : public Object
{
public:
	std::vector<Identifier *> parameters;
	BlockStatement *body;
	Environment *env;

	Function(std::vector<Identifier *> &params, BlockStatement *b, Environment *e) : Object(), parameters(params), body(b), env(e) {}

	~Function()
	{
		for (auto param : parameters)
			delete param;

		delete body;
	}

	ObjectType type() { return FUNCTION_OBJ; }

	std::string inspect()
	{
		std::string res = "def (";
		for (auto ident : parameters)
			res += ident->getStringRepr() + ", ";

		res.pop_back();
		res.pop_back();

		res += ")" + std::string("{\n") + body->getStringRepr() + std::string("\n}");

		return res;
	}
};

class Array : public Object
{
public:
	Array(std::vector<Object *> &elems) : Object(), elements(elems) {}

	ObjectType type() { return ARRAY_OBJ; }

	std::string inspect()
	{
		std::string res = "[";
		for (auto elem : elements)
			res += elem->inspect() + ", ";
	
		res.pop_back();
		res.pop_back();

		res.push_back(']');

		return res;
	}

	std::vector<Object *> elements;
};

class Builtin : public Object
{
public:
	Object *(*function)(std::vector<Object *> &);

	Builtin(Object *(*fn)(std::vector<Object *> &)) : Object(), function(fn) {}
	ObjectType type() { return BUILTIN_OBJ; }
	std::string inspect() { return "Builtin Function"; }
};

extern Null *__NULL;
extern Boolean *__TRUE;
extern Boolean *__FALSE;
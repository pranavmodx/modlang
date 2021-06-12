#pragma once

#include "ast.hpp"

typedef std::string ObjectType;

const ObjectType INTEGER_OBJ = "INTEGER";
const ObjectType BOOLEAN_OBJ = "BOOLEAN";
const ObjectType NULL_OBJ = "NULL";
const ObjectType RETURN_VALUE_OBJ = "RETURN_VALUE";
const ObjectType ERROR_OBJ = "ERROR";

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

extern Null *__NULL;
extern Boolean *__TRUE;
extern Boolean *__FALSE;
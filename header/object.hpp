#pragma once

#include <functional>
#include <unordered_set>

#include "ast.hpp"

typedef std::string ObjectType;

const ObjectType INTEGER_OBJ = "INTEGER";
const ObjectType BOOLEAN_OBJ = "BOOLEAN";
const ObjectType STRING_OBJ = "STRING";
const ObjectType RETURN_VALUE_OBJ = "RETURN_VALUE";
const ObjectType BUILTIN_OBJ = "BUILTIN";
const ObjectType FUNCTION_OBJ = "FUNCTION";
const ObjectType ARRAY_OBJ = "ARRAY";
const ObjectType HASHMAP_OBJ = "HASHMAP";
const ObjectType HASHSET_OBJ = "HASHSET";

const ObjectType NULL_OBJ = "NULL";
const ObjectType ERROR_OBJ = "ERROR";

class Object
{
public:
	virtual ObjectType type() = 0;
	virtual std::string inspect() = 0;
};

// std::unordered_set<ObjectType> Hashable{INTEGER_OBJ, BOOLEAN_OBJ,STRING_OBJ};

struct HashKey
{
	ObjectType type;
	std::string value;

	HashKey() {}
	HashKey(ObjectType type, std::string value) : type{type}, value{value} {}

	bool operator==(const HashKey &otherKey) const
	{
		return otherKey.type == type && otherKey.value == value;
	}
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

	String(std::string s) : Object(), value{s} {}
	ObjectType type() { return STRING_OBJ; }
	std::string inspect() { return value; }
};

class Error : public Object
{
public:
	std::string value;

	Error(std::string s) : Object(), value(s) {}
	ObjectType type() { return ERROR_OBJ + " "; } // some bug here, don't touch " "!
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

class Builtin : public Object
{
public:
	Object *(*function)(std::vector<Object *> &);

	Builtin(Object *(*fn)(std::vector<Object *> &)) : Object(), function(fn) {}
	ObjectType type() { return BUILTIN_OBJ; }
	std::string inspect() { return "Builtin Function"; }
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
	std::vector<Object *> elements;

	Array(std::vector<Object *> &elems) : Object(), elements(elems) {}

	ObjectType type() { return ARRAY_OBJ; }

	std::string inspect()
	{
		std::string res = "[";

		for (auto elem : elements)
			res += elem->inspect() + ", ";

		if (res.size() != 1)
		{
			res.pop_back();
			res.pop_back();
		}

		res.push_back(']');

		return res;
	}
};

class HashMapPairObj
{ // we could use std::pair but this is more explicit
public:
	Object *key;
	Object *value;

	HashMapPairObj() {}
	HashMapPairObj(Object *key, Object *value) : key{key}, value{value} {}
};

struct HashFn
{
	size_t operator()(const HashKey &hashKey) const
	{
		return std::hash<std::string>()(hashKey.type) ^
			   std::hash<std::string>()(hashKey.value);
	}
};

class HashMap : public Object
{
public:
	std::unordered_map<HashKey, HashMapPairObj, HashFn> pairs;

	ObjectType type() { return HASHMAP_OBJ; }

	std::string inspect()
	{
		std::string res = "{";

		for (auto pair : pairs)
			res += pair.second.key->inspect() + " : " + pair.second.value->inspect() + ", ";

		if (res.length() != 1)
		{
			res.pop_back();
			res.pop_back();
		}

		res.push_back('}');

		return res;
	}
};

class HashSet : public Object
{
public:
	std::unordered_map<HashKey, Object*, HashFn> pairs;

	ObjectType type() { return HASHSET_OBJ; }

	std::string inspect()
	{
		std::string res = "{";

		for (auto pair : pairs)
			res += pair.second->inspect() + ", ";

		if (res.length() != 1)
		{
			res.pop_back();
			res.pop_back();
		}

		res.push_back('}');

		return res;
	}
};

extern Null *__NULL;
extern Boolean *__TRUE;
extern Boolean *__FALSE;
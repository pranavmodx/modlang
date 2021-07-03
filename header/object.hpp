#pragma once

#include <functional>
#include <unordered_set>
#include <stack>
#include <queue>
#include <deque>

#include "ast.hpp"

typedef std::string ObjectType;

const ObjectType INTEGER_OBJ = "INTEGER";
const ObjectType BOOLEAN_OBJ = "BOOLEAN";
const ObjectType STRING_OBJ = "STRING";

const ObjectType FUNCTION_OBJ = "FUNCTION";

const ObjectType ARRAY_OBJ = "ARRAY";
const ObjectType HASHMAP_OBJ = "HASHMAP";
const ObjectType HASHSET_OBJ = "HASHSET";
const ObjectType STACK_OBJ = "STACK";
const ObjectType QUEUE_OBJ = "QUEUE";
const ObjectType DEQUE_OBJ = "DEQUE";
const ObjectType MAXHEAP_OBJ = "MAXHEAP";
const ObjectType MINHEAP_OBJ = "MINHEAP";

const ObjectType RETURN_VALUE_OBJ = "RETURN_VALUE";
const ObjectType BUILTIN_OBJ = "BUILTIN";
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
	ObjectType type() { return ERROR_OBJ + " "; }
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

		if (parameters.size() != 0)
		{
			res.pop_back();
			res.pop_back();
		}

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

		if (elements.size() != 0)
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

		if (pairs.size() != 0)
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
	std::unordered_map<HashKey, Object *, HashFn> pairs;

	ObjectType type() { return HASHSET_OBJ; }

	std::string inspect()
	{
		std::string res = "hashset<> {";

		for (auto pair : pairs)
			res += pair.second->inspect() + ", ";

		if (pairs.size() != 0)
		{
			res.pop_back();
			res.pop_back();
		}

		res.push_back('}');

		return res;
	}
};

class Stack : public Object
{
public:
	std::stack<Object *> elements;

	ObjectType type() { return STACK_OBJ; }

	std::string inspect()
	{
		if (elements.empty())
			return "stack<> {top: empty}";

		std::string res = "stack<> {top: " + elements.top()->inspect() + "}";

		return res;
	}
};

class Queue : public Object
{
public:
	std::queue<Object *> elements;

	ObjectType type() { return QUEUE_OBJ; }

	std::string inspect()
	{
		if (elements.empty())
			return "queue<> {front: empty}";

		std::string res = "queue<> {front: " + elements.front()->inspect() + "}";

		return res;
	}
};

class Deque : public Object
{
public:
	std::deque<Object *> elements;

	ObjectType type() { return DEQUE_OBJ; }

	std::string inspect()
	{
		if (elements.empty())
			return "deque<> {front: empty, back: empty}";

		std::string res = "deque<> {front: " + elements.front()->inspect() + ", back: " + elements.back()->inspect() + "}";

		return res;
	}
};

struct MaxHeapCompareFn
{
	bool operator()(Object *obj1, Object *obj2)
	{
		if (obj1->type() == INTEGER_OBJ)
			return ((Integer *)obj1)->value < ((Integer *)obj2)->value;
		else if (obj1->type() == STRING_OBJ)
			return ((String *)obj1)->value < ((String *)obj2)->value;
		
		return true;
	}
};

class MaxHeap : public Object
{
public:
	TokenType tokenType;
	std::priority_queue<Object *, std::vector<Object *>, MaxHeapCompareFn> elements;

	MaxHeap() {}
	MaxHeap(TokenType tType) : tokenType{tType} {}

	ObjectType type() { return MAXHEAP_OBJ; }

	std::string inspect()
	{
		TokenType tType = tokenType.substr(0, 3);
		transform(tType.begin(), tType.end(), tType.begin(), ::tolower);

		if (elements.empty())
			return "max_heap <" + tType + "> {top: empty}";

		std::string res = "max_heap <" + tType + "> {top: " + elements.top()->inspect() + "}";

		return res;
	}
};

struct MinHeapCompareFn
{
	bool operator()(Object *obj1, Object *obj2)
	{
		if (obj1->type() == INTEGER_OBJ)
			return ((Integer *)obj1)->value > ((Integer *)obj2)->value;
		else
			return ((String *)obj1)->value > ((String *)obj2)->value;
	}
};

class MinHeap : public Object
{
public:
	TokenType tokenType;
	std::priority_queue<Object *, std::vector<Object *>, MinHeapCompareFn> elements;

	MinHeap() {}
	MinHeap(TokenType tType) : tokenType{tType} {}

	ObjectType type() { return MINHEAP_OBJ; }

	std::string inspect()
	{
		TokenType tType = tokenType.substr(0, 3);
		transform(tType.begin(), tType.end(), tType.begin(), ::tolower);

		if (elements.empty())
			return "min_heap <" + tType + "> {top: empty}";

		std::string res = "min_heap <" + tType + "> {top: " + elements.top()->inspect() + "}";

		return res;
	}
};

extern Null *__NULL;
extern Boolean *__TRUE;
extern Boolean *__FALSE;
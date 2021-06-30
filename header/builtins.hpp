#pragma once

#include <iostream>

#include "./object.hpp"

Object *Print(std::vector<Object *> &objs)
{
	for (auto obj : objs)
	{
		if (obj->type() == ERROR_OBJ)
			return obj;

		std::cout << obj->inspect() << " ";
	}

	std::cout << std::endl;

	return __NULL;
}

Object *Len(std::vector<Object *> &objs)
{
	if (objs.size() != 1)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (1)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == STRING_OBJ)
		return new Integer(((String *)obj)->value.size());

	else if (type == ARRAY_OBJ)
		return new Integer(((Array *)obj)->elements.size());

	else if (type == HASHMAP_OBJ)
		return new Integer(((HashMap *)obj)->pairs.size());

	else if (type == HASHSET_OBJ)
		return new Integer(((HashSet *)obj)->pairs.size());

	return new Error("error: unsupported object for len()");
}

Object *Push(std::vector<Object *> &objs)
{
	if (objs.size() != 2)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)\n");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	if (objs[1]->type() == ERROR_OBJ)
		return objs[1];

	else if (type == STRING_OBJ)
		((String *)obj)->value += ((String *)objs[1])->value;

	else if (type == ARRAY_OBJ)
		((Array *)obj)->elements.push_back(((Integer *)objs[1]));

	else if (type == STACK_OBJ)
		((Stack *)obj)->elements.push(((Integer *)objs[1]));

	else if (type == QUEUE_OBJ)
		((Queue *)obj)->elements.push(((Integer *)objs[1]));

	else if (type == DEQUE_OBJ)
		((Deque *)obj)->elements.push_back(((Integer *)objs[1]));

	return new Error("error: unsupported object for push()");
}

Object *Pop(std::vector<Object *> &objs)
{
	if (objs.size() != 0)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (1)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == STRING_OBJ)
	{
		if (((String *)obj)->value.empty())
			return new Error("error: cannot pop from empty string");
		((String *)obj)->value.pop_back();
	}

	else if (type == ARRAY_OBJ)
	{
		if (((Array *)obj)->elements.empty())
			return new Error("error: cannot pop from empty array");

		((Array *)obj)->elements.pop_back();
	}

	else if (type == STACK_OBJ)
	{
		if (((Stack *)obj)->elements.empty())
			return new Error("error: cannot pop from empty stack");

		((Stack *)obj)->elements.pop();
	}

	else if (type == QUEUE_OBJ)
	{
		if (((Queue *)obj)->elements.empty())
			return new Error("error: cannot pop from empty queue");

		((Queue *)obj)->elements.pop();
	}

	else if (type == DEQUE_OBJ)
	{
		if (((Deque *)obj)->elements.empty())
			return new Error("error: cannot pop from empty deque");

		((Deque *)obj)->elements.pop_back();
	}

	return new Error("error: unsupported object for pop()");
}

Object *Insert(std::vector<Object *> &objs)
{
	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == HASHSET_OBJ)
	{
		if (objs.size() != 2)
			return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2) for hashset");

		if (objs[1]->type() == ERROR_OBJ)
			return objs[1];

		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		((HashSet *)obj)->pairs.insert({hashKey, objs[1]});
	}

	else if (type == HASHMAP_OBJ)
	{
		if (objs.size() != 3)
			return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (3) for hashmap");

		if (objs[1]->type() == ERROR_OBJ)
			return objs[1];

		if (objs[2]->type() == ERROR_OBJ)
			return objs[2];

		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		HashMapPairObj hashMapPairObj(objs[1], objs[2]);

		((HashMap *)objs[0])->pairs.insert({hashKey, hashMapPairObj});
	}

	return new Error("error: unsupported object for insert()");
}

Object *Remove(std::vector<Object *> &objs)
{
	if (objs.size() != 2)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == HASHSET_OBJ)
	{
		if (((HashSet *)obj)->pairs.empty())
			return new Error("error: cannot remove from empty hashset");

		if (objs[1]->type() == ERROR_OBJ)
			return objs[1];

		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		if (((HashSet *)obj)->pairs.find(hashKey) == ((HashSet *)obj)->pairs.end())
			return new Error("error: key not found in hashset -> " + objs[1]->inspect());
		((HashSet *)obj)->pairs.erase(hashKey);
	}

	else if (type == HASHMAP_OBJ)
	{
		if (((HashMap *)obj)->pairs.empty())
			return new Error("error: cannot remove from empty hashmap");

		if (objs[1]->type() == ERROR_OBJ)
			return objs[1];

		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		if (((HashMap *)obj)->pairs.find(hashKey) == ((HashMap *)obj)->pairs.end())
			return new Error("error: key not found in hashmap -> " + objs[1]->inspect());
		((HashMap *)objs[0])->pairs.erase(hashKey);
	}

	return new Error("error: unsupported object for remove()");
}

Object *Find(std::vector<Object *> &objs)
{
	if (objs.size() != 2)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	if (objs[1]->type() == ERROR_OBJ)
		return objs[1];

	// todo
	if (type == STRING_OBJ) {

	}

	else if (type == ARRAY_OBJ) {

	}

	else if (type == HASHMAP_OBJ) {

	}

	else if (type == HASHSET_OBJ) {

	}

	return __NULL;
}

Object *Type(std::vector<Object *> &objs)
{
	if (objs.size() != 1)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (1)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	std::cout << type << std::endl;

	return __NULL;
}

std::unordered_map<std::string, Builtin *> builtin{
	{"print", new Builtin(Print)},
	{"type", new Builtin(Type)},
	{"len", new Builtin(Len)},
	{"push", new Builtin(Push)},
	{"pop", new Builtin(Pop)},
	{"insert", new Builtin(Insert)},
	{"remove", new Builtin(Remove)},
	{"find", new Builtin(Find)},
};
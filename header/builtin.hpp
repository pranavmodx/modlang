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

	return new Error("error: invalid argument");
}

Object *Push(std::vector<Object *> &objs)
{
	if (objs.size() != 2)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)\n");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == STRING_OBJ)
		((String *)obj)->value += ((String *)objs[1])->value;

	else if (type == ARRAY_OBJ)
		((Array *)obj)->elements.push_back(((Integer *)objs[1]));

	return __NULL;
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
			return new Error("error: cannot pop from empty string");

		((Array *)obj)->elements.pop_back();
	}

	return __NULL;
}

Object *Insert(std::vector<Object *> &objs)
{
	if (objs.size() != 2 || objs.size() != 3)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2 or 3)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == HASHSET_OBJ)
	{
		if (objs.size() != 2)
			return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)");

		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		((HashSet *)obj)->pairs.insert({hashKey, objs[1]});
	}

	else if (type == HASHMAP_OBJ)
	{
		if (objs.size() != 3)
			return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)");

		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		HashMapPairObj hashMapPairObj(objs[1], objs[2]);

		((HashMap *)objs[0])->pairs.insert({hashKey, hashMapPairObj});
	}

	return __NULL;
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

		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		if (((HashSet *)obj)->pairs.find(hashKey) == ((HashSet *)obj)->pairs.end())
			return new Error("error: key not found in hashset -> " + objs[1]->inspect());
		((HashSet *)obj)->pairs.erase(hashKey);
	}

	else if (type == HASHMAP_OBJ)
	{
		if (((HashMap *)obj)->pairs.empty())
			return new Error("error: cannot remove from empty hashmap");

		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		if (((HashMap *)obj)->pairs.find(hashKey) == ((HashMap *)obj)->pairs.end())
			return new Error("error: key not found in hashmap -> " + objs[1]->inspect());
		((HashMap *)objs[0])->pairs.erase(hashKey);
	}

	return __NULL;
}

std::unordered_map<std::string, Builtin *> builtin{
	{"print", new Builtin(Print)},
	{"len", new Builtin(Len)},
	{"push", new Builtin(Push)},
	{"pop", new Builtin(Pop)},
	{"insert", new Builtin(Insert)},
	{"remove", new Builtin(Remove)},
};
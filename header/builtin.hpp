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

	if (!objs.empty())
		std::cout << std::endl;

	return __NULL;
}

Object *Len(std::vector<Object *> &objs)
{
	if (objs.size() == 0)
		return new Error("error: argument length (0) less than min (1)")

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == STRING_OBJ)
		return new Integer(((String *)obj)->value.size());

	else if (type == ARRAY_OBJ)
		return new Integer(((Array *)obj)->elements.size());

	else if (type == HASHMAP_OBJ)
		return new Integer(((HashMap *)obj)->pairs.size());

	else if (type == HASHSET_OBJ)
		return new Integer(((HashSet *)obj)->pairs.size());

	if (!objs.empty())
		std::cout << std::endl;

	return new Error("error: invalid argument");
}

Object *Push(std::vector<Object *> &objs)
{
	if (objs.size() < 2)
		return __NULL;

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == STRING_OBJ)
		((String *)obj)->value += ((String *)objs[1])->value;

	else if (type == ARRAY_OBJ)
		((Array *)obj)->elements.push_back(((Integer *)objs[1]));

	if (!objs.empty())
		std::cout << std::endl;

	return __NULL;
}

Object *Pop(std::vector<Object *> &objs)
{
	if (objs.size() == 0)
		return __NULL;

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == STRING_OBJ)
		((String *)obj)->value.pop_back();

	else if (type == ARRAY_OBJ)
		((Array *)obj)->elements.pop_back();

	if (!objs.empty())
		std::cout << std::endl;

	return __NULL;
}

Object *Insert(std::vector<Object *> &objs)
{
	if (objs.size() == 0)
		return __NULL;

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == HASHSET_OBJ) {
		HashKey hashKey (objs[1]->type(), objs[1]->inspect());
		((HashSet *)obj)->pairs.insert({hashKey, objs[1]});
	}

	if (!objs.empty())
		std::cout << std::endl;

	return __NULL;
}

Object *Remove(std::vector<Object *> &objs)
{
	if (objs.size() == 0)
		return __NULL;

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == HASHSET_OBJ) {
		HashKey hashKey (objs[1]->type(), objs[1]->inspect());
		((HashSet *)obj)->pairs.erase(hashKey);
	}

	if (!objs.empty())
		std::cout << std::endl;

	return __NULL;
}

std::unordered_map<std::string, Builtin *> builtin{
	{"print", new Builtin(*Print)},
	{"len", new Builtin(*Len)},
	{"push", new Builtin(*Push)},
	{"pop", new Builtin(*Pop)},
	{"insert", new Builtin(*Insert)},
	{"remove", new Builtin(*Remove)},};
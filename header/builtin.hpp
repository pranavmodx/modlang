#pragma once

#include <iostream>

#include "./object.hpp"

Object *print(std::vector<Object *> &objs)
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

Object *len(std::vector<Object *> &objs)
{
	if (objs.size() == 0)
		return __NULL;

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == STRING_OBJ)
		return new Integer(((String *)obj)->value.size());

	else if (type == ARRAY_OBJ)
		return new Integer(((Array *)obj)->elements.size());

	std::cout << std::endl;

	return __NULL;
}

Object *push(std::vector<Object *> &objs)
{
	if (objs.size() < 2)
		return __NULL;

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == STRING_OBJ)
		((String *)obj)->value.push_back(objs[1]);

	else if (type == ARRAY_OBJ)
		((Array *)obj)->elements.push_back(objs[1]);

	return __NULL;
}

Object *pop(std::vector<Object *> &objs)
{
	if (objs.size() == 0)
		return __NULL;

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == STRING_OBJ)
		(String *)obj->value.pop_back();

	else if (type == ARRAY_OBJ)
		(Array *)obj->elements.pop_back();

	return __NULL;
}

std::unordered_map<std::string, Builtin *> builtin{
	{"print", new Builtin(*print)},
	{"len", new Builtin(*len)},
	{"push", new Builtin(*push)},
	{"pop", new Builtin(*pop)}};
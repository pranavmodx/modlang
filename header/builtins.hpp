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

	return new Error("error: unsupported object for len()");
}

Object *Size(std::vector<Object *> &objs)
{
	if (objs.size() != 1)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (1)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == HASHSET_OBJ)
		return new Integer(((HashSet *)obj)->pairs.size());

	else if (type == STACK_OBJ)
		return new Integer(((Stack *)obj)->elements.size());

	else if (type == QUEUE_OBJ)
		return new Integer(((Queue *)obj)->elements.size());

	else if (type == DEQUE_OBJ)
		return new Integer(((Deque *)obj)->elements.size());

	else if (type == MAXHEAP_OBJ)
		return new Integer(((MaxHeap *)obj)->elements.size());

	else if (type == MINHEAP_OBJ)
		return new Integer(((MinHeap *)obj)->elements.size());

	return new Error("error: unsupported object for len()");
}

Object *Push(std::vector<Object *> &objs)
{
	if (objs.size() != 2)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)\n");

	Object *obj = objs[0];
	const ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	if (objs[1]->type() == ERROR_OBJ)
		return objs[1];

	else if (type == STRING_OBJ)
	{
		if (objs[1]->type() != STRING_OBJ)
			return new Error("error: expected " + STRING_OBJ + " got " + objs[1]->type());

		((String *)obj)->value += ((String *)objs[1])->value;
		return __NULL;
	}

	else if (type == ARRAY_OBJ)
	{
		((Array *)obj)->elements.push_back(objs[1]);
		return __NULL;
	}

	else if (type == STACK_OBJ)
	{
		((Stack *)obj)->elements.push(objs[1]);
		return __NULL;
	}

	else if (type == QUEUE_OBJ)
	{
		((Queue *)obj)->elements.push(objs[1]);
		return __NULL;
	}

	else if (type == MAXHEAP_OBJ)
	{
		if (objs[1]->type() != ((MaxHeap *)obj)->tokenType)
			return new Error("error: expected " + ((MaxHeap *)obj)->tokenType + " got " + objs[1]->type());

		((MaxHeap *)obj)->elements.push(objs[1]);
		return __NULL;
	}

	else if (type == MINHEAP_OBJ)
	{
		((MinHeap *)obj)->elements.push(objs[1]);
		return __NULL;
	}

	return new Error("error: unsupported object for push()");
}

Object *Push_Front(std::vector<Object *> &objs)
{
	if (objs.size() != 2)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)\n");

	Object *obj = objs[0];
	const ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	if (objs[1]->type() == ERROR_OBJ)
		return objs[1];

	else if (type == DEQUE_OBJ)
	{
		((Deque *)obj)->elements.push_front(objs[1]);
		return __NULL;
	}

	return new Error("error: unsupported object for push()");
}
Object *Push_Back(std::vector<Object *> &objs)
{
	if (objs.size() != 2)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (2)\n");

	Object *obj = objs[0];
	const ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	if (objs[1]->type() == ERROR_OBJ)
		return objs[1];

	else if (type == DEQUE_OBJ)
	{
		((Stack *)obj)->elements.push(objs[1]);
		return __NULL;
	}

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
		return __NULL;
	}

	else if (type == ARRAY_OBJ)
	{
		if (((Array *)obj)->elements.empty())
			return new Error("error: cannot pop from empty array");

		((Array *)obj)->elements.pop_back();
		return __NULL;
	}

	else if (type == STACK_OBJ)
	{
		if (((Stack *)obj)->elements.empty())
			return new Error("error: cannot pop from empty stack");

		((Stack *)obj)->elements.pop();
		return __NULL;
	}

	else if (type == QUEUE_OBJ)
	{
		if (((Queue *)obj)->elements.empty())
			return new Error("error: cannot pop from empty queue");

		((Queue *)obj)->elements.pop();
		return __NULL;
	}

	else if (type == MAXHEAP_OBJ)
	{
		if (((MaxHeap *)obj)->elements.empty())
			return new Error("error: cannot pop from empty heap");

		((MaxHeap *)obj)->elements.pop();
		return __NULL;
	}

	else if (type == MINHEAP_OBJ)
	{
		if (((MinHeap *)obj)->elements.empty())
			return new Error("error: cannot pop from empty heap");

		((MinHeap *)obj)->elements.pop();
		return __NULL;
	}

	return new Error("error: unsupported object for pop()");
}

Object *Pop_Front(std::vector<Object *> &objs)
{
	if (objs.size() != 0)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (1)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == DEQUE_OBJ)
	{
		if (((Deque *)obj)->elements.empty())
			return new Error("error: cannot pop from empty deque");

		((Deque *)obj)->elements.pop_front();
		return __NULL;
	}

	return new Error("error: unsupported object for pop_front()");
}

Object *Pop_Back(std::vector<Object *> &objs)
{
	if (objs.size() != 0)
		return new Error("error: argument length (" + std::to_string(objs.size()) + ") not equal to parameter length (1)");

	Object *obj = objs[0];
	ObjectType type = obj->type();

	if (type == ERROR_OBJ)
		return obj;

	else if (type == DEQUE_OBJ)
	{
		if (((Deque *)obj)->elements.empty())
			return new Error("error: cannot pop from empty deque");

		((Deque *)obj)->elements.pop_back();
		return __NULL;
	}

	return new Error("error: unsupported object for pop_back()");
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

		return __NULL;
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

		return __NULL;
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

		return __NULL;
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

		return __NULL;
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

	else if (type == STRING_OBJ)
	{
		if (objs[1]->type() != STRING_OBJ)
			return new Error("error: expected " + STRING_OBJ + " got " + objs[1]->type());

		size_t found = ((String *)obj)->value.find(((String *)objs[1])->value);
		if (found == std::string::npos)
			return new Integer(-1);
		else
			return new Integer((int)found);

		return __NULL;
	}

	else if (type == ARRAY_OBJ)
	{
		auto elems = ((Array *)obj)->elements;

		for (int i = 0; i < elems.size(); i++)
		{
			if (elems[i]->type() == objs[1]->type())
			{
				TokenType tType = elems[i]->type();

				if (tType == INTEGER_OBJ)
				{
					if (((Integer *)elems[i])->value == ((Integer *)objs[1])->value)
						return new Integer(i);
				}

				else if (tType == STRING_OBJ)
				{
					if (((String *)elems[i])->value == ((String *)objs[1])->value)
						return new Integer(i);
				}
			}
		}

		return new Integer(-1);
	}

	else if (type == HASHMAP_OBJ)
	{
		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		auto hmap = ((HashMap *)obj)->pairs;

		if (hmap.find(hashKey) != hmap.end())
			return new Boolean(true);
		return new Boolean(false);
	}

	else if (type == HASHSET_OBJ)
	{
		HashKey hashKey(objs[1]->type(), objs[1]->inspect());
		auto hmap = ((HashSet *)obj)->pairs;

		if (hmap.find(hashKey) != hmap.end())
			return new Boolean(true);
		return new Boolean(false);
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
	{"size", new Builtin(Size)},

	{"push", new Builtin(Push)},
	{"push_front", new Builtin(Push_Front)},
	{"push_back", new Builtin(Push_Back)},

	{"pop", new Builtin(Pop)},
	{"pop_front", new Builtin(Pop_Front)},
	{"pop_back", new Builtin(Pop_Back)},

	{"insert", new Builtin(Insert)},
	{"remove", new Builtin(Remove)},
	{"find", new Builtin(Find)},
};
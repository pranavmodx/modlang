#include "../header/environment.hpp"

Environment *Environment::New()
{
	Environment *env = new Environment();
	return env;
}

Environment *Environment::NewEnclosed()
{
	Environment* inner = new Environment();
    inner->outer = this;

    return inner;
}

Object *Environment::Get(std::string name)
{
	if (store.find(name) == store.end())
	{
		if (outer != nullptr)
		{
			Object *obj = outer->Get(name);
			return obj;
		}

		return new Error("identifier not found: " + name);
	}
	return store[name];
}

Object *Environment::Set(std::string name, Object *val)
{
	store[name] = val;
	return val;
}
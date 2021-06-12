#include "../header/environment.hpp"

Environment *Environment::New()
{
	Environment *env = new Environment();
	return env;
}

Object *Environment::Get(std::string name)
{
	if (store.find(name) != store.end())
		return store[name];

	Error *error = new Error("identifier not found: " + name);
	return error;
}

Object *Environment::Set(std::string name, Object *val)
{
	store[name] = val;
	return val;
}
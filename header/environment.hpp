#pragma once

#include <unordered_map>
#include <string>

#include "./object.hpp"

class Environment {
public:
	std::unordered_map<std::string, Object*> store;
	Environment *outer;
	
	Environment* New();
	Environment* NewEnclosed();
	Object* Get(std::string name);
	Object* Set(std::string name, Object* val);
};
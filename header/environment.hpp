#pragma once

#include <unordered_map>
#include <string>

#include "./object.hpp"

class Environment {
private:
	std::unordered_map<std::string, Object*> store;

public:
	Environment* New();
	Object* Get(std::string name);
	Object* Set(std::string name, Object* val);
};
#pragma once

#include "../header/ast.hpp"
#include "../header/object.hpp"

class Evaluator
{
public:
	Object *Eval(Node *node);

private:
	Object *evalStatements(std::vector<Statement*> statements);
};
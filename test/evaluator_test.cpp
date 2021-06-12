#include <iostream>

#include "../header/lexer.hpp"
#include "../header/parser.hpp"
#include "../header/evaluator.hpp"
#include "../header/environment.hpp"

void TestEvalIntegerExpression();
Object *testEval(std::string input);
void testIntegerObject(Object *obj, int expected);

int main()
{
	TestEvalIntegerExpression();
}

void TestEvalIntegerExpression()
{
	std::vector<std::pair<std::string, int>> tests = {
		{"5", 5},
		{"10", 10},
	};

	for (auto test : tests)
	{
		Object *obj = testEval(test.first);
		testIntegerObject(obj, test.second);
	}
}

Object *testEval(std::string input)
{
	Lexer lexer;
	lexer.New(input);

	Parser parser;
	parser.New(lexer);

	Program *program = parser.ParseProgram();

	Evaluator evaluator;
	Environment *env = new Environment();

	Object *obj = evaluator.Eval(program, env);
	return obj;
}

void testIntegerObject(Object *obj, int expected)
{
	if (obj->type() != "INTEGER")
		std::cout << "object is not Integer, got=" << obj->type() << std::endl;

	Integer *intObj = (Integer *)obj;

	std::cout << intObj->value << std::endl;

	if (intObj->value != expected)
		std::cout << "object has wrong value, got=" << intObj->value << " want=" << expected << std::endl;
}
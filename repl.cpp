#include <iostream>
#include <string>

#include "./header/lexer.hpp"
#include "./header/parser.hpp"
#include "./header/evaluator.hpp"

// Read Evaluate Print Loop
void repl()
{
	const std::string PROMPT = ">> ";

	Lexer lexer;
	Parser parser;
	Evaluator evaluator;

	std::string line;

	while (true)
	{
		std::cout << PROMPT;

		std::getline(std::cin, line);

		lexer.New(line);
		parser.New(lexer);

		Program *program = parser.ParseProgram();

		if (parser.Errors().size())
			for (std::string error : parser.Errors())
				std::cout << error << std::endl;
		else {
			Object *obj = evaluator.Eval(program);
			if (obj)
				std::cout << obj->inspect() << std::endl;
		}

		parser.resetErrors();
	}
}

int main()
{
	repl();

	return 0;
}
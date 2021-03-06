#include <iostream>
#include <string>

#include "./header/lexer.hpp"
#include "./header/parser.hpp"
#include "./header/evaluator.hpp"

// Read Evaluate Print Loop
void REPL()
{
	const std::string PROMPT = ">> ";

	Lexer lexer;
	Parser parser;
	Evaluator evaluator;

	Environment *env = new Environment();

	std::string line;

	while (true)
	{
		std::cout << PROMPT;

		std::getline(std::cin, line);

		if (line.empty())
			continue;

		lexer.New(line);
		parser.New(lexer);

		Program *program = parser.ParseProgram();

		if (parser.Errors().size())
			for (std::string error : parser.Errors())
				std::cout << error << std::endl;

		else {
			Object *obj = evaluator.Eval(program, env);
			if (obj != __NULL)
				std::cout << obj->inspect() << std::endl;
		}

		parser.resetErrors();
	}
}

int main()
{
	REPL();

	return 0;
}
#include <iostream>
#include <string>

#include "./header/parser.hpp"

// Read "Parse" Print Loop
void RPPL()
{
	const std::string PROMPT = ">> ";

	Lexer lexer;
	Parser parser;
	
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
		else
			std::cout << program->getStringRepr() << std::endl;

		parser.resetErrors();
	}
}

int main()
{
	RPPL();

	return 0;
}
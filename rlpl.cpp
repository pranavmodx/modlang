#include "./header/lexer.hpp"

#include <iostream>
#include <string>


// Read "Lex" Print Loop
void rlpl()
{
	const std::string PROMPT = ">> ";

	Token token;
	Lexer lexer;
	
	std::string line;

	while (true)
	{
		std::cout << PROMPT;

		std::getline(std::cin, line);

		lexer.New(line);
		token = lexer.nextToken();

		while (token.type != END)
		{
			std::cout << "Type: " << token.type << ", Literal: " << token.literal << std::endl;
			token = lexer.nextToken();
		}

		line.clear();
	}
}

int main()
{
	rlpl();
	
	return 0;
}
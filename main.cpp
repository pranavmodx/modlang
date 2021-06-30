#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

#include "./header/lexer.hpp"
#include "./header/parser.hpp"
#include "./header/evaluator.hpp"

std::string readFile(const std::string &fileName)
{
	std::ifstream fin(fileName.c_str(), std::ios::binary | std::ios::ate);

	if (!fin) {
		std::cerr << "error: could not open the file" << std::endl;
		return "";
	}

	std::ifstream::pos_type fileSize = fin.tellg();
	fin.seekg(0, std::ios::beg);

	std::vector<char> bytes(fileSize);
	fin.read(bytes.data(), fileSize);
	fin.close();

	return std::string(bytes.data(), fileSize);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cout << "error: file name not specified" << std::endl;
		exit(1);
	}

	std::string filename(argv[1]);
	std::string input = readFile(filename);

	if (input.empty())
		return 0;

	Lexer lexer;
	Parser parser;
	Evaluator evaluator;

	Environment *env = new Environment();

	lexer.New(input);
	parser.New(lexer);

	Program *program = parser.ParseProgram();

	if (!parser.Errors().empty())
	{
		std::cout << "error: " << std::endl;

		for (auto error : parser.Errors())
			std::cout << error << std::endl;

		return 0;
	}

	Object *obj = evaluator.Eval(program, env);
	if (obj->type() != NULL_OBJ)
		std::cout << obj->inspect() << std::endl;

	return 0;
}
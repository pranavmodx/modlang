#include "../header/parser.hpp"

#include <iostream>


bool testLetStatement(LetStatement *stmt, std::string expectedName);
void checkParserErrors(Parser &parser);
void TestLetStatement();

int main() {
	TestLetStatement();
}

void TestLetStatement()
{
	std::string input = 
	"let x = 5;"
	"let y = 10;"
	"let foobar = 838383;";

	// struct LetTest {
	// 	std::string input;
	// 	std::string expectedIdent;
	// 	Expression *value;
	// };

	// std::vector<LetTest> tests {
	// 	{"let x = 5;", "x", 5},
	// 	{"let y = true;", "y", true},
	// 	{"let foobar = y;", "foobar", "y"},
	// }

	Lexer lexer;
	lexer.New(input);

	Parser parser;
	parser.New(lexer);

	Program *program = parser.ParseProgram();

	checkParserErrors(parser);

	if (program == nullptr)
		std::cout << "ParseProgram() returned null" << std::endl;

	if (program->statements.size() != 3)
		std::cout << "program.statements does not contain 3 statements, got " << program->statements.size() << std::endl;

	std::vector<std::string> expectedTestIdent { "x", "y", "foobar" };

	Statement *stmt;
	for (int i=0; i<program->statements.size(); i++) {
		stmt = program->statements[i];

		if (!testLetStatement((LetStatement*) stmt, expectedTestIdent[i]))
			return;
	}
}

bool testLetStatement(LetStatement *stmt, std::string expectedName) {
	if (stmt->tokenLiteral() != "let") {
		std::cout <<  "stmt.tokenLiteral() not 'let'" << ", got " << stmt->tokenLiteral() << std::endl;
		return false;
	}

	if (stmt->name.value != expectedName) {
		std::cout << "stmt->name.value not " << expectedName << ", got " << stmt->name.value << std::endl;
		return false;
	}

	if (stmt->name.tokenLiteral() != expectedName) {
		std::cout << "stmt->name.tokenLiteral() not " << expectedName << ", got " << stmt->name.tokenLiteral() << std::endl;
		return false;
	}

	return true;
}

void checkParserErrors(Parser &parser) {
	std::vector<std::string> errors = parser.Errors();

	if (errors.empty())
		return;

	else {
		std::cout << "parser has " << errors.size() << " errors" << std::endl;
		for (std::string error: errors)
			std::cout << "parser error : " << error << std::endl;

		exit(0);
	}
}
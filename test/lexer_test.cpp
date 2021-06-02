#include "../header/lexer.hpp"

#include <iostream>
#include <vector>
#include <utility>


void testNextToken();

int main() {
	testNextToken();
}

void testNextToken() {
	std::string input = 
	"let five = 5; "
	"let ten = 10; "

	"let add = def(x, y) { "
	"x + y; "
	"}; "
	"let result = add(five, ten); "

	"!-/*5; "
	"5 < 10 > 5; "

	"if (5 < 10) { "
		"return true; "
	"} else { "
		"return false; "
	"} "

	"10 == 10; "
	"10 != 9; "

	"\"foobar\" "
	"\"foo bar\" "

	"[1, 2]; "
	"{\"foo\": \"bar\"} ";

	std::vector<std::pair<TokenType, std::string>> tests;
	tests.reserve(100);
	
	tests = {
		{LET, "let"},
		{IDENT, "five"},
		{ASSIGN, "="},
		{INT, "5"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "ten"},
		{ASSIGN, "="},
		{INT, "10"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "add"},
		{ASSIGN, "="},
		{FUNCTION, "def"},
		{LPAREN, "("},
		{IDENT, "x"},
		{COMMA, ","},
		{IDENT, "y"},
		{RPAREN, ")"},
		{LBRACE, "{"},
		{IDENT, "x"},
		{PLUS, "+"},
		{IDENT, "y"},
		{SEMICOLON, ";"},
		{RBRACE, "}"},
		{SEMICOLON, ";"},
		{LET, "let"},
		{IDENT, "result"},
		{ASSIGN, "="},
		{IDENT, "add"},
		{LPAREN, "("},
		{IDENT, "five"},
		{COMMA, ","},
		{IDENT, "ten"},
		{RPAREN, ")"},
		{SEMICOLON, ";"},
		{BANG, "!"},
		{MINUS, "-"},
		{SLASH, "/"},
		{ASTERISK, "*"},
		{INT, "5"},
		{SEMICOLON, ";"},
		{INT, "5"},
		{LT, "<"},
		{INT, "10"},
		{GT, ">"},
		{INT, "5"},
		{SEMICOLON, ";"},
		{IF, "if"},
		{LPAREN, "("},
		{INT, "5"},
		{LT, "<"},
		{INT, "10"},
		{RPAREN, ")"},
		{LBRACE, "{"},
		{RETURN, "return"},
		{TRUE, "true"},
		{SEMICOLON, ";"},
		{RBRACE, "}"},
		{ELSE, "else"},
		{LBRACE, "{"},
		{RETURN, "return"},
		{FALSE, "false"},
		{SEMICOLON, ";"},
		{RBRACE, "}"},
		{INT, "10"},
		{EQ, "=="},
		{INT, "10"},
		{SEMICOLON, ";"},
		{INT, "10"},
		{NEQ, "!="},
		{INT, "9"},
		{SEMICOLON, ";"},
		{STRING, "foobar"},
		{STRING, "foo bar"},
		{LBRACKET, "["},
		{INT, "1"},
		{COMMA, ","},
		{INT, "2"},
		{RBRACKET, "]"},
		{SEMICOLON, ";"},
		{LBRACE, "{"},
		{STRING, "foo"},
		{COLON, ":"},
		{STRING, "bar"},
		{RBRACE, "}"},
		{END, ""}
	};

	Lexer lexer;
	lexer.New(input);

	Token token;
	for (int i=0; i<tests.size(); i++) {
		token = lexer.nextToken();

		if (token.type != tests[i].first)
			std::cout << i << "expected " << tests[i].first << ", got " << token.type << std::endl;

		if (token.literal != tests[i].second)
			std::cout << i << "expected " << tests[i].second << ", got " << token.literal << std::endl;
	}
}


# generates all the executables
all: rlpl rppl lexer_test parser_test


# links individual obj files
rlpl: rlpl.o token.o lexer.o
	g++ -std=c++17 -o rlpl rlpl.o token.o lexer.o

rppl: rppl.o token.o lexer.o ast.o parser.o
	g++ -std=c++17 -o rppl rppl.o token.o lexer.o ast.o parser.o

lexer_test: lexer_test.o token.o lexer.o
	g++ -std=c++17 -o lexer_test lexer_test.o token.o lexer.o

parser_test: parser_test.o token.o lexer.o ast.o parser.o
	g++ -std=c++17 -o parser_test parser_test.o token.o lexer.o ast.o parser.o


# specifies individual obj's file dependencies and recipe (command)
rlpl.o: rlpl.cpp header/lexer.hpp
	g++ -std=c++17 -c rlpl.cpp

rppl.o: rppl.cpp header/lexer.hpp header/parser.hpp
	g++ -std=c++17 -c rppl.cpp

lexer_test.o: test/lexer_test.cpp
	g++ -std=c++17 -c test/lexer_test.cpp

parser_test.o: test/parser_test.cpp
	g++ -std=c++17 -c test/parser_test.cpp

token.o: src/token.cpp header/token.hpp
	g++ -std=c++17 -c src/token.cpp

lexer.o: src/lexer.cpp header/lexer.hpp
	g++ -std=c++17 -c src/lexer.cpp

ast.o: src/ast.cpp header/ast.hpp
	g++ -std=c++17 -c src/ast.cpp

parser.o: src/parser.cpp header/parser.hpp
	g++ -std=c++17 -c src/parser.cpp


# removes all the files created by previous 'make' command
clean:
	rm -rf ./*.o ./*.out ./rlpl ./rppl ./lexer_test ./parser_test
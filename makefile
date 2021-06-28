# generates all the executables
all: rlpl rppl repl lexer_test parser_test evaluator_test


# links individual obj files
rlpl: rlpl.o token.o lexer.o
	g++ -std=c++17 -o rlpl rlpl.o token.o lexer.o

rppl: rppl.o token.o lexer.o ast.o parser.o
	g++ -std=c++17 -o rppl rppl.o token.o lexer.o ast.o parser.o

repl: repl.o token.o lexer.o ast.o parser.o object.o environment.o evaluator.o
	g++ -std=c++17 -o repl repl.o token.o lexer.o ast.o parser.o object.o environment.o evaluator.o

lexer_test: lexer_test.o token.o lexer.o
	g++ -std=c++17 -o lexer_test lexer_test.o token.o lexer.o

parser_test: parser_test.o token.o lexer.o ast.o parser.o
	g++ -std=c++17 -o parser_test parser_test.o token.o lexer.o ast.o parser.o

evaluator_test: evaluator_test.o token.o lexer.o ast.o parser.o object.o  environment.o evaluator.o
	g++ -std=c++17 -o evaluator_test evaluator_test.o token.o lexer.o ast.o parser.o object.o environment.o evaluator.o


# specifies individual obj's file dependencies and recipe (command)

# shell
rlpl.o: rlpl.cpp header/lexer.hpp header/token.hpp
	g++ -std=c++17 -c rlpl.cpp

rppl.o: rppl.cpp header/lexer.hpp header/parser.hpp header/token.hpp header/ast.hpp
	g++ -std=c++17 -c rppl.cpp

repl.o: repl.cpp header/lexer.hpp header/parser.hpp header/evaluator.hpp header/token.hpp header/ast.hpp header/builtin.hpp header/object.hpp header/environment.hpp
	g++ -std=c++17 -c repl.cpp

# src files
token.o: src/token.cpp header/token.hpp
	g++ -std=c++17 -c src/token.cpp

lexer.o: src/lexer.cpp header/lexer.hpp header/token.hpp
	g++ -std=c++17 -c src/lexer.cpp

ast.o: src/ast.cpp header/ast.hpp header/token.hpp
	g++ -std=c++17 -c src/ast.cpp

parser.o: src/parser.cpp header/parser.hpp header/token.hpp header/lexer.hpp header/ast.hpp
	g++ -std=c++17 -c src/parser.cpp

object.o: src/object.cpp header/object.hpp header/ast.hpp
	g++ -std=c++17 -c src/object.cpp

environment.o: src/environment.cpp header/environment.hpp header/object.hpp
	g++ -std=c++17 -c src/environment.cpp

evaluator.o: src/evaluator.cpp header/evaluator.hpp header/builtin.hpp header/ast.hpp header/object.hpp header/environment.hpp
	g++ -std=c++17 -c src/evaluator.cpp


# test files
lexer_test.o: test/lexer_test.cpp
	g++ -std=c++17 -c test/lexer_test.cpp

parser_test.o: test/parser_test.cpp
	g++ -std=c++17 -c test/parser_test.cpp

evaluator_test.o: test/evaluator_test.cpp
	g++ -std=c++17 -c test/evaluator_test.cpp


# removes all the files created by previous 'make' command
clean:
	rm -rf ./*.o ./*.out ./rlpl ./rppl ./repl ./lexer_test ./parser_test ./evaluator_test
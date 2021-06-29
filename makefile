CXX=g++
CXXFLAGS=-std=c++11

# generates all the executables
all: mod rlpl rppl repl lexer_test parser_test evaluator_test


# links individual obj files
mod: main.o token.o lexer.o ast.o parser.o object.o environment.o evaluator.o
	$(CXX) $(CXXFLAGS) -o mod main.o token.o lexer.o ast.o parser.o object.o environment.o evaluator.o

repl: repl.o token.o lexer.o ast.o parser.o object.o environment.o evaluator.o
	$(CXX) $(CXXFLAGS) -o repl repl.o token.o lexer.o ast.o parser.o object.o environment.o evaluator.o

rppl: rppl.o token.o lexer.o ast.o parser.o
	$(CXX) $(CXXFLAGS) -o rppl rppl.o token.o lexer.o ast.o parser.o

rlpl: rlpl.o token.o lexer.o
	$(CXX) $(CXXFLAGS) -o rlpl rlpl.o token.o lexer.o

lexer_test: lexer_test.o token.o lexer.o
	$(CXX) $(CXXFLAGS) -o lexer_test lexer_test.o token.o lexer.o

parser_test: parser_test.o token.o lexer.o ast.o parser.o
	$(CXX) $(CXXFLAGS) -o parser_test parser_test.o token.o lexer.o ast.o parser.o

evaluator_test: evaluator_test.o token.o lexer.o ast.o parser.o object.o  environment.o evaluator.o
	$(CXX) $(CXXFLAGS) -o evaluator_test evaluator_test.o token.o lexer.o ast.o parser.o object.o environment.o evaluator.o


# specifies individual obj's file dependencies and recipe (command)

# main
main.o: main.cpp header/lexer.hpp header/parser.hpp header/evaluator.hpp header/token.hpp header/ast.hpp header/builtins.hpp header/object.hpp header/environment.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# shell
rlpl.o: rlpl.cpp header/lexer.hpp header/token.hpp
	$(CXX) $(CXXFLAGS) -c rlpl.cpp

rppl.o: rppl.cpp header/lexer.hpp header/parser.hpp header/token.hpp header/ast.hpp
	$(CXX) $(CXXFLAGS) -c rppl.cpp

repl.o: repl.cpp header/lexer.hpp header/parser.hpp header/evaluator.hpp header/token.hpp header/ast.hpp header/builtins.hpp header/object.hpp header/environment.hpp
	$(CXX) $(CXXFLAGS) -c repl.cpp

# src files
token.o: src/token.cpp header/token.hpp
	$(CXX) $(CXXFLAGS) -c src/token.cpp

lexer.o: src/lexer.cpp header/lexer.hpp header/token.hpp
	$(CXX) $(CXXFLAGS) -c src/lexer.cpp

ast.o: src/ast.cpp header/ast.hpp header/token.hpp
	$(CXX) $(CXXFLAGS) -c src/ast.cpp

parser.o: src/parser.cpp header/parser.hpp header/token.hpp header/lexer.hpp header/ast.hpp
	$(CXX) $(CXXFLAGS) -c src/parser.cpp

object.o: src/object.cpp header/object.hpp header/ast.hpp
	$(CXX) $(CXXFLAGS) -c src/object.cpp

environment.o: src/environment.cpp header/environment.hpp header/object.hpp
	$(CXX) $(CXXFLAGS) -c src/environment.cpp

evaluator.o: src/evaluator.cpp header/evaluator.hpp header/builtins.hpp header/ast.hpp header/object.hpp header/environment.hpp
	$(CXX) $(CXXFLAGS) -c src/evaluator.cpp


# test files
lexer_test.o: test/lexer_test.cpp
	$(CXX) $(CXXFLAGS) -c test/lexer_test.cpp

parser_test.o: test/parser_test.cpp
	$(CXX) $(CXXFLAGS) -c test/parser_test.cpp

evaluator_test.o: test/evaluator_test.cpp
	$(CXX) $(CXXFLAGS) -c test/evaluator_test.cpp


# removes all the files created by previous 'make' command
clean:
	rm -rf ./*.o ./*.out ./rlpl ./rppl ./repl ./lexer_test ./parser_test ./evaluator_test
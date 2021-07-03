#pragma once

#include <string>
#include <unordered_map>

typedef std::string TokenType;

class Token
{
public:
	TokenType type;
	std::string literal;

	Token() {}
	Token(TokenType type, std::string literal) : type(type), literal(literal) {}
	// check
	Token(TokenType type, char ch) : type(type), literal(std::string(1, ch)) {}
};

const TokenType ILLEGAL = "ILLEGAL"; // unknown token
const TokenType END = "END"; // end of file

const TokenType IDENT = "IDENT"; // identifiers
const TokenType INTEGER = "INTEGER"; // int data type
const TokenType STRING = "STRING"; // string data type

// operators
const TokenType ASSIGN = "=";
const TokenType PLUS = "+";
const TokenType MINUS = "-";
const TokenType BANG = "!";
const TokenType ASTERISK = "*";
const TokenType SLASH = "/";
const TokenType MODULO = "%";

// comparison
const TokenType LT = "<";
const TokenType GT = ">";

const TokenType LTEQ = "<=";
const TokenType GTEQ = ">=";

const TokenType EQ = "==";
const TokenType NEQ = "!=";

// punctuations
const TokenType COMMA = ",";
const TokenType SEMICOLON = ";";
const TokenType COLON = ":";

// brackets
const TokenType LPAREN = "(";
const TokenType RPAREN = ")";
const TokenType LBRACE = "{";
const TokenType RBRACE = "}";
const TokenType LBRACKET = "[";
const TokenType RBRACKET = "]";

// keywords
const TokenType TRUE = "TRUE";
const TokenType FALSE = "FALSE";
const TokenType LET = "LET";
const TokenType IF = "IF";
const TokenType ELSE = "ELSE";
const TokenType WHILE = "WHILE";
const TokenType FUNCTION = "FUNCTION";
const TokenType RETURN = "RETURN";
const TokenType HASHSET = "HASHSET";
const TokenType STACK = "STACK";
const TokenType QUEUE = "QUEUE";
const TokenType DEQUE = "DEQUE";
const TokenType MAX_HEAP = "MAX_HEAP";
const TokenType MIN_HEAP = "MIN_HEAP";

extern std::unordered_map<std::string, TokenType> keywords;

TokenType LookupIdentifier(std::string ident);
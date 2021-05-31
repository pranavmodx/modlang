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
  	Token(TokenType type, char ch) : type(type), literal(std::string(1, ch)) {}
};

const TokenType ILLEGAL = "ILLEGAL"; // unknown token
const TokenType END = "END"; // end of file

const TokenType IDENT = "IDENT"; // identifiers
const TokenType INT = "INT";
const TokenType STRING = "STRING";

const TokenType ASSIGN = "=";
const TokenType PLUS = "+";
const TokenType MINUS = "-";
const TokenType BANG = "!";
const TokenType ASTERISK = "*";
const TokenType SLASH = "/";
const TokenType MODULO = "%";

const TokenType LT = "<";
const TokenType GT = ">";
const TokenType LTE = "<=";
const TokenType GTE = ">=";

const TokenType EQ = "==";
const TokenType NEQ = "!=";

const TokenType COMMA = ",";
const TokenType SEMICOLON = ";";

const TokenType LPAREN = "(";
const TokenType RPAREN = ")";
const TokenType LBRACE = "{";
const TokenType RBRACE = "}";
const TokenType LBRACKET = "[";
const TokenType RBRACKET = "]";

// keywords
const TokenType FUNCTION = "FUNCTION";
const TokenType LET = "LET";
const TokenType TRUE = "TRUE";
const TokenType FALSE = "FALSE";
const TokenType IF = "IF";
const TokenType ELSE = "ELSE";
const TokenType RETURN = "RETURN";

extern std::unordered_map<std::string, TokenType> keywords;

TokenType LookupIdentifier(std::string ident);
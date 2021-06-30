#include "../header/token.hpp"

std::unordered_map<std::string, TokenType> keywords = {
    {"true", TRUE},
    {"false", FALSE},
    {"let", LET},
    {"if", IF},
    {"else", ELSE},
    {"while", WHILE},
    {"def", FUNCTION},
    {"return", RETURN},
    {"hashset", HASHSET},
    {"stack", STACK},
    {"queue", QUEUE},
    {"deque", DEQUE},
};

TokenType LookupIdentifier(std::string ident)
{
  if (keywords.find(ident) == keywords.end())
    return IDENT;
  return keywords[ident];
}

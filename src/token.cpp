#include "../header/token.hpp"

std::unordered_map<std::string, TokenType> keywords = {
    {"let", LET},
    {"def", FUNCTION},
    {"true", TRUE},
    {"false", FALSE},
    {"if", IF},
    {"else", ELSE},
    {"return", RETURN},
};

TokenType LookupIdentifier(std::string ident)
{
  if (keywords.find(ident) == keywords.end())
    return IDENT;
  return keywords[ident];
}

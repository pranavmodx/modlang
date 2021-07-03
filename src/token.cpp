#include "../header/token.hpp"

std::unordered_map<std::string, TokenType> keywords = {
    {"true", TRUE},
    {"false", FALSE},

    {"int", INTEGER},
    {"str", STRING},

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
    {"max_heap", MAX_HEAP},
    {"min_heap", MIN_HEAP},
};

TokenType LookupIdentifier(std::string ident)
{
  if (keywords.find(ident) == keywords.end())
    return IDENT;
  return keywords[ident];
}

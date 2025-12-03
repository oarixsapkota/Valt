#ifndef VALT_LEXER_H
#define VALT_LEXER_H

#include "token.h"

typedef struct Keyword Keyword;
struct Keyword {
  char *word;
  TokenType type;
};

Token *lexer(const char *buffer);
void free_tokens(Token *tokens);

#endif // VALT_LEXER_H
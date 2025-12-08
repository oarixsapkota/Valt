#ifndef VALT_LEXER_H
#define VALT_LEXER_H

#include "token.h"
#include "valt.h"

typedef struct {
  const char *buffer;
  uint64 index;
  uint64 line;
  uint64 col;
} Lexer;

Token *lexer(const char *buffer);

#endif // VALT_LEXER_H

#ifndef VALT_TOKEN_H
#define VALT_TOKEN_H

#include "valt.h"

typedef enum {
  // LEXER HELPER TOKENS
  TOKEN_START = 0,
  
  // TOKEN ID
  TOKEN_IDENTIFIER,

  // TOKEN Keywords
  // functions
  TOKEN_FUNC,
  TOKEN_RETURN,
  TOKEN_PASS,
  // Branching
  TOKEN_IF,
  TOKEN_ELIF,
  TOKEN_ELSE,
  // Looping
  TOKEN_DO,
  TOKEN_WHILE,
  TOKEN_FOR,
  TOKEN_GUARD,
  // Loop Control
  TOKEN_BREAK,
  TOKEN_CONTINUE,
  // Types
  TOKEN_VOID,
  TOKEN_CHAR,
  TOKEN_BOOL,
  TOKEN_INT,
  TOKEN_FLOAT,
  TOKEN_STRING,
  // Float types
  TOKEN_FLOAT32,
  TOKEN_FLOAT64,
  TOKEN_FLOAT128,
  // Int types
  TOKEN_INT8,
  TOKEN_INT16,
  TOKEN_INT32,
  TOKEN_INT64,
  TOKEN_UINT8,
  TOKEN_UINT16,
  TOKEN_UINT32,
  TOKEN_UINT64,
  // Misc
  TOKEN_OBJECT,
  TOKEN_ARRAY,

  // TOKEN Punctations
  TOKEN_O_PREN,
  TOKEN_C_PREN,
  TOKEN_O_BRACKET,
  TOKEN_C_BRACKET,
  TOKEN_O_BRACE,
  TOKEN_C_BRACE,
  TOKEN_SEMI_COLN,
  TOKEN_COLN,
  TOKEN_COMMA,
  TOKEN_QUESTION,
  // Assign
  TOKEN_ASSIGN,
  TOKEN_PLUS_EQ,
  TOKEN_MINUS_EQ,
  TOKEN_STAR_EQ,
  TOKEN_SLASH_EQ,
  TOKEN_PERC_EQ,
  // Access
  TOKEN_DOT,
  TOKEN_ARROW,
  // Unary
  TOKEN_2_PLUS,
  TOKEN_2_MINUS,
  TOKEN_BANG,
  // Additive
  TOKEN_PLUS,
  TOKEN_MINUS,
  // Multiplicative
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_PERCENT,
  // Relative
  TOKEN_EQUAL,
  TOKEN_NOT_EQ,
  // Equative
  TOKEN_GREATER,
  TOKEN_LESS,
  TOKEN_GREATER_EQ,
  TOKEN_LESS_EQ,
  // Logical
  TOKEN_AND,
  TOKEN_OR,
  // Adress
  TOKEN_ADDRESS_OF,

  // TOKEN Literal
  TOKEN_LIT_INT,
  TOKEN_LIT_FLOAT,
  TOKEN_LIT_CHARACTER,
  TOKEN_LIT_STRING,
  TOKEN_LIT_BOOLEAN_TRUE,
  TOKEN_LIT_BOOLEAN_FALSE,
  
  // Ending TOKEN
  TOKEN_END
} TokenType;

typedef struct {
  uint64 line;
  uint64 col;
} Pos;

typedef struct Token Token;
struct Token {
  TokenType type;
  char *value;
  Pos pos;
  Token *next;
};

typedef struct {
  char *word;
  TokenType type;
} Keyword;

const char *token_type_to_str(TokenType type);
TokenType str_to_token_type(const char *word);

void print_tokens(Token *tokens);

void add_token(Token **tail, Token token);
void free_s_token(Token *head);

#endif // VALT_TOKEN_H

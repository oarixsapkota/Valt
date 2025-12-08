#include "inc/token.h"

#include "inc/utils.h"

#include <string.h>

const char *token_type_to_str(TokenType type) {
  static const char *TOKEN_STR[TOKEN_END + 1] = {
      // tokens_to_str
      // HELPER TOKEN
      [TOKEN_START] = "TOKEN_START",
      // IDENTIFIER
      [TOKEN_IDENTIFIER] = "TOKEN_IDENTIFIER",
      // Keywords - functions
      [TOKEN_FUNC] = "TOKEN_FUNC",
      [TOKEN_RETURN] = "TOKEN_RETURN",
      [TOKEN_PASS] = "TOKEN_PASS",
      // Branching
      [TOKEN_IF] = "TOKEN_IF",
      [TOKEN_ELIF] = "TOKEN_ELIF",
      [TOKEN_ELSE] = "TOKEN_ELSE",
      // Looping
      [TOKEN_DO] = "TOKEN_DO",
      [TOKEN_WHILE] = "TOKEN_WHILE",
      [TOKEN_FOR] = "TOKEN_FOR",
      [TOKEN_GUARD] = "TOKEN_GUARD",
      // Loop control
      [TOKEN_BREAK] = "TOKEN_BREAK",
      [TOKEN_CONTINUE] = "TOKEN_CONTINUE",
      // Types
      [TOKEN_VOID] = "TOKEN_VOID",
      [TOKEN_CHAR] = "TOKEN_CHAR",
      [TOKEN_BOOL] = "TOKEN_BOOL",
      [TOKEN_INT] = "TOKEN_INT",
      [TOKEN_FLOAT] = "TOKEN_FLOAT",
      [TOKEN_STRING] = "TOKEN_STRING",
      // Float types
      [TOKEN_FLOAT32] = "TOKEN_FLOAT32",
      [TOKEN_FLOAT64] = "TOKEN_FLOAT64",
      [TOKEN_FLOAT128] = "TOKEN_FLOAT128",
      // Int types
      [TOKEN_INT8] = "TOKEN_INT8",
      [TOKEN_INT16] = "TOKEN_INT16",
      [TOKEN_INT32] = "TOKEN_INT32",
      [TOKEN_INT64] = "TOKEN_INT64",
      [TOKEN_UINT8] = "TOKEN_UINT8",
      [TOKEN_UINT16] = "TOKEN_UINT16",
      [TOKEN_UINT32] = "TOKEN_UINT32",
      [TOKEN_UINT64] = "TOKEN_UINT64",
      // Misc
      [TOKEN_OBJECT] = "TOKEN_OBJECT",
      [TOKEN_ARRAY] = "TOKEN_ARRAY",
      // Punctuations
      [TOKEN_O_PREN] = "TOKEN_O_PREN",
      [TOKEN_C_PREN] = "TOKEN_C_PREN",
      [TOKEN_O_BRACKET] = "TOKEN_O_BRACKET",
      [TOKEN_C_BRACKET] = "TOKEN_C_BRACKET",
      [TOKEN_O_BRACE] = "TOKEN_O_BRACE",
      [TOKEN_C_BRACE] = "TOKEN_C_BRACE",
      [TOKEN_SEMI_COLN] = "TOKEN_SEMI_COLN",
      [TOKEN_COLN] = "TOKEN_COLN",
      [TOKEN_COMMA] = "TOKEN_COMMA",
      [TOKEN_QUESTION] = "TOKEN_QUESTION",
      // Assignment
      [TOKEN_ASSIGN] = "TOKEN_ASSIGN",
      [TOKEN_PLUS_EQ] = "TOKEN_PLUS_EQ",
      [TOKEN_MINUS_EQ] = "TOKEN_MINUS_EQ",
      [TOKEN_STAR_EQ] = "TOKEN_STAR_EQ",
      [TOKEN_SLASH_EQ] = "TOKEN_SLASH_EQ",
      [TOKEN_PERC_EQ] = "TOKEN_PERC_EQ",
      // Access
      [TOKEN_DOT] = "TOKEN_DOT",
      [TOKEN_ARROW] = "TOKEN_ARROW",
      // Unary
      [TOKEN_2_PLUS] = "TOKEN_2_PLUS",
      [TOKEN_2_MINUS] = "TOKEN_2_MINUS",
      [TOKEN_BANG] = "TOKEN_BANG",
      // Additive
      [TOKEN_PLUS] = "TOKEN_PLUS",
      [TOKEN_MINUS] = "TOKEN_MINUS",
      // Multiplicative
      [TOKEN_STAR] = "TOKEN_STAR",
      [TOKEN_SLASH] = "TOKEN_SLASH",
      [TOKEN_PERCENT] = "TOKEN_PERCENT",
      // Relative
      [TOKEN_EQUAL] = "TOKEN_EQUAL",
      [TOKEN_NOT_EQ] = "TOKEN_NOT_EQ",
      [TOKEN_GREATER] = "TOKEN_GREATER",
      [TOKEN_LESS] = "TOKEN_LESS",
      [TOKEN_GREATER_EQ] = "TOKEN_GREATER_EQ",
      [TOKEN_LESS_EQ] = "TOKEN_LESS_EQ",
      // Logical
      [TOKEN_AND] = "TOKEN_AND",
      [TOKEN_OR] = "TOKEN_OR",
      // Address
      [TOKEN_ADDRESS_OF] = "TOKEN_ADDRESS_OF",
      // Literals
      [TOKEN_LIT_INT] = "TOKEN_LIT_INT",
      [TOKEN_LIT_FLOAT] = "TOKEN_LIT_FLOAT",
      [TOKEN_LIT_CHARACTER] = "TOKEN_LIT_CHARACTER",
      [TOKEN_LIT_STRING] = "TOKEN_LIT_STRING",
      [TOKEN_LIT_BOOLEAN_TRUE] = "TOKEN_LIT_BOOLEAN_TRUE",
      [TOKEN_LIT_BOOLEAN_FALSE] = "TOKEN_LIT_BOOLEAN_FALSE",
      // HELPER TOKEN
      [TOKEN_END] = "TOKEN_END"
      // End
  };
  return TOKEN_STR[type];
}

TokenType str_to_token_type(const char *word) {
  static const Keyword keyword[] = {
      {"void", TOKEN_VOID},              // void
      {"char", TOKEN_CHAR},              // unsigned int
      {"bool", TOKEN_BOOL},              // unsigned int
      {"int", TOKEN_INT},                // int
      {"int8", TOKEN_INT8},              // int
      {"int16", TOKEN_INT16},            // int
      {"int32", TOKEN_INT32},            // int
      {"int64", TOKEN_INT64},            // int
      {"uint8", TOKEN_UINT8},            // unsigned int
      {"uint16", TOKEN_UINT16},          // unsigned int
      {"uint32", TOKEN_UINT32},          // unsigned int
      {"uint64", TOKEN_UINT64},          // unsigned int
      {"float", TOKEN_FLOAT},            // float
      {"float32", TOKEN_FLOAT32},        // float
      {"float64", TOKEN_FLOAT64},        // float
      {"float128", TOKEN_FLOAT128},      // float
      {"string", TOKEN_STRING},          // string
      {"array", TOKEN_ARRAY},            // array
      {"object", TOKEN_OBJECT},          // object
      {"func", TOKEN_FUNC},              // function
      {"return", TOKEN_RETURN},          // return
      {"pass", TOKEN_PASS},              // pass
      {"if", TOKEN_IF},                  // if stmt
      {"elif", TOKEN_ELIF},              // else_if-clause
      {"else", TOKEN_ELSE},              // else-clause
      {"do", TOKEN_DO},                  // do
      {"while", TOKEN_WHILE},            // while
      {"for", TOKEN_FOR},                // for
      {"guard", TOKEN_GUARD},            // guard
      {"break", TOKEN_BREAK},            // break loop
      {"continue", TOKEN_CONTINUE},      // continue from top
      {"true", TOKEN_LIT_BOOLEAN_TRUE},  // true ( 1 )
      {"false", TOKEN_LIT_BOOLEAN_FALSE} // false ( 2 )
  };
  const uint64 n_keywords = sizeof(keyword) / sizeof(Keyword);
  for (uint64 i = 0; i < n_keywords; ++i) {
    if (strcmp(word, keyword[i].word) == 0)
      return keyword[i].type;
  }
  return TOKEN_IDENTIFIER;
}

void print_tokens(Token *tokens) {
  while (tokens) {
    Token *next = tokens->next;
    switch (tokens->type) {
      case TOKEN_IDENTIFIER:
        cprintf(MSG_YELLOW, "%s\t: %s\n", token_type_to_str(tokens->type), tokens->value);
        break;
      case TOKEN_LIT_CHARACTER:
        cprintf(MSG_GREEN, "%s\t: \'%s\'\n", token_type_to_str(tokens->type), tokens->value);
        break;
      case TOKEN_LIT_STRING:
        cprintf(MSG_GREEN, "%s\t: \"%s\"\n", token_type_to_str(tokens->type), tokens->value);
        break;
      case TOKEN_LIT_INT:
      case TOKEN_LIT_FLOAT:
        cprintf(MSG_BLUE, "%s\t\t: %s\n", token_type_to_str(tokens->type), tokens->value);
        break;
      case TOKEN_START:
      case TOKEN_END:
        cprintf(MSG_RED, "%s%s\n", STYLE_BOLD, token_type_to_str(tokens->type));
        break;
      default:
        cprintf(MSG_YELLOW, "%s\n", token_type_to_str(tokens->type));
        break;
    }
    tokens = next;
  }
}

void add_token(Token **tail, Token token) {
  Token *new_token = malloc_s(sizeof(Token));
  new_token->type = token.type;
  new_token->value = token.value;
  new_token->next = NULL;

  if (*tail) {
    (*tail)->next = new_token;
  }
  *tail = new_token;
}

void free_s_token(Token *head) {
  while (head) {
    Token *next = head->next;
    if (head->value != NULL) {
      free_s(head->value);
    }
    free_s(head);
    head = next;
  }
}
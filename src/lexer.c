#include "inc/lexer.h"

#include "inc/token.h"
#include "inc/valt.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *allocate_sub_string(const char *buffer, const uint64 start, const uint64 end) {
  const uint64 word_size = end - start;
  char *out = malloc(word_size + 1);
  if (!out) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }
  memcpy(out, buffer + start, word_size);
  out[word_size] = '\0';
  return out;
}

char *get_word(const char *buffer, uint64 *index, uint32 *char_num) {
  const uint64 start = *index;
  while (isalnum(buffer[*index]) || buffer[*index] == '_') {
    (*index)++;
    (*char_num)++;
  }
  return allocate_sub_string(buffer, start, *index);
}

TokenType get_word_type(const char *word) {
  const Keyword keyword[] = {
      // Type
      {"void", TOKEN_VOID},         // void
      {"char", TOKEN_CHAR},         // unsigned int
      {"bool", TOKEN_BOOL},         // unsigned int
      {"int", TOKEN_INT},           // int
      {"int8", TOKEN_INT8},         // int
      {"int16", TOKEN_INT16},       // int
      {"int32", TOKEN_INT32},       // int
      {"int64", TOKEN_INT64},       // int
      {"uint8", TOKEN_UINT8},       // unsigned int
      {"uint16", TOKEN_UINT16},     // unsigned int
      {"uint32", TOKEN_UINT32},     // unsigned int
      {"uint64", TOKEN_UINT64},     // unsigned int
      {"float", TOKEN_FLOAT},       // float
      {"float32", TOKEN_FLOAT32},   // float
      {"float64", TOKEN_FLOAT64},   // float
      {"float128", TOKEN_FLOAT128}, // float
      {"string", TOKEN_STRING},     // string
      {"array", TOKEN_ARRAY},       // array
      {"object", TOKEN_OBJECT},     // object
      // Function
      {"func", TOKEN_FUNC},     // function
      {"return", TOKEN_RETURN}, // return
      {"pass", TOKEN_PASS},     // pass
      // Branching
      {"if", TOKEN_IF},     // if stmt
      {"elif", TOKEN_ELIF}, // else_if-clause
      {"else", TOKEN_ELSE}, // else-clause
      // Looping
      {"do", TOKEN_DO},       // do
      {"while", TOKEN_WHILE}, // while
      {"for", TOKEN_FOR},     // for
      {"guard", TOKEN_GUARD}, // guard
      // Looping Control
      {"break", TOKEN_BREAK},       // break loop
      {"continue", TOKEN_CONTINUE}, // continue from top
      // Boolean Literal
      {"true", TOKEN_LIT_BOOLEAN_TRUE},  // true ( 1 )
      {"false", TOKEN_LIT_BOOLEAN_FALSE} // false ( 2 )
  }; // KYE-PARE
  const uint64 n_keywords = sizeof(keyword) / sizeof(Keyword);
  for (uint64 i = 0; i < n_keywords; ++i) {
    if (strcmp(word, keyword[i].word) == 0)
      return keyword[i].type;
  }
  return TOKEN_IDENTIFIER;
}

char *get_digit(const char *buffer, uint64 *index, uint32 *char_num, const uint64 line_num, uint8 *is_float) {
  const uint64 start = *index;
  *is_float = 0;
  while (isdigit(buffer[*index]) || buffer[*index] == '.') {
    if (buffer[*index] == '.') {
      if (!isdigit(buffer[*index + 1])) {
        fprintf(stderr, "Incomplete Float literal at line %lld char %d.\n", line_num, *char_num);
        return NULL;
      }
      (*is_float)++;
    }
    if (*is_float >= 2) {
      fprintf(stderr, "Invalid symbol `.' at line %lld char %d\n", line_num, *char_num);
      return NULL;
    }
    (*index)++;
    (*char_num)++;
  }
  return allocate_sub_string(buffer, start, *index);
}

char *get_char_lit(const char *buffer, uint64 *index, uint32 *char_num, const uint64 line_num) {
  (*index)++;
  const uint64 start = *index;
  while (buffer[*index] != '\'' && buffer[*index] != '\0' && buffer[*index] != '\n') {
    (*index)++;
    (*char_num)++;
  }
  if (buffer[*index] == '\0' || buffer[*index] == '\n') {
    fprintf(stderr, "Unterminated character literal at line %lld char %d.\n", line_num, *char_num);
    return NULL;
  }
  char *out = allocate_sub_string(buffer, start, *index);
  (*index)++;
  return out;
}

char *get_str_lit(const char *buffer, uint64 *index, uint32 *char_num, const uint64 line_num) {
  (*index)++;
  const uint64 start = *index;
  while (buffer[*index] != '"' && buffer[*index] != '\0' && buffer[*index] != '\n') {
    (*index)++;
    (*char_num)++;
  }
  if (buffer[*index] == '\0' || buffer[*index] == '\n') {
    fprintf(stderr, "Unterminated String literal at line %lld char %d.\n", line_num, *char_num);
    return NULL;
  }
  char *out = allocate_sub_string(buffer, start, *index);
  (*index)++;
  return out;
}

void push_token(Token *token_head, Token **token_tail, const TokenType type, char *value) {
  Token *new_token = malloc(sizeof(Token));
  if (!new_token) {
    fprintf(stderr, "Memory allocation failed.\n");
    free(token_head);
    exit(EXIT_FAILURE);
  }
  new_token->type = type;
  new_token->value = value;
  new_token->next = NULL;

  if (*token_tail) {
    (*token_tail)->next = new_token;
  }
  *token_tail = new_token;
}

void free_tokens(Token *token) {
  while (token) {
    Token *next = token->next;
    if (token->value != NULL) {
      free(token->value);
    }
    free(token);
    token = next;
  }
}

Token *lexer(const char *buffer) {
  uint64 index = 0;
  uint64 line_num = 1;
  uint32 char_num = 1;
  Token *head = malloc(sizeof(Token));
  if (!head) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  head->type = TOKEN_START;
  head->value = NULL;
  head->next = NULL;

  Token *token_tail = head;
  while (buffer[index] != '\0') {
    if (buffer[index] == '\n') {
      line_num++;
      char_num = 1;
      index++;
    } else if (isspace(buffer[index])) {
      index++;
      char_num++;
    } else if (isdigit(buffer[index]) || (buffer[index] == '.' && isdigit(buffer[index + 1]))) {
      uint8 is_float;
      char *digit = get_digit(buffer, &index, &char_num, line_num, &is_float);
      TokenType type = (is_float == 1) ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
      if (is_float >= 3)
        type = TOKEN_LIT_INT;
      push_token(head, &token_tail, type, digit);
    } else if (isalpha(buffer[index]) || buffer[index] == '_') {
      char *word = get_word(buffer, &index, &char_num);
      TokenType type = get_word_type(word);
      if (type != TOKEN_IDENTIFIER) {
        free(word);
        word = NULL;
        push_token(head, &token_tail, type, word);
      } else {
        if (buffer[index] == ':') {
          push_token(head, &token_tail, TOKEN_LABEL, word);
          index++;
          char_num++;
        } else {
          push_token(head, &token_tail, type, word);
        }
      }
    } else if (buffer[index] == 39) { // 39 = ' for char literal
      char *char_lit = get_char_lit(buffer, &index, &char_num, line_num);
      push_token(head, &token_tail, TOKEN_LIT_CHARACTER, char_lit);
    } else if (buffer[index] == 34) { // 34 = " for string literal
      char *str_lit = get_str_lit(buffer, &index, &char_num, line_num);
      push_token(head, &token_tail, TOKEN_LIT_STRING, str_lit);
    } else {
      switch (buffer[index]) {
        case '/':
          if (buffer[index + 1] == '/') {
            while (buffer[index] != '\n')
              index++;
          } else if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_SLASH_EQ, NULL);
            index++;
            char_num++;
          } else {
            push_token(head, &token_tail, TOKEN_SLASH, NULL);
            index++;
            char_num++;
          }
          break;

        case '=':
          if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_EQUAL, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_ASSIGN, NULL);
          index++;
          char_num++;
          break;
        case '>':
          if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_GREATER_EQ, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_GREATER, NULL);
          index++;
          char_num++;
          break;
        case '<':
          if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_LESS_EQ, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_LESS, NULL);
          index++;
          char_num++;
          break;

        case '+':
          if (buffer[index + 1] == '+') {
            push_token(head, &token_tail, TOKEN_2_PLUS, NULL);
            index++;
            char_num++;
          } else if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_PLUS_EQ, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_PLUS, NULL);
          index++;
          char_num++;
          break;
        case '-':
          if (buffer[index + 1] == '-') {
            push_token(head, &token_tail, TOKEN_2_MINUS, NULL);
            index++;
            char_num++;
          } else if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_MINUS_EQ, NULL);
            index++;
            char_num++;
          } else if (buffer[index + 1] == '>') {
            push_token(head, &token_tail, TOKEN_ARROW, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_MINUS, NULL);
          index++;
          char_num++;
          break;
        case '*':
          if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_STAR_EQ, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_STAR, NULL);
          index++;
          char_num++;
          break;
        case '%':
          if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_PERC_EQ, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_PERCENT, NULL);
          index++;
          char_num++;
          break;

        case '!':
          if (buffer[index + 1] == '=') {
            push_token(head, &token_tail, TOKEN_NOT_EQ, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_BANG, NULL);
          index++;
          char_num++;
          break;

        case '&':
          if (buffer[index + 1] == '&') {
            push_token(head, &token_tail, TOKEN_AND, NULL);
            index++;
            char_num++;
          } else
            push_token(head, &token_tail, TOKEN_ADDRESS_OF, NULL);
          index++;
          char_num++;
          break;
        case '|':
          if (buffer[index + 1] == '|') {
            push_token(head, &token_tail, TOKEN_OR, NULL);
            index++;
            char_num++;
          } else
            fprintf(stderr, "Invalid symbol `%c' at line %lld char %d\n", buffer[index], line_num, char_num);
          index++;
          char_num++;
          break;

        case '.':
          push_token(head, &token_tail, TOKEN_DOT, NULL);
          index++;
          char_num++;
          break;

        case '(':
          push_token(head, &token_tail, TOKEN_O_PREN, NULL);
          index++;
          char_num++;
          break;
        case ')':
          push_token(head, &token_tail, TOKEN_C_PREN, NULL);
          index++;
          char_num++;
          break;
        case '[':
          push_token(head, &token_tail, TOKEN_O_BRACKET, NULL);
          index++;
          char_num++;
          break;
        case ']':
          push_token(head, &token_tail, TOKEN_C_BRACKET, NULL);
          index++;
          char_num++;
          break;
        case '{':
          push_token(head, &token_tail, TOKEN_O_BRACE, NULL);
          index++;
          char_num++;
          break;
        case '}':
          push_token(head, &token_tail, TOKEN_C_BRACE, NULL);
          index++;
          char_num++;
          break;
        case ':':
          push_token(head, &token_tail, TOKEN_COLN, NULL);
          index++;
          char_num++;
          break;
        case ';':
          push_token(head, &token_tail, TOKEN_SEMI_COLN, NULL);
          index++;
          char_num++;
          break;
        case '?':
          push_token(head, &token_tail, TOKEN_QUESTION, NULL);
          index++;
          char_num++;
          break;
        case ',':
          push_token(head, &token_tail, TOKEN_COMMA, NULL);
          index++;
          char_num++;
          break;

        default:
          fprintf(stderr, "Invalid symbol `%c' at line %lld char %d\n", buffer[index], line_num, char_num);
          while (isspace(buffer[index]))
            index++;
          char_num++;
          index++;
          break;
      }
    }
  }
  push_token(head, &token_tail, TOKEN_END, NULL);
  return head;
}
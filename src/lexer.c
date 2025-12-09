#include "inc/lexer.h"

#include "inc/utils.h"

#include <ctype.h>
#include <string.h>

Lexer init_lexer(const char *buffer) {
  Lexer lexer;

  lexer.buffer = buffer;
  lexer.index = 0;
  lexer.line = 1;
  lexer.col = 0;

  return lexer;
}

char lexer_eat(Lexer *self) {
  char c = self->buffer[self->index];
  (self->index)++;
  (self->col)++;
  return c;
}

char lexer_peek(Lexer *self, int32 offset) {
  return self->buffer[self->index + offset];
}

char *allocate_sub_string(Lexer *self, const uint64 start) {
  const uint64 word_size = self->index - start;
  char *out = malloc_s(word_size + 1);
  memcpy(out, self->buffer + start, word_size);
  out[word_size] = '\0';
  return out;
}

Token get_word(Lexer *self) {
  uint64 start = self->index;
  while (isalnum(lexer_peek(self, 0)) || lexer_peek(self, 0) == '_')
    lexer_eat(self);

  char *value = allocate_sub_string(self, start);

  Token token;
  token = (Token){.pos.line = self->line, .pos.col = self->col};
  token.next = NULL;

  TokenType type = str_to_token_type(value);
  token.type = type;

  if (type != TOKEN_IDENTIFIER) {
    free_s(value);
    token.value = NULL;
  } else {
    token.value = value;
  }
  return token;
}

Token get_digit(Lexer *self) {
  uint64 start = self->index;
  bool isfloat = false;
  while (isdigit(lexer_peek(self, 0)) || lexer_peek(self, 0) == '.') {
    if (lexer_peek(self, 0) == '.') {
      if (!isdigit(lexer_peek(self, 1))) {
        char *value = allocate_sub_string(self, start);
        TokenType type = (isfloat) ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
        Token token = (Token){.type = type, .value = value, .next = NULL, .pos.line = self->line, .pos.col = self->col};
        break;
      }
      isfloat = true;
    }
    lexer_eat(self);
  }

  char *value = allocate_sub_string(self, start);

  TokenType type = (isfloat) ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
  Token token = (Token){.type = type, .value = value, .next = NULL, .pos.line = self->line, .pos.col = self->col};

  return token;
}

Token get_char_lit(Lexer *self) {
  lexer_eat(self);
  uint64 start = self->index;
  while (lexer_peek(self, 0) != '\'' && lexer_peek(self, 0) != '\0' && lexer_peek(self, 0) != '\n')
    lexer_eat(self);

  char *value = allocate_sub_string(self, start);

  Token token = (Token){.type = TOKEN_LIT_CHARACTER, .value = value, .next = NULL, .pos.line = self->line, .pos.col = self->col};

  lexer_eat(self);

  return token;
}

Token get_string_lit(Lexer *self) {
  lexer_eat(self);
  uint64 start = self->index;
  while (lexer_peek(self, 0) != '\"' && lexer_peek(self, 0) != '\0' && lexer_peek(self, 0) != '\n')
    lexer_eat(self);

  char *value = allocate_sub_string(self, start);

  Token token = (Token){.type = TOKEN_LIT_STRING, .value = value, .next = NULL, .pos.line = self->line, .pos.col = self->col};

  lexer_eat(self);

  return token;
}

/**
 * Lexer entry function.
 */
Token *lexer(const char *buffer) {
  Token *head = malloc_s(sizeof(Token));
  head->type = TOKEN_START;
  head->value = NULL;
  head->next = NULL;
  Token *tail = head;

  Lexer lexer = init_lexer(buffer);
  while (lexer_peek(&lexer, 0) != '\0') {
    char current_c = lexer.buffer[lexer.index];
    char next_c = lexer.buffer[lexer.index + 1];
    Token current_t;
    current_t = (Token){
        .type = TOKEN_END,
        .value = NULL,
        .pos.line = lexer.line,
        .pos.col = lexer.col,
        .next = NULL,
    };
    // Line && col counting
    if (current_c == '\n') {
      (lexer.line)++;
      (lexer.col) = 0;
    } else if (isalpha(current_c) || current_c == '_') {
      current_t = get_word(&lexer);
    } else if (isdigit(current_c) || (current_c == '.' && isdigit(next_c))) {
      current_t = get_digit(&lexer);
    } else {
      switch (current_c) {
        case '\'': // Lex Char literals
          current_t = get_char_lit(&lexer);
          break;

        case '\"': // Lex String Literals
          current_t = get_string_lit(&lexer);
          break;

        case '/':
          if (next_c == '/') {
            // Skip line-comments
            while (next_c != '\n') {
              lexer_eat(&lexer);
              next_c = lexer.buffer[lexer.index + 1];
            }
          } else if (next_c == '*') {
            // Skip Block-comments
            while (!(lexer_peek(&lexer, -1) == '*' && current_c == '/')) {
              lexer_eat(&lexer);
              current_c = lexer.buffer[lexer.index];
              if (current_c == '\n') {
                (lexer.line)++;
                (lexer.col) = 0;
              }
            }
          } else if (next_c == '=') {
            current_t.type = TOKEN_SLASH_EQ;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_SLASH;
          }
          break;

        case '%':
          if (next_c == '=') {
            current_t.type = TOKEN_PERC_EQ;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_PERCENT;
          }
          break;

        case '*':
          if (next_c == '=') {
            current_t.type = TOKEN_STAR_EQ;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_STAR;
          }
          break;

        case '+':
          if (next_c == '=') {
            current_t.type = TOKEN_PLUS_EQ;
            lexer_eat(&lexer);
          } else if (next_c == '+') {
            current_t.type = TOKEN_2_PLUS;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_PLUS;
          }
          break;

        case '-':
          if (next_c == '=') {
            current_t.type = TOKEN_MINUS_EQ;
            lexer_eat(&lexer);
          } else if (next_c == '-') {
            current_t.type = TOKEN_2_MINUS;
            lexer_eat(&lexer);
          } else if (next_c == '>') {
            current_t.type = TOKEN_ARROW;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_MINUS;
          }
          break;

        case '=':
          if (next_c == '=') {
            current_t.type = TOKEN_EQUAL;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_ASSIGN;
          }
          break;

        case '>':
          if (next_c == '=') {
            current_t.type = TOKEN_GREATER_EQ;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_GREATER;
          }
          break;

        case '<':
          if (next_c == '=') {
            current_t.type = TOKEN_LESS_EQ;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_LESS;
          }
          break;

        case '!':
          if (next_c == '=') {
            current_t.type = TOKEN_NOT_EQ;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_BANG;
          }
          break;

        case '&':
          if (next_c == '&') {
            current_t.type = TOKEN_AND;
            lexer_eat(&lexer);
          } else {
            current_t.type = TOKEN_ADDRESS_OF;
          }
          break;

        case '|':
          if (next_c == '|') {
            current_t.type = TOKEN_OR;
            lexer_eat(&lexer);
          }
          break;

        case '.':
          current_t.type = TOKEN_DOT;
          break;
        case '(':
          current_t.type = TOKEN_O_PREN;
          break;
        case ')':
          current_t.type = TOKEN_C_PREN;
          break;
        case '{':
          current_t.type = TOKEN_O_BRACE;
          break;
        case '}':
          current_t.type = TOKEN_C_BRACE;
          break;
        case '[':
          current_t.type = TOKEN_O_BRACKET;
          break;
        case ']':
          current_t.type = TOKEN_C_BRACKET;
          break;
        case ';':
          current_t.type = TOKEN_SEMI_COLN;
          break;
        case ':':
          current_t.type = TOKEN_COLN;
          break;
        case ',':
          current_t.type = TOKEN_COMMA;
          break;
        case '?':
          current_t.type = TOKEN_QUESTION;
          break;
        default:
          if (!isspace(current_c))
            cprintf(MSG_WARN, "unexpected symbol `%c' at line %llu col %llu\n", current_c, lexer.line, lexer.col);
          break;
      }
    }
    if (current_t.type != TOKEN_END) {
      add_token(&tail, current_t);
    }
    lexer_eat(&lexer);
  }
  add_token(&tail,
      (Token){
          .type = TOKEN_END,
          .value = NULL,
          .pos.col = 0,
          .pos.line = 0,
          .next = NULL,
      });
  print_tokens(head);
  return head;
}

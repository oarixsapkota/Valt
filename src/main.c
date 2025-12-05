#include "inc/lexer.h"
#include "inc/utils.h"

#include <stdio.h>
#include <stdlib.h>

void print_tokens(Token *token) {
  while (token) {
    Token *next = token->next;
    if (token->value != NULL) {
      if (token->type == TOKEN_IDENTIFIER) {
        printf("IDENT\t: %s\n", token->value);
      } else if (token->type == TOKEN_LABEL) {
        printf("LABEL\t: %s\n", token->value);
      } else if (token->type == TOKEN_LIT_INT || token->type == TOKEN_LIT_FLOAT) {
        printf("NUM\t: %s\n", token->value);
      } else if (token->type == TOKEN_LIT_STRING || token->type == TOKEN_LIT_CHARACTER) {
        printf("QUOT\t: %s\n", token->value);
      }
    } else {
      if (token->type == TOKEN_LIT_BOOLEAN_FALSE) {
        printf("BOOL\t: false\n");
      } else if (token->type == TOKEN_LIT_BOOLEAN_TRUE) {
        printf("BOOL\t: true\n");
      } else {
        printf("ID\t: %d\n", token->type);
      }
    }
    token = next;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Error : Incorrect usage.\n");
    printf("Correct usage : Valt <filename.vt>\n");
    exit(EXIT_FAILURE);
  }

  char *buffer = read_file(argv[1]);
  
  if (buffer == NULL) {
    fprintf(stderr, "FILE::ERROR\n");
    return 1;
  }

  Token *tokens = lexer(buffer);

  print_tokens(tokens);
  free_tokens(tokens);

  free(buffer);
  return 0;
}
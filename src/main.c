#include <stdio.h>
#include <stdlib.h>

#include "inc/lexer.h"

char *read_file(const char *file_name) {
  // open file.
  FILE *file_ptr = fopen(file_name, "rb");
  if (!file_ptr) {
    fprintf(stderr, "Error opening file %s\n", file_name);
    return NULL;
  }
  // seek to end to get file size.
  if (fseek(file_ptr, 0, SEEK_END) != 0) {
    fprintf(stderr, "Error seeking end of file %s\n", file_name);
    fclose(file_ptr);
    return NULL;
  }
  long file_size = ftell(file_ptr);
  // check if file size is correct.
  if (file_size < 0) {
    fclose(file_ptr);
    fprintf(stderr, "Error reading file %s\n", file_name);
    return NULL;
  }
  rewind(file_ptr);
  // allocate memory for the buffer.
  char *buffer = (char *)malloc(file_size + 1);
  if (!buffer) {
    fclose(file_ptr);
    fprintf(stderr, "Error allocating memory\n");
    return NULL;
  }
  // get the file into buffer.
  size_t file_length = fread(buffer, 1, (size_t)file_size, file_ptr);
  fclose(file_ptr);
  buffer[file_length] = '\0';
  return buffer;
}

void print_tokens(Token *token) {
  while (token) {
    Token *next = token->next;
    if (token->value != NULL) {
      if (token->type == TOKEN_IDENTIFIER) {
        printf("IDENT\t: %s\n", token->value);
      } else if (token->type == TOKEN_LABEL) {
        printf("LABEL\t: %s\n", token->value);
      } else if (token->type == TOKEN_LIT_INT ||
                 token->type == TOKEN_LIT_FLOAT) {
        printf("NUM\t: %s\n", token->value);
      } else if (token->type == TOKEN_LIT_STRING ||
                 token->type == TOKEN_LIT_CHARACTER) {
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

int main(void) {
  char *file_name = "../test/tok.vt";
  char *buffer = read_file(file_name);
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
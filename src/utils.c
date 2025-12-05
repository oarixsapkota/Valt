#include "inc/utils.h"

#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *file_name) {
  // open file.
  FILE *file_ptr = fopen(file_name, "rb");
  if (!file_ptr) {
    fprintf(stderr, "Error : Can't open file : %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  // seek to end to get file size.
  if (fseek(file_ptr, 0, SEEK_END) != 0) {
    fprintf(stderr, "Error : Seeking failed : %s\n", file_name);
    fclose(file_ptr);
    exit(EXIT_FAILURE);
  }
  long file_size = ftell(file_ptr);
  // check if file size is correct.
  if (file_size <= 0) {
    fclose(file_ptr);
    fprintf(stderr, "Error : Incorrect file size : %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  rewind(file_ptr);
  // allocate memory for the buffer.
  char *buffer = (char *)malloc(file_size + 1);
  if (!buffer) {
    fclose(file_ptr);
    fprintf(stderr, "Error : Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  // get the file into buffer.
  size_t file_length = fread(buffer, 1, (size_t)file_size, file_ptr);
  if (file_length != file_size) {
    fclose(file_ptr);
    free(buffer);
    fprintf(stderr, "Error : Can't read file : %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  fclose(file_ptr);
  buffer[file_length] = '\0';
  return buffer;
}
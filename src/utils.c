#include "inc/utils.h"

#include <stdio.h>
#include <stdlib.h>

void cprintf(MsgType type, const char *fmt, ...) {
  char *color;

  switch (type) {
    case MSG_RED:
      color = COLOR_RED;
      break;
    case MSG_GREEN:
      color = COLOR_GREEN;
      break;
    case MSG_BLUE:
      color = COLOR_BLUE;
      break;
    case MSG_YELLOW:
      color = COLOR_YELLOW;
      break;
    case MSG_INFO:
      printf("%s%sINFO%s    : ", STYLE_BOLD, COLOR_GREEN, STYLE_RESET);
      color = COLOR_BLUE;
      break;
    case MSG_WARN:
      printf("%s%sWARINIG%s : ", STYLE_BOLD, COLOR_YELLOW, STYLE_RESET);
      color = STYLE_RESET;
      break;
    case MSG_ERROR:
      printf("%s%sERROR%S   : ", STYLE_BOLD, COLOR_RED, STYLE_RESET);
      color = STYLE_RESET;
      break;
    case MSG_FAITAL:
      printf("%s%sFAITAL%s  : ", STYLE_BOLD, COLOR_RED, STYLE_RESET);
      color = STYLE_BOLD;
      break;
    case MSG:
    default:
      color = STYLE_RESET;
      break;
  }

  printf("%s%S", color, STYLE_RESET);

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  printf(STYLE_RESET);
}

char *read_file(const char *file_name) {
  // open file.
  FILE *file_ptr = fopen(file_name, "rb");
  if (!file_ptr) {
    cprintf(MSG_FAITAL, "Can't open file : %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  // seek to end to get file size.
  if (fseek(file_ptr, 0, SEEK_END) != 0) {
    cprintf(MSG_FAITAL, "Seeking failed : %s\n", file_name);
    fclose(file_ptr);
    exit(EXIT_FAILURE);
  }
  long file_size = ftell(file_ptr);
  // check if file size is correct.
  if (file_size <= 0) {
    fclose(file_ptr);
    cprintf(MSG_FAITAL, "Incorrect file size : %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  rewind(file_ptr);
  // allocate memory for the buffer.
  char *buffer = (char *)malloc_s(file_size + 1);
  // get the file into buffer.
  size_t file_length = fread(buffer, 1, (size_t)file_size, file_ptr);
  if (file_length != file_size) {
    fclose(file_ptr);
    free(buffer);
    cprintf(MSG_FAITAL, "Can't read file : %s\n", file_name);
    exit(EXIT_FAILURE);
  }
  fclose(file_ptr);
  buffer[file_length] = '\0';
  return buffer;
}

void *malloc_s(uint64 size) {
  if (size == 0) {
    cprintf(MSG_FAITAL, "malloc called with size 0.\n");
    exit(EXIT_FAILURE);
  }
  void *ptr = malloc(size);
  if (ptr == NULL) {
    cprintf(MSG_FAITAL, "malloc failed to allocate %llu bytes.\n", size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *calloc_s(uint64 nmemb, uint64 size) {
  if (size == 0) {
    cprintf(MSG_FAITAL, "calloc called with size 0.\n");
    exit(EXIT_FAILURE);
  }
  void *ptr = calloc(nmemb, size);
  if (ptr == NULL) {
    cprintf(MSG_FAITAL, "calloc failed to allocate %llu bytes.\n", size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *realloc_s(void *ptr, uint64 size) {
  if (size == 0) {
    free_s(ptr);
    cprintf(MSG_FAITAL, "relloc called with size 0.\n");
    exit(EXIT_FAILURE);
  }
  void *ptr_n;
  if (ptr != NULL) {
    ptr_n = realloc(ptr, size);
  } else {
    cprintf(MSG_WARN, "realloc called with NULL ptr returns NULL.\n");
    return NULL;
  }

  if (ptr_n == NULL) {
    cprintf(MSG_FAITAL, "realloc failed to allocate %llu bytes.\n", size);
    exit(EXIT_FAILURE);
  }
  return ptr_n;
}

void free_s(void *ptr) {
  if (ptr != NULL) {
    free(ptr);
  } else {
    cprintf(MSG_WARN, "can't free a null pointer.\n");
  }
}

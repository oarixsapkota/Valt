#ifndef VALT_UTILS_H
#define VALT_UTILS_H

#include "valt.h"

#include <stdarg.h>

// Output
typedef enum {
  MSG,
  MSG_RED,
  MSG_GREEN,
  MSG_BLUE,
  MSG_YELLOW,
  MSG_INFO,
  MSG_WARN,
  MSG_ERROR,
  MSG_FAITAL,
} MsgType;

void cprintf(MsgType type, const char *fmt, ...);

// File
char *read_file(const char *file_name);

// Memory
void *malloc_s(uint64 size);
void *calloc_s(uint64 nmemb, uint64 size);
void *realloc_s(void *ptr, uint64 size);
void free_s(void *ptr);

#endif // VALT_UTILS_H

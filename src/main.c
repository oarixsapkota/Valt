#include "inc/lexer.h"
#include "inc/utils.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    cprintf(MSG_FAITAL, "incorrect usage.\n");
    cprintf(MSG_INFO,"Correct usage :: Valt <filename.vt>\n");
    exit(EXIT_FAILURE);
  }
  char *buffer = read_file(argv[1]);
  lexer(buffer);
  free_s(buffer);
  return 0;
}

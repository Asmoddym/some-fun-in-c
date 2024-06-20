#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_line.h"

int main() {
  // char *a = get_line();
  // printf("%s, %lu\n", a == NULL ? "NULL" : a, a == NULL ? 0 : strlen(a));
  // free(a);

  // a = get_line();
  // printf("%s, %lu\n", a == NULL ? "NULL" : a, a == NULL ? 0 : strlen(a));
  // free(a);
  // a = get_line();
  // printf("%s, %lu\n", a == NULL ? "NULL" : a, a == NULL ? 0 : strlen(a)); 
  // free(a);

  char *a;

  while ((a = get_line()) != NULL) {
    printf("%s, %lu\n", a, strlen(a));
    free(a);
  }

  get_line_buffer(GETLINE_DESTROY_BUFFER);
  
}

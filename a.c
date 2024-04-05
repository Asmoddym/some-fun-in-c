#include <string.h>
#include <stdio.h>
#include "get_line.h"

int main() {
  const char *a = get_line();
  printf("%s, %lu\n", a == NULL ? "NULL" : a, a == NULL ? 0 : strlen(a));

a = get_line();
  printf("%s, %lu\n", a == NULL ? "NULL" : a, a == NULL ? 0 : strlen(a));
  a = get_line();
  printf("%s, %lu\n", a == NULL ? "NULL" : a, a == NULL ? 0 : strlen(a)); 

  get_line_buffer(GETLINE_DESTROY_BUFFER);
  
}

#include <unistd.h>

#include "my_strlen.h"

void my_perror(const char *message) {
  write(2, message, my_strlen(message));
}

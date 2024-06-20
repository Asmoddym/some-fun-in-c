#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "get_line.h"
#include "my_perror.h"

#include <stdio.h>
# define BUFFER_SIZE 256

bool is_delimiter(char c) {
  return c == 0 || c == '\n';
}

int buffer_data_size_to_copy(const t_chunk *buffer) {
  int size = 0;

  for (; size < BUFFER_SIZE && !is_delimiter(buffer->data[size]); size++);

  return size;
}

bool read_from_buffer(t_chunk *buffer, t_chunk *chunk) {
  int size = buffer_data_size_to_copy(buffer);
  char *tmp = strncpy(malloc(sizeof(char) * (size + 1)), buffer->data, size);

  // If a delimiter was found, we need to skip it as it will be at the beginning of the future buffer.
  // index_offset will equal 1 to allow the delimiter to be skipped.
  int index_offset = (size == BUFFER_SIZE || buffer->data[size] == 0 ? 0 : 1);

  buffer->data = buffer->data + size + index_offset;
  buffer->size = buffer->size - size - index_offset;

  chunk->data = tmp;
  chunk->data[size] = 0;
  chunk->size = size;

  return index_offset != 0;
}

void append_to_line(const t_chunk *chunk, t_chunk *line) {
  int new_size = line->size + chunk->size;

  line->data = realloc(line->data, sizeof(char) * (new_size + 1));

  strcpy(line->data + line->size, chunk->data);
  line->size = new_size;
}

t_chunk *get_line_buffer(int mode) {
  static t_chunk *buffer = NULL;

  if (mode == GETLINE_GET_BUFFER) {
    if (buffer == NULL) {
      buffer = malloc(sizeof(t_chunk));
      buffer->handle = malloc(sizeof(char) * BUFFER_SIZE);
      buffer->data = buffer->handle;
      buffer->data[0] = 0;
      buffer->size = 0;
    }
  } else if (mode == GETLINE_DESTROY_BUFFER) {
    free(buffer->handle);
    free(buffer);
  } else {
    my_perror("Unhandled get_line mode");

    exit(1);
  }

  return buffer;
}

char *get_line() {
  t_chunk *buffer =  get_line_buffer(GETLINE_GET_BUFFER);
  t_chunk line = { .data = NULL, .size = 0};
  t_chunk chunk = { .data = NULL, .size = 0};
  bool delimiter_found = 0;

  while (!delimiter_found) {
    if (buffer->size == 0) {
      buffer->size = read(0, buffer->data, BUFFER_SIZE);

      if (buffer->size == 0) {
        break;
      }
    }

    delimiter_found = read_from_buffer(buffer, &chunk);
    append_to_line(&chunk, &line);
  }

  return line.data;
}

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "my_get_line.h"
#include "my_perror.h"

#include <stdio.h>

bool is_delimiter(char c) {
  return c == 0 || c == '\n';
}

int buffer_data_size_to_copy(const t_chunk *buffer) {
  int size = 0;

  for (; size < BUFFER_SIZE && size < buffer->size && !is_delimiter(buffer->data[size]); size++);

  return size;
}

bool read_from_buffer_and_append_to_line(t_chunk *buffer, t_chunk *line) {
  int size = buffer_data_size_to_copy(buffer);
  char *tmp = strncpy(malloc(sizeof(char) * (size + 1)), buffer->data, size);
  tmp[size] = 0;

  // If a delimiter was found, we need to skip it as it will be at the beginning of the future buffer.
  // index_offset will equal 1 to allow the delimiter to be skipped.
  int index_offset = size != buffer->size;

  buffer->data = buffer->data + size + index_offset;
  buffer->size = buffer->size - size - index_offset;

  line->data = realloc(line->data, sizeof(char) * (line->size + size + 1));
  strcpy(line->data + line->size, tmp);
  line->size += size;
  free(tmp);

  return index_offset != 0;
}

t_chunk *get_line_buffer(int mode) {
  static t_chunk *buffer = NULL;

  if (mode == GETLINE_GET_BUFFER) {
    if (buffer == NULL) {
      buffer = calloc(sizeof(t_chunk), 1);
      buffer->data = buffer->handle;
    }
  } else if (mode == GETLINE_DESTROY_BUFFER) {
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
  bool delimiter_found = 0;

  while (!delimiter_found) {
    if (buffer->size <= 0) {
      buffer->data = buffer->handle;
      buffer->size = read(0, buffer->data, BUFFER_SIZE);

      if (buffer->size <= 0) {
        break;
      }
    }

    delimiter_found = read_from_buffer_and_append_to_line(buffer, &line);
  }

  return line.data;
}

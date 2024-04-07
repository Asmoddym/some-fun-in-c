#include <unistd.h>
#include <stdlib.h>

#include "get_line.h"
#include "my_perror.h"

# define BUFFER_SIZE 256

void write_to_buffer(t_chunk *buffer) {
  buffer->size = read(0, buffer->data, BUFFER_SIZE);
}

bool is_delimiter(char c) {
  return c == 0 || c == '\n';
}

bool read_from_buffer(t_chunk *buffer, t_chunk *chunk) {
  int size = 0;
  char *tmp;

  for (; size < BUFFER_SIZE && !is_delimiter(buffer->data[size]); size++);

  tmp = malloc(sizeof(char) * (size + 1));
  for (int i = 0; i < size; i++) {
    tmp[i] = buffer->data[i];
  }
  tmp[size] = 0;

  int offset = (size == BUFFER_SIZE) || buffer->data[size] == 0 ? 0 : 1;

  for (int i = 0; size + offset + i < buffer->size && buffer->data[size + offset + i] != 0; i++) {
    buffer->data[i] = buffer->data[size + offset + i];
  }
  buffer->data[buffer->size - size - offset] = 0;
  buffer->size -= size + offset;

  chunk->data = tmp;
  chunk->size = size;

  return offset != 0;
}

void append_to_line(const t_chunk *chunk, t_chunk *line) {
  int new_size = line->size + chunk->size;
  char *new_data = malloc(sizeof(char) * (new_size) + 1);

  for (int i = 0; i < line->size; i++) {
    new_data[i] = line->data[i];
  }

  for (int i = 0; i < chunk->size; i++) {
    new_data[line->size + i] = chunk->data[i];
  }
  new_data[new_size] = 0;

  free(chunk->data);

  if (line->data != NULL) {
    free(line->data);
  }

  line->data = new_data;
  line->size = new_size;
}

t_chunk *get_line_buffer(int mode) {
  static t_chunk *buffer = NULL;

  if (mode == GETLINE_GET_BUFFER) {
    if (buffer == NULL) {
      buffer = malloc(sizeof(t_chunk));
      buffer->data = malloc(sizeof(char) * BUFFER_SIZE);
      buffer->data[0] = 0;
      buffer->size = 0;
    }
  } else if (mode == GETLINE_DESTROY_BUFFER) {
    free(buffer->data);
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
      write_to_buffer(buffer);

      if (buffer->size == 0) {
        break;
      }
    }

    delimiter_found = read_from_buffer(buffer, &chunk);
    append_to_line(&chunk, &line);
  }

  return line.data;
}

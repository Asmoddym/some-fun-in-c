#ifndef GET_LINE_H_
# define GET_LINE_H_

typedef struct {
  void *handle;
  char *data;
  int size;
} t_chunk;

char *get_line();

t_chunk *get_line_buffer(int mode);

# define bool char

# define GETLINE_GET_BUFFER 0
# define GETLINE_DESTROY_BUFFER 1

#endif

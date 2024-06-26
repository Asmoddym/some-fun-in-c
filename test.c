#include <stdio.h>

void test(int i)
{
  printf("coucou\n");
}

int main()
{
  void *a = test;

  ((void (*)(int))a)(1);
}

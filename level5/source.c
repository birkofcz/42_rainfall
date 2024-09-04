#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void o(void)
{
  system("/bin/sh");
  _exit(1);
}

void n(void)
{
  char input_buffer[520];
  fgets(input_buffer, 512, stdin);
  printf(input_buffer);
  exit(1);
}

int main(void)
{
  n();
  return 0;
}

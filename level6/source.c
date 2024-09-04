#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void n(void)
{
  system("/bin/cat /home/user/level7/.pass");
  return;
}

void m(void)
{
  puts("Nope");
  return;
}

int main(int argc, char **argv)
{
  char *input_buffer;
  void (*f_ptr)(void);

  input_buffer = (char *)malloc(64);
  f_ptr = (void **)malloc(4);
  *f_ptr = m;
  strcpy(input_buffer, argv[1]);
  f_ptr();
  return (0);
}

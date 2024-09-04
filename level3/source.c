#include <stdio.h>
#include <stdlib.h>

unsigned int m;

void v(void)
{
  char input_buffer[520];
  
  fgets(input_buffer, 512, stdin);
  printf(input_buffer);
  if (m == 64)
  {
    fwrite("Wait what?!\n", 1, 12, stdout);
    system("/bin/sh");
  }
  return;
}

int main(void)
{
  v();
  return 0;
}

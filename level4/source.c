#include <stdio.h>
#include <stdlib.h>

unsigned int m;

void p(char *input_buffer)
{
  printf(input_buffer);
  return;
}

void n(void)
{
  char input_buffer[520];

  fgets(input_buffer, 512, stdin);
  p(input_buffer);
  if (m == 16930116)
    system("/bin/cat /home/user/level5/.pass");

  return;
}

int main(void)
{
  n();
  return 0;
}

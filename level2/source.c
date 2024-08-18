#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	p(void)
{
	char			buff[76];
	unsigned int	ret_addr;

	fflush(stdout);
	gets(buff);
	ret_addr = *(unsigned int *)(buff + 80);
	if ((ret_addr & 0xb0000000) == 0xb0000000)
	{
		printf("%p\n", ret_addr);
		exit(1);
	}
	puts(buff);
	strdup(buff);
}

int main(void)
{
	p();
	return (0);
}
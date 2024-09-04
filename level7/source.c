#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char pw_buffer[100];

void		m(void)
{
	time_t	current_time;

	current_time = time((time_t *)0x0);
	printf("%s - %d\n", pw_buffer, current_time);
	return;
}

int			main(int argc, char **argv)
{
	void	*ptr1;
	void	*ptr2;
	void	*ptr3;
	FILE	*fd;

	ptr1 = (void *)malloc(8);
	*ptr1 = 1; 
	ptr2 = (void *)malloc(8);
	*(void **)(ptr1 + 1) = ptr2;
	ptr3 = (void *)malloc(8);
	*ptr3 = 2;
	ptr2 = (void *)malloc(8);
	*(void **)(ptr3 + 1) = ptr2;
  
	strcpy((char *)ptr1[1], argv[1]);
	strcpy((char *)ptr3[1], argv[2]);
  
	fd = fopen("/home/user/level8/.pass", "r");
	fgets(pw_buffer, 68, fd);
	puts("~~");
	return 0;
}

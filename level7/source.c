#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char pw_buffer[68];

void m(void)
{
	time_t	current_time;

	current_time = time((time_t *)0x0);
	printf("%s - %d\n", pw_buffer, current_time);
	return;
}

int main(int argc, char **argv)
{
    int *struct1 = (int *)malloc(8);
    struct1[0] = 1;
    struct1[1] = (int)malloc(8);

    int *struct2 = (int *)malloc(8);
    struct2[0] = 2;
    struct2[1] = (int)malloc(8);
    FILE *fd;

    strcpy((char *)struct1[1], argv[1]);  
    strcpy((char *)struct2[1], argv[2]);

    FILE *file = fopen("/home/user/level8/.pass", "r");
    if (file != NULL) {
        fgets(pw_buffer, sizeof(pw_buffer), file);
        fclose(file);                              
    }

    puts("~~");
    return 0;
}

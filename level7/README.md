## Basic analysis
~~~shell
level7@RainFall:~$ ./level7
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 42
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 42 42
~~
~~~
OK, so binary refuses to talk to us until there are two arguments passed. Let's examine with **Ghidra** tool.
~~~C
undefined4 main(undefined4 param_1,int param_2)
{
  undefined4 *puVar1;
  void *pvVar2;
  undefined4 *puVar3;
  FILE *__stream;
  
  puVar1 = (undefined4 *)malloc(8);
  *puVar1 = 1;
  pvVar2 = malloc(8);
  puVar1[1] = pvVar2;
  puVar3 = (undefined4 *)malloc(8);
  *puVar3 = 2;
  pvVar2 = malloc(8);
  puVar3[1] = pvVar2;
  strcpy((char *)puVar1[1],*(char **)(param_2 + 4));
  strcpy((char *)puVar3[1],*(char **)(param_2 + 8));
  __stream = fopen("/home/user/level8/.pass","r");
  fgets(c,0x44,__stream);
  puts("~~");
  return 0;
}
~~~
It allocates some memory for parameters, but the main part is the file stream, which is obvisously read into the variable **c**, which is not used here. Let's dig further...unti; we find the 
**m()** function that is printing the c variable (also doe some other useless stuff..).
~~~C
void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)
{
  time_t param2;
  
  param2 = time((time_t *)0x0);
  printf("%s - %d\n",c,param2);
  return;
}
~~~
S,we need to run the **m()** somehow. We can probably use the 

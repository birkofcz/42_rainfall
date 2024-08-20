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

